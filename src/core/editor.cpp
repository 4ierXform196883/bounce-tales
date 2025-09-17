#include "editor.hpp"

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

const nlohmann::json objectConfigTemplate = {
    {"pos", {0.0f, 0.0f}},
    {"scale", {1.0f, 1.0f}},
    {"origin", {0.0f, 0.0f}},
    {"rotation", 0.0f},
    {"mass", 1.0f},
    {"air_resistance", 0.1f}};

const nlohmann::json backgroundConfigTemplate = {
    {"islands", 60},
    {"clouds", 40},
    {"additional_distance", 4.0f}};

const nlohmann::json playerConfigTemplate = {
    {"spawn_pos", {0.0f, 0.0f}},
    {"control_force", 0.1f}};

const nlohmann::json triggerDeathConfigTemplate = {
    {"type", "death_zone"},
    {"size", {50.0f, 50.0f}}};

const nlohmann::json rockConfigTemplate = {
    {"mass", 2}};

const nlohmann::json triggerWin0ConfigTemplate = {
    {"type", "win_zone"},
    {"texture", "exit_0"}};

const nlohmann::json triggerWin1ConfigTemplate = {
    {"type", "win_zone"},
    {"texture", "exit_1"}};

const nlohmann::json triggerEggConfigTemplate = {
    {"type", "egg"},
    {"texture", "egg"}};

const nlohmann::json triggerSkinLightConfigTemplate = {
    {"type", "player_set_skin:light"},
    {"size", {50.0f, 50.0f}}};

const nlohmann::json triggerSkinNormalConfigTemplate = {
    {"type", "player_set_skin:normal"},
    {"size", {50.0f, 50.0f}}};

const nlohmann::json triggerSkinHeavyConfigTemplate = {
    {"type", "player_set_skin:heavy"},
    {"size", {50.0f, 50.0f}}};

const nlohmann::json simpleFlowerRedObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", ""},
    {"texture", "flower_red"}};

const nlohmann::json simpleFlowerBlueObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", ""},
    {"texture", "flower_blue"}};

const nlohmann::json simpleFlowerWhiteObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", ""},
    {"texture", "flower_white"}};

const nlohmann::json simpleBushObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", ""},
    {"texture", "bush"}};

const nlohmann::json simpleGrass1ObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", "1"},
    {"texture", "grass"}};

const nlohmann::json simpleGrass2ObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", "2"},
    {"texture", "grass"}};

const nlohmann::json simpleGrass3ObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", "3"},
    {"texture", "grass"}};

const nlohmann::json simpleGrass4ObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", "4"},
    {"texture", "grass"}};

const nlohmann::json simpleGrass5ObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", "5"},
    {"texture", "grass"}};

const nlohmann::json simpleGrass6ObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", "6"},
    {"texture", "grass"}};

const nlohmann::json simpleGrass7ObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", "7"},
    {"texture", "grass"}};

const nlohmann::json simpleGrass8ObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"fps", 1},
    {"subtexture", "8"},
    {"texture", "grass"}};

const nlohmann::json platformConfigTemplate = {
    {"verts", {{0.0f, 0.0f}, {50.0f, 0.0f}, {50.0f, 50.0f}, {50.0f, 0.0f}}},
    {"path", {{0.0f, 0.0f}, {100.0f, 100.0f}}},
    {"texture", "ground_1"},
    // {"color", "#00AA00"},
    {"speed_mult", 1.0f}};

const nlohmann::json jumpPadConfigTemplate = {
    {"power", 10.0f}};
  
const nlohmann::json enemyConfigTemplate = {
    {"walk_distance", 100.0f},
    {"speed", 1.0f},
    {"start_pos", {0.0f, 0.0f}}};

const nlohmann::json spikesConfigTemplate = {
    {"count", 5}};

const nlohmann::json doorConfigTemplate = {
    {"tag", ""},
    {"start_pos", {0.0f, 0.0f}},
    {"elevation", 50.0f}};

const nlohmann::json switchConfigTemplate = {
    {"doors", {}},
    {"is_on", false}};

const nlohmann::json windConfigTemplate = {
    {"size", {100.0f, 100.0f}},
    {"direction", {1.0f, 0.0f}}};

const nlohmann::json waterConfigTemplate = {
    {"size", {100.0f, 100.0f}}};

const nlohmann::json groundConfigTemplate = {
    {"verts", {{0.0f, 0.0f}, {50.0f, 0.0f}, {50.0f, 50.0f}, {0.0f, 50.0f}, {-50.0f, 50.0f}, {-50.0f, 0.0f}, {0.0f, 0.0f}}},
    {"color", "#426500FF"},
    // {"texture", "ground_0"},
    {"bezier_verts", true}};

const nlohmann::json cameraConfigTemplate = {
    {"size", {800.0f, 600.0f}},
    {"follow", "player"}};

void Editor::load(const std::string &path)
{
  selection.visualRect.setFillColor(sf::Color(0, 120, 215, 100));
  selection.visualRect.setOutlineColor(sf::Color(0, 120, 215));
  selection.visualRect.setOutlineThickness(1.0f);
  this->loaded = true;
  const_cast<sf::View &>(defaultView) = sf::View({0, 0}, Game::getSettings()->camera_size);

  // Opening JSON
  std::ifstream file(path);
  if (!file.is_open())
  {
    std::cerr << "[ERROR] (Editor::load) Couldn't open file: " << path << "\n";
    Game::loadLevel("menu");
    return;
  }

  nlohmann::json data;
  try
  {
    file >> data;
  }
  catch (const std::exception &e)
  {
    std::cerr << "[ERROR] (Editor::load) Failed to parse JSON: " << e.what() << "\n";
    Game::loadLevel("menu");
    return;
  }
  file.close();

  if (data.contains("display_name") && data["display_name"].is_string())
    levelName = data["display_name"].get<std::string>();
  levelPath = path;

  std::shared_ptr<GameObject> ptr;

  // Background
  ptr = createObjectOfType("background", data.contains("background") ? data["background"] : backgroundConfigTemplate);
  backgroundConfig = (data.contains("background") && ptr) ? data["background"] : backgroundConfigTemplate;
  if (!ptr)
    ptr = createObjectOfType("background", backgroundConfigTemplate);
  background = std::dynamic_pointer_cast<Background>(ptr);
  if (background)
    Game::getGuiManager()->setConfig("background", &backgroundConfig);

  // Player
  ptr = createObjectOfType("player", data.contains("player") ? data["player"] : playerConfigTemplate);
  configs.push_back({"player", (data.contains("player") && ptr) ? data["player"] : playerConfigTemplate});
  if (!ptr)
    ptr = createObjectOfType("player", playerConfigTemplate);
  player = std::dynamic_pointer_cast<Player>(ptr);
  if (player)
    drawable.push_back(player);

  // Music
  if (data.contains("music") && data["music"].is_string())
    Game::getSoundManager()->setMusic(data["music"], Game::getSettings()->getDouble("Volume", "music", 50));
  else
    Game::getSoundManager()->setMusic("level", Game::getSettings()->getDouble("Volume", "music", 50));

  auto loadArray = [&](const std::string &key, const std::string &type)
  {
    if (data.contains(key) && data[key].is_array())
    {
      for (const auto &part : data[key])
      {
        auto obj = createObjectOfType(type, part);
        if (!obj)
          continue;
        drawable.push_back(obj);
        configs.emplace_back(type, part);
      }
    }
  };

  loadArray("triggers", "trigger");
  loadArray("platforms", "platform");
  loadArray("jump_pads", "jump_pad");
  loadArray("spikes", "spikes");
  loadArray("doors", "door");
  loadArray("switches", "switch");
  loadArray("wind", "wind");
  loadArray("water", "water");
  loadArray("ground", "ground");
  loadArray("simple", "simple");
  loadArray("rocks", "rock");
  loadArray("enemies", "enemy");

  // Camera
  ptr = createObjectOfType("camera", data.contains("camera") ? data["camera"] : cameraConfigTemplate);
  cameraConfig = (data.contains("camera") && ptr) ? data["camera"] : cameraConfigTemplate;
  if (!ptr)
    ptr = createObjectOfType("camera", cameraConfigTemplate);
  camera = std::dynamic_pointer_cast<Camera>(ptr);
  if (camera)
  {
    camera->setFollowObject(nullptr);
    Game::getGuiManager()->setConfig("camera", &cameraConfig);
  }
}

void Editor::save(const std::string &path)
{
  nlohmann::json data;

  if (camera)
    data["camera"] = cameraConfig;

  if (background)
    data["background"] = backgroundConfig;

  data["music"] = Game::getSoundManager()->getMusicName();
  data["display_name"] = levelName;

  for (size_t i = 0; i < drawable.size(); ++i)
  {
    const std::string &type = configs.at(i).first;
    if (type == "player")
      data["player"] = configs.at(i).second;
    else if (type == "trigger")
      data["triggers"].push_back(configs.at(i).second);
    else if (type == "simple")
      data["simple"].push_back(configs.at(i).second);
    else if (type == "platform")
      data["platforms"].push_back(configs.at(i).second);
    else if (type == "jump_pad")
      data["jump_pads"].push_back(configs.at(i).second);
    else if (type == "spikes")
      data["spikes"].push_back(configs.at(i).second);
    else if (type == "door")
      data["doors"].push_back(configs.at(i).second);
    else if (type == "switch")
      data["switches"].push_back(configs.at(i).second);
    else if (type == "wind")
      data["wind"].push_back(configs.at(i).second);
    else if (type == "water")
      data["water"].push_back(configs.at(i).second);
    else if (type == "ground")
      data["ground"].push_back(configs.at(i).second);
    else if (type == "rock")
      data["rocks"].push_back(configs.at(i).second);
    else if (type == "enemy")
      data["enemies"].push_back(configs.at(i).second);
  }

  std::ofstream file(path);
  if (!file.is_open())
  {
    std::cerr << "[ERROR] (Editor::save) Couldn't open file for writing\n";
    return;
  }
  file << std::setw(4) << data << std::endl;
  file.close();
}

void Editor::recreateSelectedObject()
{
  if (!selection.selectedObject)
    return;

  auto &config = getConfigForObject(selection.selectedObject);
  std::string type = getObjectType(selection.selectedObject);

  auto it = std::find(drawable.begin(), drawable.end(), selection.selectedObject);
  if (it == drawable.end())
    return;

  int dIndex = std::distance(drawable.begin(), it);

  auto newObj = createObjectOfType(type, config);
  if (!newObj)
    return;

  selection.selectedObject = newObj;

  if (dIndex >= 0 && dIndex < static_cast<int>(drawable.size()))
    drawable.at(dIndex) = selection.selectedObject;
}

void Editor::updateAll()
{
  if (camera)
    GameObject::update(camera);

  if (background)
    GameObject::update(background);

  for (auto it = updatable.begin(); it != updatable.end();)
  {
    auto &obj = *it;
    if (!obj || !obj->alive)
    {
      it = updatable.erase(it);
      continue;
    }
    GameObject::update(obj);
    ++it;
  }
}

void Editor::drawAll(sf::RenderTarget &target)
{
  ObjectManager::drawAll(target);
  target.draw(selection.visualRect);

  if (selection.selectedGroundSpline)
    target.draw(*selection.selectedGroundSpline);

  if (selection.selectedGroundSpline && selection.selectedSplinePoint != -1)
  {
    int size = (selection.selectedSplinePoint % 3 == 0) ? 3 : 2;
    sf::CircleShape circle(size);
    circle.setOrigin(size, size);
    circle.setFillColor(sf::Color::Green);

    if (selection.selectedSplinePoint < selection.selectedGroundSpline->getPointCount())
    {
      sf::Vector2f pos = selection.selectedGroundSpline->getPoint(selection.selectedSplinePoint);
      pos += selection.selectedGroundSpline->getPosition();
      circle.setPosition(pos);
      target.draw(circle);
    }
  }

  if (newObject)
    GameObject::draw(newObject, target);
}

int Editor::getClickedPoint()
{
  if (!selection.selectedGroundSpline)
    return -1;
  auto mousePos = Game::getMousePos();
  for (size_t i = 0; i < selection.selectedGroundSpline->getPointCount(); ++i)
  {
    const auto &point = selection.selectedGroundSpline->getPoint(i) - selection.selectedObject->getOrigin() + selection.selectedObject->getPosition();
    if (sf::FloatRect(point - sf::Vector2f(5, 5), sf::Vector2f(10, 10)).contains(mousePos))
    {
      return i;
    }
  }
  return -1;
}

std::shared_ptr<GameObject> Editor::getClickedObject()
{
  auto mousePos = Game::getMousePos();
  for (auto it = drawable.rbegin(); it != drawable.rend(); ++it)
  {
    const auto &obj = *it;
    if (auto particle = std::dynamic_pointer_cast<Particle>(obj))
      continue;
    auto ptr = std::dynamic_pointer_cast<Ground>(obj);
    if (!ptr && obj->getGlobalBounds().contains(mousePos))
      return obj;
    else if (ptr)
    {
      const Hitbox &firstHitbox = CircleHitbox{.radius = 1, .center = mousePos};
      const Hitbox &secondHitbox = ptr->getHitbox();
      if (collide(firstHitbox, secondHitbox))
        return obj;
    }
  }
  return nullptr;
}

void Editor::handleEvent(const sf::Event &event)
{
  handleInput();
  switch (event.type)
  {
  case sf::Event::MouseWheelScrolled:
    if (event.mouseWheelScroll.wheel == sf::Mouse::Wheel::VerticalWheel)
      onMouseScroll(event.mouseWheelScroll.delta);
    break;
  case sf::Event::KeyPressed:
    handleKeyboard();
    break;
  default:
    break;
  }
}

void Editor::handleInput()
{
  if (newObject)
    newObject->setPosition(Game::getMousePos());
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !mouseState.prevLeftPressed)
    onLeftDown();
  else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseState.prevLeftPressed)
    onLeftDrag();
  else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mouseState.prevLeftPressed)
    onLeftUp();
  else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !mouseState.prevRightPressed)
    onRightDown();
  else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
    onMiddleDrag();
  mouseState.prevLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
  mouseState.prevRightPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
  mouseState.prevMiddlePos = Game::getLocalMousePos();
  mouseState.prevLeftPos = Game::getMousePos();
}

void Editor::onLeftDown()
{
  if (mode == Create)
  {
    if (!newObject)
      return;

    std::string realType;
    auto posSep = newObjectType.find(':');
    if (posSep == std::string::npos)
      realType = newObjectType;
    else
      realType = newObjectType.substr(0, posSep);

    if (realType == "player" && player)
      return;

    if (realType == "player" && !player)
    {
      if (auto p = std::dynamic_pointer_cast<Player>(newObject))
        player = p;
    }

    drawable.push_back(newObject);
    configs.emplace_back(realType, getTemplateConfigForObjectType(newObjectType));

    sf::Color oc = newObject->getColor();
    int newAlpha = std::min(255, static_cast<int>(oc.a) * 4);
    newObject->setColor(sf::Color(oc.r, oc.g, oc.b, static_cast<sf::Uint8>(newAlpha)));

    try
    {
      auto &config = getConfigForObject(newObject);
      if (!config.is_object())
        config = nlohmann::json::object();
      std::string key = (realType == "player" ? "spawn_pos" : (realType == "door" || realType == "enemy" ? "start_pos" : "pos"));
      config[key] = {newObject->getPosition().x, newObject->getPosition().y};
    }
    catch (...)
    {
    }

    newObject = createObjectOfType(realType, getTemplateConfigForObjectType(newObjectType));
    if (!newObject)
      return;

    oc = newObject->getColor();
    int smallAlpha = static_cast<int>(oc.a) / 4;
    newObject->setColor(sf::Color(oc.r, oc.g, oc.b, static_cast<sf::Uint8>(smallAlpha)));
    return;
  }

  mouseState.leftDownPos = Game::getMousePos();

  auto clickedObject = getClickedObject();
  auto clickedPoint = getClickedPoint();

  if (selection.selectedGroundSpline && clickedPoint != -1)
  {
    selection.selectedSplinePoint = clickedPoint;
    return;
  }

  if (selection.selectedGroundSpline && !selection.selectedGroundSpline->isFinished())
  {
    if (selection.selectedObject)
    {
      sf::Vector2f pos = Game::getMousePos() - selection.selectedObject->getPosition() + selection.selectedObject->getOrigin();
      selection.selectedGroundSpline->addControlPoint(pos);
      selection.selectedSplinePoint = static_cast<int>(selection.selectedGroundSpline->getPointCount()) - 2;
    }
    return;
  }

  if (!clickedObject)
  {
    selection.selectedObjectType = "";
    selection.selectedObject = nullptr;
    Game::getGuiManager()->setConfig("object", nullptr);
    selection.selectedObjects.clear();
    selection.visualRect.setPosition(mouseState.leftDownPos);
    if (selection.selectedGroundSpline)
      selection.selectedGroundSpline->setFinished(true);
    selection.selectedGroundSpline = nullptr;
    selection.selectedSplinePoint = -1;
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
  {
    if (selection.selectedGroundSpline)
      selection.selectedGroundSpline->setFinished(true);
    selection.selectedObject = nullptr;
    Game::getGuiManager()->setConfig("object", nullptr);
    selection.selectedGroundSpline = nullptr;
    selection.selectedSplinePoint = -1;
    auto it = std::find(selection.selectedObjects.begin(), selection.selectedObjects.end(), clickedObject);
    if (it != selection.selectedObjects.end())
      selection.selectedObjects.erase(it);
    else
      selection.selectedObjects.push_back(clickedObject);
    Game::getGuiManager()->setEditorInfo("Selected " + std::to_string(selection.selectedObjects.size()) + " objects");
    return;
  }

  if (std::find(selection.selectedObjects.begin(), selection.selectedObjects.end(), clickedObject) == selection.selectedObjects.end())
  {
    selection.selectedObjects.clear();
    selection.selectedObjects.push_back(clickedObject);
    selection.selectedObjectType = getObjectType(clickedObject);
    selection.selectedObject = clickedObject;
    if (selection.selectedGroundSpline)
      selection.selectedGroundSpline->setFinished(true);
    selection.selectedGroundSpline = nullptr;
    selection.selectedSplinePoint = -1;
    auto ptr = std::dynamic_pointer_cast<Ground>(clickedObject);
    try
    {
      auto &config = getConfigForObject(clickedObject);
      if (!config.is_object())
        config = nlohmann::json::object();
      Game::getGuiManager()->setConfig("object", &config);
      if (config.contains("type") && config["type"].is_string())
        Game::getGuiManager()->setEditorInfo("Selected " + selection.selectedObjectType + ":" + config["type"].get<std::string>());
      else
        Game::getGuiManager()->setEditorInfo("Selected " + selection.selectedObjectType);
      if (ptr && config.contains("bezier_verts") && config["bezier_verts"].is_boolean() && config["bezier_verts"].get<bool>() && config.contains("verts") && config["verts"].is_array())
      {
        std::vector<sf::Vector2f> verts;
        verts.reserve(config["verts"].size());
        for (const auto &point : config["verts"])
        {
          if (point.is_array() && point.size() >= 2 && point[0].is_number() && point[1].is_number())
            verts.emplace_back(sf::Vector2f(point[0].get<float>(), point[1].get<float>()));
        }
        selection.selectedGroundSpline = std::make_shared<Spline>(verts);
        selection.selectedGroundSpline->setPosition(clickedObject->getPosition());
      }
    }
    catch (...)
    {
      Game::getGuiManager()->setConfig("object", nullptr);
    }
    return;
  }

  if (auto ptr = std::dynamic_pointer_cast<Ground>(clickedObject); ptr && selection.selectedGroundSpline)
  {
    selection.selectedSplinePoint = -1;
  }
}

void Editor::onLeftDrag()
{
  if (mode == Create)
    return;

  auto mousePos = Game::getMousePos();
  auto delta = mousePos - mouseState.prevLeftPos;

  if (selection.selectedObjects.empty())
  {
    selection.visualRect.setSize(mousePos - mouseState.leftDownPos);
    return;
  }

  if (selection.selectedSplinePoint != -1)
  {
    if (selection.selectedGroundSpline)
    {
      int pointIndex = selection.selectedSplinePoint;
      if (pointIndex >= 0 && pointIndex < static_cast<int>(selection.selectedGroundSpline->getPointCount()))
      {
        selection.selectedGroundSpline->movePoint(pointIndex, delta);
      }
    }
    return;
  }

  if (selection.selectedObject)
  {
    if (auto ptr = std::dynamic_pointer_cast<Ground>(selection.selectedObject))
    {
      ptr->move(delta);
      if (selection.selectedGroundSpline)
        selection.selectedGroundSpline->move(delta);
      return;
    }
  }

  if (!selection.selectedObjects.empty())
  {
    for (auto &obj : selection.selectedObjects)
    {
      if (obj)
        obj->move(delta);
    }
  }
}

void Editor::onLeftUp()
{
  if (mode == Create)
    return;

  if (!selection.selectedObjects.empty())
  {
    if (selection.selectedObject)
    {
      if (auto ptr = std::dynamic_pointer_cast<Ground>(selection.selectedObject))
      {
        if (selection.selectedGroundSpline && selection.selectedGroundSpline->isFinished() && selection.selectedSplinePoint != -1)
        {
          updateGroundShape();
          return;
        }
      }
    }

    for (auto &obj : selection.selectedObjects)
    {
      if (!obj)
        continue;
      try
      {
        auto &config = getConfigForObject(obj);
        if (!config.is_object())
          config = nlohmann::json::object();
        std::string objType = getObjectType(obj);
        std::string key = (objType == "player" ? "spawn_pos" : (objType == "door" || objType == "enemy" ? "start_pos" : "pos"));
        config[key] = {obj->getPosition().x, obj->getPosition().y};
      }
      catch (...)
      {
      }
    }
    return;
  }

  auto mousePos = Game::getMousePos();
  selection.visualRect.setSize({0.f, 0.f});

  if (std::abs((mousePos - mouseState.leftDownPos).x) < 5.f && std::abs((mousePos - mouseState.leftDownPos).y) < 5.f)
    return;

  sf::FloatRect selectionRect(
      std::min(mouseState.leftDownPos.x, mousePos.x),
      std::min(mouseState.leftDownPos.y, mousePos.y),
      std::abs(mousePos.x - mouseState.leftDownPos.x),
      std::abs(mousePos.y - mouseState.leftDownPos.y));

  ConvexHitbox selectionHitbox{.points = {sf::Vector2f(selectionRect.left, selectionRect.top),
                                          sf::Vector2f(selectionRect.left + selectionRect.width, selectionRect.top),
                                          sf::Vector2f(selectionRect.left + selectionRect.width, selectionRect.top + selectionRect.height),
                                          sf::Vector2f(selectionRect.left, selectionRect.top + selectionRect.height)}};
  selection.selectedObjects.clear();
  for (const auto &obj : drawable)
  {
    if (!obj)
      continue;
    if (auto ptr = std::dynamic_pointer_cast<Ground>(obj); !ptr)
    {
      try
      {
        if (selectionRect.intersects(obj->getGlobalBounds()))
          selection.selectedObjects.push_back(obj);
      }
      catch (...)
      {
      }
    }
    else
    {
      try
      {
        const Hitbox &secondHitbox = ptr->getHitbox();
        if (collide(selectionHitbox, secondHitbox))
          selection.selectedObjects.push_back(obj);
      }
      catch (...)
      {
      }
    }
  }
  Game::getGuiManager()->setEditorInfo("Selected " + std::to_string(selection.selectedObjects.size()) + " objects");
}

void Editor::onRightDown()
{
  auto clickedPoint = getClickedPoint();
  if (clickedPoint != -1 && selection.selectedGroundSpline)
  {
    int removedPoints = 0;
    try
    {
      removedPoints = selection.selectedGroundSpline->removeControlPoint(clickedPoint);
    }
    catch (...)
    {
      return;
    }

    if (selection.selectedGroundSpline->isFinished())
      updateGroundShape();

    if (std::abs(selection.selectedSplinePoint - clickedPoint) <= 1)
      selection.selectedSplinePoint = -1;
    else if (!selection.selectedGroundSpline->isFinished() && clickedPoint == 0 && selection.selectedSplinePoint <= 2)
      selection.selectedSplinePoint = -1;
    else if (selection.selectedSplinePoint > clickedPoint)
      selection.selectedSplinePoint = std::max(-1, selection.selectedSplinePoint - removedPoints);
  }
}

void Editor::onMiddleDrag()
{
  auto localMouse = Game::getLocalMousePos();
  sf::Vector2f delta = localMouse - mouseState.prevMiddlePos;
  if (camera)
    camera->move(-delta * camera->getZoom());
}

void Editor::onMouseScroll(float delta)
{
  auto &objectManager = Game::getObjectManager();
  if (!objectManager)
    return;

  auto cam = objectManager->getCamera();
  if (!cam)
    return;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
  {
    float factor = cam->getZoom() * (delta > 0 ? 0.9f : 1.1f);
    factor = std::clamp(factor, 0.1f, 10.0f);
    cam->setZoom(factor);
    return;
  }

  if (mode == Create)
  {
    size_t n = static_cast<size_t>(std::distance(std::begin(newObjectTypes), std::end(newObjectTypes)));
    auto it = std::find(std::begin(newObjectTypes), std::end(newObjectTypes), newObjectType);
    size_t idx = (it == std::end(newObjectTypes)) ? 0 : static_cast<size_t>(std::distance(std::begin(newObjectTypes), it));
    if (delta > 0)
      idx = (idx + 1) % n;
    else
      idx = (idx + n - 1) % n;
    newObjectType = newObjectTypes[idx];

    std::string realType;
    auto posSep = newObjectType.find(':');
    if (posSep == std::string::npos)
      realType = newObjectType;
    else
      realType = newObjectType.substr(0, posSep);

    auto newObj = createObjectOfType(realType, getTemplateConfigForObjectType(newObjectType));
    if (!newObj)
      return;

    newObject = newObj;
    Game::getGuiManager()->setEditorInfo("New object type:    " + newObjectType);
    sf::Color oc = newObject->getColor();
    newObject->setColor(sf::Color(oc.r, oc.g, oc.b, static_cast<sf::Uint8>(oc.a / 4)));
    newObject->setPosition(Game::getMousePos());
  }
}

void Editor::handleKeyboard()
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
  {
    mode = Edit;
    newObject = nullptr;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
  {
    if (selection.selectedObject || selection.selectedGroundSpline)
      return;
    mode = Create;
    std::string realType;
    auto posSep = newObjectType.find(':');
    if (posSep == std::string::npos)
      realType = newObjectType;
    else
      realType = newObjectType.substr(0, posSep);

    auto newObj = createObjectOfType(realType, getTemplateConfigForObjectType(newObjectType));
    if (!newObj)
      return;
    newObject = newObj;
    sf::Color oc = newObject->getColor();
    newObject->setColor(sf::Color(oc.r, oc.g, oc.b, static_cast<sf::Uint8>(oc.a / 4)));
    newObject->setPosition(Game::getMousePos());
    Game::getGuiManager()->setEditorInfo("New object type:    " + newObjectType);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && !selection.selectedObjects.empty())
  {
    for (auto &obj : selection.selectedObjects)
    {
      if (!obj)
        continue;
      auto it = std::find(drawable.begin(), drawable.end(), obj);
      if (it != drawable.end())
      {
        if ((*it) == player)
          player = nullptr;
        size_t index = static_cast<size_t>(std::distance(drawable.begin(), it));
        drawable.erase(it);
        if (index < configs.size())
          configs.erase(configs.begin() + static_cast<ptrdiff_t>(index));
      }
    }
    Game::getGuiManager()->setEditorInfo("Deleted " + std::to_string(selection.selectedObjects.size()) + " objects");
    selection.selectedObjects.clear();
    selection.selectedObject = nullptr;
    Game::getGuiManager()->setConfig("object", nullptr);
    selection.selectedObjectType = "";
    selection.selectedGroundSpline = nullptr;
    selection.selectedSplinePoint = -1;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
           (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) &&
           !selection.selectedObjects.empty())
  {
    if (selection.selectedGroundSpline && selection.selectedSplinePoint != -1)
    {
      sf::Vector2f pos = Game::getMousePos() - selection.selectedGroundSpline->getPosition();
      pos += selection.selectedGroundSpline->getOrigin();
      selection.selectedGroundSpline->insertControlPoint(selection.selectedSplinePoint, pos);
      updateGroundShape();
      return;
    }
    std::vector<std::shared_ptr<GameObject>> newSelectedObjects;
    for (auto &obj : selection.selectedObjects)
    {
      if (!obj)
        continue;
      try
      {
        auto &config = getConfigForObject(obj);
        std::string objType = getObjectType(obj);
        if (objType == "player")
          continue;
        std::string key = (objType == "player" ? "spawn_pos" : (objType == "door" || objType == "enemy" ? "start_pos" : "pos"));
        auto newObj = createObjectOfType(objType, config);
        auto newConfig = config;
        newConfig[key] = {newObj->getPosition().x + 20, newObj->getPosition().y + 20};
        if (newObj)
        {
          drawable.push_back(newObj);
          configs.push_back({objType, newConfig});
          newSelectedObjects.push_back(newObj);
        }
      }
      catch (...)
      {
      }
    }
    Game::getGuiManager()->setEditorInfo("Duplicated " + std::to_string(selection.selectedObjects.size()) + " objects");
    selection.selectedObjects = newSelectedObjects;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
           (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)))
  {
    this->save(levelPath);
    Game::getGuiManager()->setEditorInfo("Level saved to \"" + levelPath + "\"");
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && selection.selectedGroundSpline)
  {
    bool newFinished = !selection.selectedGroundSpline->isFinished();
    selection.selectedGroundSpline->setFinished(newFinished);
    if (newFinished)
      updateGroundShape();
    else
      selection.selectedSplinePoint = static_cast<int>(selection.selectedGroundSpline->getPointCount()) - 2;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
  {
    Game::getGuiManager()->toggleEditorInstructionsVisibility();
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && !selection.selectedObjects.empty())
  {
    std::vector<size_t> indices;
    for (auto &obj : selection.selectedObjects)
    {
      if (!obj)
        continue;
      auto it = std::find(drawable.begin(), drawable.end(), obj);
      if (it != drawable.end())
        indices.push_back(static_cast<size_t>(std::distance(drawable.begin(), it)));
    }
    std::sort(indices.rbegin(), indices.rend());
    std::vector<std::shared_ptr<GameObject>> movedObjects;
    std::vector<std::pair<std::string, nlohmann::json>> movedConfigs;
    for (auto i : indices)
    {
      if (i < drawable.size())
      {
        movedObjects.push_back(drawable[i]);
        movedConfigs.push_back(configs[i]);
        drawable.erase(drawable.begin() + static_cast<ptrdiff_t>(i));
        configs.erase(configs.begin() + static_cast<ptrdiff_t>(i));
      }
    }
    if (!drawable.empty())
    {
      drawable.insert(drawable.begin() + static_cast<ptrdiff_t>(drawable.size() - 1), movedObjects.begin(), movedObjects.end());
      configs.insert(configs.begin() + static_cast<ptrdiff_t>(configs.size() - 1), movedConfigs.begin(), movedConfigs.end());
    }
    else
    {
      drawable.insert(drawable.end(), movedObjects.begin(), movedObjects.end());
      configs.insert(configs.end(), movedConfigs.begin(), movedConfigs.end());
    }
    Game::getGuiManager()->setEditorInfo("Moved " + std::to_string(movedObjects.size()) + " objects to the front");
  }
}

bool Editor::collide(const Hitbox &firstHitbox, const Hitbox &secondHitbox)
{
  if (firstHitbox.index() == 2)
    return false;
  switch (secondHitbox.index())
  {
  case 0:
  case 1:
  {
    return collision_calculator::getGJKCollisionSimplex(firstHitbox, secondHitbox).size() != 0;
  }
  case 2:
  {
    auto &triangles = std::get<ConcaveHitbox>(secondHitbox).triangles;
    for (auto triangle : triangles)
    {
      auto simplex = collision_calculator::getGJKCollisionSimplex(firstHitbox, triangle);
      if (simplex.size() != 0)
        return true;
    }
  }
  default:
    return false;
  }
  return false;
}

nlohmann::json &Editor::getConfigForObject(const std::shared_ptr<GameObject> &object)
{
  auto it = std::find(drawable.begin(), drawable.end(), object);
  if (it == drawable.end())
  {
    throw std::runtime_error("Object not found in drawable");
  }
  size_t index = std::distance(drawable.begin(), it);
  return configs.at(index).second;
}

const std::string &Editor::getObjectType(const std::shared_ptr<GameObject> &object)
{
  auto it = std::find(drawable.begin(), drawable.end(), object);
  if (it == drawable.end())
  {
    throw std::runtime_error("Object not found in drawable");
  }
  size_t index = std::distance(drawable.begin(), it);
  return configs.at(index).first;
}

const nlohmann::json &Editor::getTemplateConfigForObjectType(const std::string &type)
{
  if (type == "player")
    return playerConfigTemplate;
  else if (type == "trigger:win_zone0")
    return triggerWin0ConfigTemplate;
  else if (type == "trigger:win_zone1")
    return triggerWin1ConfigTemplate;
  else if (type == "trigger:death_zone")
    return triggerDeathConfigTemplate;
  else if (type == "trigger:egg")
    return triggerEggConfigTemplate;
  else if (type == "trigger:player_set_skin:light")
    return triggerSkinLightConfigTemplate;
  else if (type == "trigger:player_set_skin:normal")
    return triggerSkinNormalConfigTemplate;
  else if (type == "trigger:player_set_skin:heavy")
    return triggerSkinHeavyConfigTemplate;
  else if (type == "trigger")
    return triggerDeathConfigTemplate;
  else if (type == "simple")
    return simpleBushObjectConfigTemplate;
  else if (type == "simple:flower_red")
    return simpleFlowerRedObjectConfigTemplate;
  else if (type == "simple:flower_blue")
    return simpleFlowerBlueObjectConfigTemplate;
  else if (type == "simple:flower_white")
    return simpleFlowerWhiteObjectConfigTemplate;
  else if (type == "simple:grass1")
    return simpleGrass1ObjectConfigTemplate;
  else if (type == "simple:grass2")
    return simpleGrass2ObjectConfigTemplate;
  else if (type == "simple:grass3")
    return simpleGrass3ObjectConfigTemplate;
  else if (type == "simple:grass4")
    return simpleGrass4ObjectConfigTemplate;
  else if (type == "simple:grass5")
    return simpleGrass5ObjectConfigTemplate;
  else if (type == "simple:grass6")
    return simpleGrass6ObjectConfigTemplate;
  else if (type == "simple:grass7")
    return simpleGrass7ObjectConfigTemplate;
  else if (type == "simple:grass8")
    return simpleGrass8ObjectConfigTemplate;
  else if (type == "simple:bush")
    return simpleBushObjectConfigTemplate;
  else if (type == "platform")
    return platformConfigTemplate;
  else if (type == "jump_pad")
    return jumpPadConfigTemplate;
  else if (type == "spikes")
    return spikesConfigTemplate;
  else if (type == "enemy")
    return enemyConfigTemplate;
  else if (type == "door")
    return doorConfigTemplate;
  else if (type == "switch")
    return switchConfigTemplate;
  else if (type == "wind")
    return windConfigTemplate;
  else if (type == "water")
    return waterConfigTemplate;
  else if (type == "ground")
    return groundConfigTemplate;
  else if (type == "camera")
    return cameraConfigTemplate;
  else if (type == "background")
    return backgroundConfigTemplate;
  else if (type == "rock")
    return rockConfigTemplate;
  return simpleBushObjectConfigTemplate;
}

void Editor::updateGroundShape()
{
  if (!selection.selectedGroundSpline || !selection.selectedObject)
  {
    std::cout << "no spline or object\n";
    return;
  }
  bool actualFinished = selection.selectedGroundSpline->isFinished();
  if (!actualFinished)
    selection.selectedGroundSpline->setFinished(true);
  auto &config = getConfigForObject(selection.selectedObject);
  config["verts"].clear();
  for (size_t i = 0; i < selection.selectedGroundSpline->getPointCount(); ++i)
  {
    const auto &point = selection.selectedGroundSpline->getPoint(i);
    config["verts"].push_back({point.x, point.y});
  }
  selection.selectedGroundSpline->setFinished(actualFinished);
  int dIndex = std::distance(drawable.begin(), std::find(drawable.begin(), drawable.end(), selection.selectedObject));
  int sIndex = std::distance(selection.selectedObjects.begin(), std::find(selection.selectedObjects.begin(), selection.selectedObjects.end(), selection.selectedObject));
  selection.selectedObject = createObjectOfType("ground", config);
  drawable.at(dIndex) = selection.selectedObject;
  selection.selectedObjects.at(sIndex) = selection.selectedObject;
}
