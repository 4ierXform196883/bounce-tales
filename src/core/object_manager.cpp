#include "object_manager.hpp"

#include "game.hpp"
#include <queue>
#include "collision_calculator.hpp"
#include "curved_shape.hpp"
#include "earcut.hpp"
#include "jump_pad.hpp"
#include "platform.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define dot(v1, v2) (v1.x * v2.x + v1.y * v2.y)

using json = nlohmann::json;

void ObjectManager::loadObject(std::shared_ptr<GameObject> obj, const nlohmann::json &data)
{
    if (data.contains("pos"))
        obj->setPosition(sf::Vector2f(data["pos"][0], data["pos"][1]));
    if (data.contains("scale"))
        obj->setScale(sf::Vector2f(data["scale"][0], data["scale"][1]));
    if (data.contains("origin"))
        obj->setOrigin(sf::Vector2f(data["origin"][0], data["origin"][1]));
    if (data.contains("rotation"))
        obj->setRotation(data["rotation"]);
    if (data.contains("mass") && obj->isPhysical())
        std::dynamic_pointer_cast<IPhysical>(obj)->setMass(data["mass"]);
    if (data.contains("air_resistance") && obj->isPhysical())
        std::dynamic_pointer_cast<IPhysical>(obj)->setAirResistance(data["air_resistance"]);
}

void ObjectManager::load(const std::string &path)
{
    const_cast<sf::View &>(defaultView) = sf::View({0, 0}, Game::getSettings().camera_size);

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] (ObjectManager::load) Couldn't open file\n";
        return;
    }
    json data;
    file >> data;
    file.close();
    auto &curData = data["camera"];
    camera = std::make_shared<Camera>(sf::Vector2f(curData["size"][0], curData["size"][1]));
    loadObject(camera, curData);
    curData = data["background"];
    background = std::make_shared<Background>(curData["islands"], curData["clouds"], curData["additional_distance"]);
    curData = data["player"];
    player = std::make_shared<Player>(curData["control_force"]);
    loadObject(player, curData);
    for (const auto &part : data["ground"])
    {
        std::vector<sf::Vector2f> verts;
        verts.reserve(part["verts"].size());

        for (const auto &point : part["verts"])
            verts.emplace_back(sf::Vector2f(point[0], point[1]));

        auto ptr = std::make_shared<Ground>(std::move(verts), part["texture"]);
        loadObject(ptr, part);
        drawable.emplace(ptr->getTag(), ptr);
        collidable.emplace(ptr->getTag(), ptr);
    }

    for (const auto &part : data["platforms"])
    {
        std::vector<sf::Vector2f> verts, path;
        verts.reserve(part["verts"].size());
        path.reserve(part["path"].size());

        for (const auto &point : part["verts"])
            verts.emplace_back(sf::Vector2f(point[0], point[1]));
        for (const auto &point : part["path"])
            path.emplace_back(sf::Vector2f(point[0], point[1]));

        auto ptr = std::make_shared<Platform>(std::move(verts), part["texture"], std::move(path), part["speed_mult"]);
        loadObject(ptr, part);
        drawable.emplace(ptr->getTag(), ptr);
        updatable.emplace(ptr->getTag(), ptr);
        collidable.emplace(ptr->getTag(), ptr);
        physical.emplace(ptr->getTag(), ptr);
    }

    for (const auto &part : data["jump_pads"])
    {
        auto ptr = std::make_shared<JumpPad>(part["power"]);
        loadObject(ptr, part);
        drawable.emplace(ptr->getTag(), ptr);
        updatable.emplace(ptr->getTag(), ptr);
        collidable.emplace(ptr->getTag(), ptr);
    }
    if (data["camera"].contains("follow") && drawable.find(data["camera"]["follow"]) != drawable.end())
        camera->setFollowObject(drawable.at(data["camera"]["follow"]));
}

// IMPLEMENT
void ObjectManager::save(const std::string &path)
{
}

void ObjectManager::updateAll()
{
    // DEBUG
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        player->setPosition(Game::getMousePos());
        player->addForce(-player->getSpeed());
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        camera->setPosition(Game::getMousePos());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
    {
        camera->setFollowObject(player);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        camera->setFollowObject(nullptr);
    }
    // =====
    GameObject::update(camera);
    GameObject::update(background);
    for (auto it = updatable.begin(); it != updatable.end();)
    {
        if (!it->second->isAlive())
        {
            it = updatable.erase(it);
            continue;
        }
        GameObject::update(it->second);
        ++it;
    }
    GameObject::update(player);
}

void ObjectManager::collideAll()
{
    for (auto it = physical.begin(); it != physical.end();)
    {
        if (!it->second->isAlive())
        {
            it = physical.erase(it);
            continue;
        }
        for (auto itt = collidable.begin(); itt != collidable.end();)
        {
            if (!itt->second->isAlive())
            {
                itt = collidable.erase(itt);
                continue;
            }
            GameObject::calculateCollision(it->second, itt->second);
            ++itt;
        }
        ++it;
    }

    for (auto col : collidable)
        GameObject::calculateCollision(player, col.second);
    for (auto sPhys : physical)
        GameObject::calculateCollision(player, sPhys.second);
}

void ObjectManager::drawAll(sf::RenderTarget &target)
{
    target.setView(defaultView);
    GameObject::draw(background, target);
    target.setView(this->getCamera());
    for (auto it = drawable.begin(); it != drawable.end();)
    {
        if (!it->second->isAlive())
        {
            it = drawable.erase(it);
            continue;
        }
        GameObject::draw(it->second, target);
        ++it;
    }
    GameObject::draw(player, target);
}

void ObjectManager::addObject(std::shared_ptr<GameObject> object)
{
    drawable.emplace(object->getTag(), object);
    updatable.emplace(object->getTag(), object);
    if (object->isCollidable())
        collidable.emplace(object->getTag(), object);
    if (object->isPhysical())
        physical.emplace(object->getTag(), object);
}

void ObjectManager::addObjects(const std::vector<std::shared_ptr<GameObject>> &objects)
{
    for (const auto& object: objects)
        this->addObject(object);
}

sf::View ObjectManager::getCamera() const
{
    if (camera)
        return camera->getView();
    return defaultView;
}

// void ObjectManager::recurDatasiveCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second)
// {
// std::queue<std::shared_ptr<GameObject>> firstObjects;
// firstObjects.push(first);
// while (firstObjects.size() != 0)
// {
//     std::shared_ptr<GameObject> firstCur = firstObjects.front();

//     // Another BFS
//     std::queue<std::shared_ptr<GameObject>> secondObjects;
//     secondObjects.push(second);
//     while (secondObjects.size() != 0)
//     {
//         std::shared_ptr<GameObject> secondCur = secondObjects.front();
//         if (firstCur->getCollidable() && secondCur->getCollidable() && !(firstCur->getCollidable()->isTrigger() && secondCur->getCollidable()->isTrigger()))
//             calculateCollision(firstCur, secondCur);
//         for (auto secondChild : secondCur->getChildren())
//             secondObjects.push(secondChild);
//         secondObjects.pop();
//     }
//     // ==========================

//     for (auto firstChild : firstCur->getChildren())
//         firstObjects.push(firstChild);
//     firstObjects.pop();
// }
// }