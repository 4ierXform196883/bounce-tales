#include "object_manager.hpp"

#include "game.hpp"
#include <queue>
#include "collision_calculator.hpp"
#include "curved_shape.hpp"
#include "earcut.hpp"
#include "jump_pad.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define dot(v1, v2) (v1.x * v2.x + v1.y * v2.y)

using json = nlohmann::json;

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

    sf::Vector2f vec = {data["camera"]["pos"][0], data["camera"]["pos"][1]};
    ;
    camera = std::make_shared<Camera>(vec, Game::getSettings().camera_size);
    auto &cur = data["background"];
    background = std::make_shared<Background>(cur["islands"], cur["clouds"], cur["additional_distance"]);
    cur = data["player"];
    vec = {cur["pos"][0], cur["pos"][1]};
    player = std::make_shared<Player>(vec, cur["control_force"], cur["max_speed"], cur["friction"], cur["gravity"]);
    camera->setFollowObject(player);
    for (const auto &ground_data : data["ground"])
    {
        std::vector<std::array<double, 2>> verts;
        verts.reserve(ground_data["points"].size());

        for (const auto &point : ground_data["points"])
            verts.emplace_back(std::array<double, 2>{point[0], point[1]});

        auto ptr = std::make_shared<Ground>(std::move(verts));
        drawable.emplace(ptr->getTag(), ptr);
        collidable.emplace(ptr->getTag(), ptr);
    }

    for (const auto &jump_pad_data : data["jump_pads"])
    {
        vec = {jump_pad_data["pos"][0], jump_pad_data["pos"][1]};
        auto ptr = std::make_shared<JumpPad>(vec, jump_pad_data["power"]);
        drawable.emplace(ptr->getTag(), ptr);
        updatable.emplace(ptr->getTag(), ptr);
        collidable.emplace(ptr->getTag(), ptr);
    }
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
    for (auto obj : updatable)
        GameObject::update(obj.second);
    GameObject::update(player);
}

void ObjectManager::collideAll()
{
    for (auto phys : physical)
    {
        for (auto col : collidable)
            GameObject::calculateCollision(phys.second, col.second);
        for (auto sPhys : physical)
            GameObject::calculateCollision(phys.second, sPhys.second);
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
    for (auto obj : drawable)
        GameObject::draw(obj.second, target);
    GameObject::draw(player, target);
}

sf::View ObjectManager::getCamera() const
{
    if (camera)
        return camera->getView();
    return defaultView;
}

// void ObjectManager::recursiveCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second)
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