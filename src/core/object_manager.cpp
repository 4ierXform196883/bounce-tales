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

std::shared_ptr<GameObject> ObjectManager::createObjectOfType(const std::string &type, const nlohmann::json &config)
{
  if (type == "background")
  {
    if (!config.contains("islands") || !config.contains("clouds") || !config.contains("additional_distance"))
      return nullptr;
    return std::make_shared<Background>(config["islands"], config["clouds"], config["additional_distance"]);
  }
  else if (type == "player")
  {
    if (!config.contains("spawn_pos") || !config.contains("control_force"))
      return nullptr;
    auto ptr = std::make_shared<Player>(sf::Vector2f(config["spawn_pos"][0], config["spawn_pos"][1]), config["control_force"]);
    loadObject(ptr, config);
    return ptr;
  }
  else if (type == "trigger")
  {
    if (!config.contains("type"))
      return nullptr;
    std::shared_ptr<TriggerObject> ptr;
    if (config.contains("size"))
      ptr = std::make_shared<TriggerObject>(sf::Vector2f(config["size"][0], config["size"][1]), config["type"]);
    else if (config.contains("animation") && config["animation"] != "" && config.contains("texture") && config["texture"] != "")
      ptr = std::make_shared<TriggerObject>(config["texture"], config["animation"], config["fps"], config["type"]);
    else if (config.contains("subtexture") && config["subtexture"] != "" && config.contains("texture") && config["texture"] != "")
      ptr = std::make_shared<TriggerObject>(config["texture"], config["subtexture"], config["type"]);
    else if (config.contains("texture") && config["texture"] != "")
      ptr = std::make_shared<TriggerObject>(config["texture"], config["type"]);
    else
      return nullptr;
    loadObject(ptr, config);
    return ptr;
  }
  else if (type == "simple")
  {
    if (!config.contains("tag") || !config.contains("texture"))
      return nullptr;
    std::shared_ptr<SimpleObject> ptr;
    if (config.contains("subtexture") && config["subtexture"] != "" && config.contains("texture") && config["texture"] != "")
      ptr = std::make_shared<SimpleObject>(config["tag"], config["texture"], config["subtexture"]);
    else if (config.contains("animation") && config["animation"] != "" && config.contains("texture") && config["texture"] != "")
      ptr = std::make_shared<SimpleObject>(config["tag"], config["texture"], config["animation"], config["fps"]);
    else if (config.contains("texture") && config["texture"] != "")
      ptr = std::make_shared<SimpleObject>(config["tag"], config["texture"]);
    else
      return nullptr;
    loadObject(ptr, config);
    return ptr;
  }
  else if (type == "platform")
  {
    if (!config.contains("texture") || config["texture"] == "" || !config.contains("verts") || !config.contains("path") || !config.contains("speed_mult"))
      return nullptr;
    std::vector<sf::Vector2f> verts, path;
    verts.reserve(config["verts"].size());
    path.reserve(config["path"].size());

    for (const auto &point : config["verts"])
      verts.emplace_back(sf::Vector2f(point[0], point[1]));
    for (const auto &point : config["path"])
      path.emplace_back(sf::Vector2f(point[0], point[1]));

    auto ptr = std::make_shared<Platform>(config["texture"], std::move(verts), std::move(path), config["speed_mult"]);
    loadObject(ptr, config);
    return ptr;
  }

  // Jump pads
  else if (type == "jump_pad")
  {
    if (!config.contains("power"))
      return nullptr;
    auto ptr = std::make_shared<JumpPad>(config["power"]);
    loadObject(ptr, config);
    return ptr;
  }
  // Spikes
  else if (type == "spikes")
  {
    if (!config.contains("count"))
      return nullptr;
    auto ptr = std::make_shared<Spikes>(config["count"]);
    loadObject(ptr, config);
    return ptr;
  }
  if (type == "door")
  {
    if (!config.contains("tag") || !config.contains("start_pos") || !config.contains("elevation"))
      return nullptr;
    auto ptr = std::make_shared<Door>(config["tag"], sf::Vector2f(config["start_pos"][0], config["start_pos"][1]), config["elevation"]);
    loadObject(ptr, config);
    return ptr;
  }
  else if (type == "switch")
  {
    if (!config.contains("doors"))
      return nullptr;
    std::vector<std::shared_ptr<Door>> doors(config["doors"].size());
    for (const auto &doorName : config["doors"])
    {
      auto it = std::find_if(drawable.begin(), drawable.end(),
          [doorName](const std::shared_ptr<GameObject> &obj)
          { return obj->getTag() == doorName; });
      if (it == drawable.end())
        continue;
      if (auto doorPtr = std::dynamic_pointer_cast<Door>((*it)))
        doors.push_back(doorPtr);
    }
    auto ptr = std::make_shared<Switch>(doors);
    if (config.contains("is_on"))
      ptr->setState(config["is_on"]);
    loadObject(ptr, config);
    return ptr;
  }
  else if (type == "wind")
  {
    if (!config.contains("size") || !config.contains("direction"))
      return nullptr;
    sf::Vector2f wSize = sf::Vector2f(config["size"][0], config["size"][1]);
    sf::Vector2f wDirection = sf::Vector2f(config["direction"][0], config["direction"][1]);
    auto ptr = std::make_shared<Wind>(wSize, wDirection);

    loadObject(ptr, config);
    return ptr;
  }
  else if (type == "water")
  {
    if (!config.contains("size"))
      return nullptr;
    auto ptr = std::make_shared<Water>(sf::Vector2f(config["size"][0], config["size"][1]));
    loadObject(ptr, config);
    return ptr;
  }
  else if (type == "ground")
  {
    if (!config.contains("verts") || !config.contains("texture"))
      return nullptr;
    std::vector<sf::Vector2f> verts;
    verts.reserve(config["verts"].size());

    for (const auto &point : config["verts"])
      verts.emplace_back(sf::Vector2f(point[0], point[1]));

    auto ptr = std::make_shared<Ground>(std::move(verts), config["texture"], config.contains("bezier_verts") ? (bool)config["bezier_verts"] : false);
    loadObject(ptr, config);
    return ptr;
  }
  else if (type == "camera")
  {
    if (!config.contains("size"))
      return nullptr;
    auto ptr = std::make_shared<Camera>(sf::Vector2f(config["size"][0], config["size"][1]));
    loadObject(ptr, config);
    if (config.contains("follow"))
    {
      auto followIt = std::find_if(drawable.begin(), drawable.end(),
          [&config](const std::shared_ptr<GameObject> &obj)
          { return obj->getTag() == static_cast<std::string>(config["follow"]); });
      if (followIt != drawable.end())
        ptr->setFollowObject(*followIt);
    }
    return ptr;
  }
  return nullptr;
}

void ObjectManager::load(const std::string &path)
{
  this->loaded = true;
  const_cast<sf::View &>(defaultView) = sf::View({0, 0}, Game::getSettings()->camera_size);

  // Opening json
  std::ifstream file(path);
  if (!file.is_open())
  {
    std::cerr << "[ERROR] (ObjectManager::load) Couldn't open file\n";
    Game::loadLevel("menu");
    return;
  }
  json data;
  file >> data;
  file.close();

  // Background
  if (!data.contains("background"))
  {
    std::cerr << "[ERROR] (ObjectManager::load) No background data found\n";
    Game::loadLevel("menu");
    return;
  }
  std::shared_ptr<GameObject> ptr;
  ptr = createObjectOfType("background", data["background"]);
  if (!ptr)
  {
    std::cerr << "[ERROR] (ObjectManager::load) Couldn't create background\n";
    Game::loadLevel("menu");
    return;
  }
  background = std::dynamic_pointer_cast<Background>(ptr);

  // Player
  if (!data.contains("player"))
  {
    std::cerr << "[ERROR] (ObjectManager::load) No player data found\n";
    Game::loadLevel("menu");
    return;
  }
  ptr = createObjectOfType("player", data["player"]);
  if (!ptr)
  {
    std::cerr << "[ERROR] (ObjectManager::load) Couldn't create player\n";
    Game::loadLevel("menu");
    return;
  }
  player = std::dynamic_pointer_cast<Player>(ptr);
  drawable.push_back(player);
  updatable.push_back(player);
  collidable.push_back(player);
  physical.push_back(player);

  // Music
  if (data.contains("music"))
    Game::getSoundManager()->setMusic(data["music"], Game::getSettings()->getDouble("Volume", "music", 50));
  else
    Game::getSoundManager()->setMusic("menu", Game::getSettings()->getDouble("Volume", "music", 50));

  // Triggers
  for (const auto &part : data["triggers"])
  {
    ptr = createObjectOfType("trigger", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
  }

  // Simple objects
  for (const auto &part : data["simple"])
  {
    ptr = createObjectOfType("simple", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
  }

  // Platforms
  for (const auto &part : data["platforms"])
  {
    ptr = createObjectOfType("platform", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
    physical.push_back(ptr);
  }

  // Jump pads
  for (const auto &part : data["jump_pads"])
  {
    ptr = createObjectOfType("jump_pad", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
  }

  // Spikes
  for (const auto &part : data["spikes"])
  {
    ptr = createObjectOfType("spikes", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
  }

  // Doors
  for (const auto &part : data["doors"])
  {
    ptr = createObjectOfType("door", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
  }

  // Switches
  for (const auto &part : data["switches"])
  {
    ptr = createObjectOfType("switch", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
  }

  // Wind
  for (const auto &part : data["wind"])
  {
    ptr = createObjectOfType("wind", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
  }

  // Water
  for (const auto &part : data["water"])
  {
    ptr = createObjectOfType("water", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
  }

  // Ground
  for (const auto &part : data["ground"])
  {
    ptr = createObjectOfType("ground", part);
    if (!ptr)
      continue;
    drawable.push_back(ptr);
    updatable.push_back(ptr);
    collidable.push_back(ptr);
  }

  // Camera
  if (!data.contains("camera"))
  {
    std::cerr << "[ERROR] (ObjectManager::load) No camera data found\n";
    Game::loadLevel("menu");
    return;
  }
  ptr = createObjectOfType("camera", data["camera"]);
  if (!ptr)
  {
    std::cerr << "[ERROR] (ObjectManager::load) Couldn't create camera\n";
    Game::loadLevel("menu");
    return;
  }
  camera = std::dynamic_pointer_cast<Camera>(ptr);
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
  for (auto obj1 = physical.begin(); obj1 != physical.end();)
  {
    if (!(*obj1)->alive)
    {
      obj1 = physical.erase(obj1);
      continue;
    }
    for (auto obj2 = collidable.begin(); obj2 != collidable.end();)
    {
      if (!(*obj2)->alive)
      {
        obj2 = collidable.erase(obj2);
        continue;
      }
      GameObject::calculateCollision((*obj1), (*obj2));
      ++obj2;
    }
    ++obj1;
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
