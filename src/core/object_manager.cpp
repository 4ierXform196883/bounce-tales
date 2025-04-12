#include "object_manager.hpp"

#include <queue>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

#include "game.hpp"
#include "collision_calculator.hpp"
#include "curved_shape.hpp"
#include "earcut.hpp"
#include "jump_pad.hpp"
#include "platform.hpp"
#include "trigger_object.hpp"
#include "spikes.hpp"
#include "door.hpp"
#include "switch.hpp"
#include "wind.hpp"
#include "water.hpp"
#include "particle.hpp"

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
    this->loaded = true;
    const_cast<sf::View &>(defaultView) = sf::View({0, 0}, Game::getSettings().camera_size);

    // Opening json
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] (ObjectManager::load) Couldn't open file\n";
        return;
    }
    json data;
    file >> data;
    file.close();

    // Background
    auto &curData = data["background"];
    background = std::make_shared<Background>(curData["islands"], curData["clouds"], curData["additional_distance"]);
    curData = data["player"];
    player = std::make_shared<Player>(sf::Vector2f(curData["spawn_pos"][0], curData["spawn_pos"][1]), curData["control_force"]);
    loadObject(player, curData);
    drawable.push_back(player);
    updatable.push_back(player);
    collidable.push_back(player);
    physical.push_back(player);

    // Music
    Game::getSoundManager().setMusic(data["music"], Game::getSettings().getDouble("Volume", "music", 50));

    // Triggers
    for (const auto &part : data["triggers"])
    {
        std::shared_ptr<TriggerObject> ptr;
        std::function<void(std::shared_ptr<GameObject> other)> callback;
        if (part["type"] == "death_zone")
        {
            callback = [](std::shared_ptr<GameObject> other)
            {
                if (auto otherPtr = std::dynamic_pointer_cast<Player>(other))
                    otherPtr->onDeath();
            };
        }
        else if (part["type"] == "win_zone")
        {
            callback = [](std::shared_ptr<GameObject> other)
            {
                if (auto otherPtr = std::dynamic_pointer_cast<Player>(other))
                    otherPtr->onWin();
            };
        }

        else if (part["type"] == "player_set_skin")
        {
            Player::Skin type = part["skin"] == "heavy" ? Player::Skin::HEAVY : (part["skin"] == "light" ? Player::Skin::LIGHT : Player::Skin::NORMAL);
            callback = [type](std::shared_ptr<GameObject> other)
            {
                if (auto otherPtr = std::dynamic_pointer_cast<Player>(other))
                    otherPtr->setSkin(type);
            };
        }
        else if (part["type"] == "egg")
        {
            callback = [this, ptr](std::shared_ptr<GameObject> other)
            {
                Particle::spawnCircle("particles", "star", 2, ptr->getPosition(), 1, 10, 10);
                ptr->alive = false;
                Game::getStats().currentEggs += 1;
            };
            Game::getStats().currentTotalEggs += 1;
        }
        if (part.contains("size"))
            ptr = std::make_shared<TriggerObject>(sf::Vector2f(part["size"][0], part["size"][1]), callback);
        else if (part.contains("animation"))
            ptr = std::make_shared<TriggerObject>(part["texture"], part["animation"], part["fps"], callback);
        else if (part.contains("subtexture"))
            ptr = std::make_shared<TriggerObject>(part["texture"], part["subtexture"], callback);
        else if (part.contains("texture"))
            ptr = std::make_shared<TriggerObject>(part["texture"], callback);
        else
            continue;
        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
    }

    // Simple objects
    for (const auto &part : data["simple"])
    {
        std::shared_ptr<SimpleObject> ptr;
        if (part.contains("subtexture"))
            ptr = std::make_shared<SimpleObject>(part["tag"], part["texture"], part["subtexture"]);
        else if (part.contains("animation"))
            ptr = std::make_shared<SimpleObject>(part["tag"], part["texture"], part["animation"], part["fps"]);
        else
            ptr = std::make_shared<SimpleObject>(part["tag"], part["texture"]);
        loadObject(ptr, part);
        drawable.push_back(ptr);
    }

    // Platforms
    for (const auto &part : data["platforms"])
    {
        std::vector<sf::Vector2f> verts, path;
        verts.reserve(part["verts"].size());
        path.reserve(part["path"].size());

        for (const auto &point : part["verts"])
            verts.emplace_back(sf::Vector2f(point[0], point[1]));
        for (const auto &point : part["path"])
            path.emplace_back(sf::Vector2f(point[0], point[1]));

        auto ptr = std::make_shared<Platform>(part["texture"], std::move(verts), std::move(path), part["speed_mult"]);
        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
        physical.push_back(ptr);
    }

    // Jump pads
    for (const auto &part : data["jump_pads"])
    {
        auto ptr = std::make_shared<JumpPad>(part["power"]);
        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
    }

    // Spikes
    for (const auto &part : data["spikes"])
    {
        auto ptr = std::make_shared<Spikes>(part["count"]);
        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
    }

    // Doors
    for (const auto &part : data["doors"])
    {
        auto ptr = std::make_shared<Door>(part["tag"], sf::Vector2f(part["start_pos"][0], part["start_pos"][1]), part["elevation"]);
        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
    }

    // Switches
    for (const auto &part : data["switches"])
    {
        std::vector<std::shared_ptr<Door>> doors(part["doors"].size());
        for (const auto &doorName : part["doors"])
        {
            auto it = std::find_if(updatable.begin(), updatable.end(),
                                   [doorName](const std::shared_ptr<GameObject> &obj)
                                   { return obj->getTag() == doorName; });
            if (it == updatable.end())
                continue;
            if (auto doorPtr = std::dynamic_pointer_cast<Door>((*it)))
                doors.push_back(doorPtr);
        }
        auto ptr = std::make_shared<Switch>(doors);
        if (part.contains("is_on"))
            ptr->setState(part["is_on"]);
        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
    }

    // Wind
    for (const auto &part : data["wind"])
    {
        sf::Vector2f wSize = sf::Vector2f(part["size"][0], part["size"][1]);
        sf::Vector2f wDirection = sf::Vector2f(part["direction"][0], part["direction"][1]);
        auto ptr = std::make_shared<Wind>(wSize, wDirection);

        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
    }

    // Water
    for (const auto &part : data["water"])
    {
        auto ptr = std::make_shared<Water>(sf::Vector2f(part["size"][0], part["size"][1]));
        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
    }

    // Ground
    for (const auto &part : data["ground"])
    {
        std::vector<sf::Vector2f> verts;
        verts.reserve(part["verts"].size());

        for (const auto &point : part["verts"])
            verts.emplace_back(sf::Vector2f(point[0], point[1]));

        auto ptr = std::make_shared<Ground>(std::move(verts), part["texture"]);
        loadObject(ptr, part);
        drawable.push_back(ptr);
        updatable.push_back(ptr);
        collidable.push_back(ptr);
    }

    // Camera
    curData = data["camera"];
    camera = std::make_shared<Camera>(sf::Vector2f(curData["size"][0], curData["size"][1]));
    loadObject(camera, curData);
    if (!curData.contains("follow"))
        return;
    auto followIt = std::find_if(drawable.begin(), drawable.end(),
                                 [&curData](const std::shared_ptr<GameObject> &obj)
                                 { return obj->getTag() == static_cast<std::string>(curData["follow"]); });
    if (followIt != drawable.end())
        camera->setFollowObject(*followIt);
}

// IMPLEMENT
void ObjectManager::save(const std::string &path)
{
}

void ObjectManager::clear()
{
    loaded = false;
    background = nullptr;
    camera = nullptr;
    player = nullptr;
    drawable.clear();
    updatable.clear();
    collidable.clear();
    physical.clear();
}

void ObjectManager::updateAll()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        player->setPosition(Game::getMousePos());
        player->addForce(-player->getSpeed());
    }
    GameObject::update(camera);
    GameObject::update(background);
    for (auto it = updatable.begin(); it != updatable.end();)
    {
        if (!(*it)->alive)
        {
            it = updatable.erase(it);
            continue;
        }
        GameObject::update((*it));
        ++it;
    }
}

void ObjectManager::collideAll()
{
    for (auto it = physical.begin(); it != physical.end();)
    {
        if (!(*it)->alive)
        {
            it = physical.erase(it);
            continue;
        }
        for (auto itt = collidable.begin(); itt != collidable.end();)
        {
            if (!(*itt)->alive)
            {
                itt = collidable.erase(itt);
                continue;
            }
            GameObject::calculateCollision((*it), (*itt));
            ++itt;
        }
        ++it;
    }
}

void ObjectManager::drawAll(sf::RenderTarget &target)
{
    target.setView(defaultView);
    GameObject::draw(background, target);
    target.setView(this->getView());
    for (auto it = drawable.begin(); it != drawable.end();)
    {
        if (!(*it)->alive)
        {
            it = drawable.erase(it);
            continue;
        }
        GameObject::draw((*it), target);
        ++it;
    }
}

void ObjectManager::addObject(std::shared_ptr<GameObject> object)
{
    drawable.push_back(object);
    updatable.push_back(object);
    if (object->isCollidable())
        collidable.push_back(object);
    if (object->isPhysical())
        physical.push_back(object);
}

void ObjectManager::addObjects(const std::vector<std::shared_ptr<GameObject>> &objects)
{
    for (const auto &object : objects)
        this->addObject(object);
}

sf::View ObjectManager::getView() const
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