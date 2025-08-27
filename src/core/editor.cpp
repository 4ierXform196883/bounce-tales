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

const nlohmann::json triggerWinConfigTemplate = {
    {"type", "win_zone"},
    {"size", {50.0f, 50.0f}},
    {"texture", "exit_0"}};

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

const nlohmann::json simpleObjectConfigTemplate = {
    {"tag", ""},
    {"animation", ""},
    {"subtexture", ""},
    {"texture", "cloud_0"}};

const nlohmann::json platformConfigTemplate = {
    {"verts", {{0.0f, 0.0f}, {50.0f, 0.0f}, {50.0f, 50.0f}, {50.0f, 0.0f}}},
    {"path", {{0.0f, 0.0f}, {100.0f, 100.0f}}},
    {"texture", "ground_1"},
    {"speed_mult", 1.0f}};

const nlohmann::json jumpPadConfigTemplate = {
    {"power", 10.0f}};

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
    {"bezierVerts", true},
    {"texture", "ground_0"}};

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

    // Opening json
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] (Editor::load) Couldn't open file\n";
        Game::loadLevel("menu");
        return;
    }
    nlohmann::json data;
    file >> data;
    file.close();

    if (data.contains("display_name"))
        levelName = data["display_name"];
    levelPath = path;

    std::shared_ptr<GameObject> ptr;

    // Background
    ptr = createObjectOfType("background", data.contains("background") ? data["background"] : backgroundConfigTemplate);
    backgroundConfig = data.contains("background") && ptr ? data["background"] : backgroundConfigTemplate;
    background = std::dynamic_pointer_cast<Background>(ptr ? ptr : createObjectOfType("background", backgroundConfigTemplate));

    // Player
    ptr = createObjectOfType("player", data.contains("player") ? data["player"] : playerConfigTemplate);
    configs.push_back({"player", data.contains("player") && ptr ? data["player"] : playerConfigTemplate});
    player = std::dynamic_pointer_cast<Player>(ptr ? ptr : createObjectOfType("player", playerConfigTemplate));
    drawable.push_back(player);

    // Music
    if (data.contains("music"))
        Game::getSoundManager()->setMusic(data["music"], Game::getSettings()->getDouble("Volume", "music", 50));
    else
        Game::getSoundManager()->setMusic("level", Game::getSettings()->getDouble("Volume", "music", 50));

    // Triggers
    for (const auto &part : data["triggers"])
    {
        ptr = createObjectOfType("trigger", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("trigger", part);
    }

    // Simple objects
    for (const auto &part : data["simple"])
    {
        ptr = createObjectOfType("simple", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("simple", part);
    }

    // Platforms
    for (const auto &part : data["platforms"])
    {
        ptr = createObjectOfType("platform", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("platform", part);
    }

    // Jump pads
    for (const auto &part : data["jump_pads"])
    {
        ptr = createObjectOfType("jump_pad", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("jump_pad", part);
    }

    // Spikes
    for (const auto &part : data["spikes"])
    {
        ptr = createObjectOfType("spikes", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("spikes", part);
    }

    // Doors
    for (const auto &part : data["doors"])
    {
        ptr = createObjectOfType("door", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("door", part);
    }

    // Switches
    for (const auto &part : data["switches"])
    {
        ptr = createObjectOfType("switch", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("switch", part);
    }

    // Wind
    for (const auto &part : data["wind"])
    {
        ptr = createObjectOfType("wind", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("wind", part);
    }

    // Water
    for (const auto &part : data["water"])
    {
        ptr = createObjectOfType("water", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("water", part);
    }

    // Ground
    for (const auto &part : data["ground"])
    {
        ptr = createObjectOfType("ground", part);
        if (!ptr)
            continue;
        drawable.push_back(ptr);
        configs.emplace_back("ground", part);
    }

    // Camera
    ptr = createObjectOfType("camera", data.contains("camera") ? data["camera"] : cameraConfigTemplate);
    cameraConfig = data.contains("camera") && ptr ? data["camera"] : cameraConfigTemplate;
    camera = std::dynamic_pointer_cast<Camera>(ptr ? ptr : createObjectOfType("camera", cameraConfigTemplate));
    camera->setFollowObject(nullptr);
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

void Editor::updateAll()
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
    handleInput();
}

void Editor::drawAll(sf::RenderTarget &target)
{
    ObjectManager::drawAll(target);
    target.draw(selection.visualRect);
    if (selection.selectedGroundSpline)
        target.draw(*selection.selectedGroundSpline);
    if (selection.selectedSplinePoint != -1 && selection.selectedGroundSpline)
    {
        int size = selection.selectedSplinePoint % 3 == 0 ? 3 : 2;
        sf::CircleShape circle(size);
        circle.setOrigin(size, size);
        circle.setFillColor(sf::Color::Green);
        sf::Vector2f pos = selection.selectedGroundSpline->getPoint(selection.selectedSplinePoint);
        pos += selection.selectedGroundSpline->getPosition();
        circle.setPosition(pos);
        target.draw(circle);
    }
    if (newObject)
        GameObject::draw(newObject, target);
}

// is not a GameObject
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
    for (const auto &obj : drawable)
    {
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
    // std::cout << "objs: " << selection.selectedObjects.size();
    // std::cout << "; sel size: " << selection.visualRect.getSize().x << " " << selection.visualRect.getSize().y;
    // std::cout << "; sel type: " << selection.selectedObjectType;
    // std::cout << "; sel object: " << selection.selectedObject;
    // std::cout << "; sel spline: " << selection.selectedGroundSpline;
    // std::cout << "; sel point: " << selection.selectedSplinePoint << "\n";
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
    // std::cout << "Selected objects: " << selection.selectedObjects.size() << "\n";
}

void Editor::onLeftDown()
{
    if (mode == Create)
    {
        if (!newObject)
            return;
        
        std::string realType = (newObjectType.find("trigger:") != std::string::npos) ? "trigger" : newObjectType;
        if (realType == "player" && player)
            return;
        else if (!player)
            player = std::dynamic_pointer_cast<Player>(newObject);
        drawable.push_back(newObject);
        configs.emplace_back(realType, getTemplateConfigForObjectType(newObjectType));
        sf::Color oc = newObject->getColor();
        newObject->setColor(sf::Color(oc.r, oc.g, oc.b, oc.a * 4));
        auto& config = getConfigForObject(newObject);
        std::string key = (realType == "player" ? "spawn_pos" : (realType == "door" ? "start_pos" : "pos"));
        config[key] = {newObject->getPosition().x, newObject->getPosition().y};
        newObject = createObjectOfType(realType, getTemplateConfigForObjectType(newObjectType));
        // newObject->setOrigin(newObject->getLocalBounds().width / 2, newObject->getLocalBounds().height / 2);
        oc = newObject->getColor();
        newObject->setColor(sf::Color(oc.r, oc.g, oc.b, oc.a / 4));
        return;
    }
    mouseState.leftDownPos = Game::getMousePos();

    auto clickedObject = getClickedObject();
    auto clickedPoint = getClickedPoint();
    // std::cout << (clickedObject ? getObjectType(clickedObject) : "null") << " " << clickedPoint << " " << selection.selectedObjects.size() << "\n";

    // Нажатие на точку сплайна (дальше случаи можно не рассматривать)
    if (selection.selectedGroundSpline && clickedPoint != -1)
    {
        // selectedObject не меняем так как существование спалайна => выделен Ground
        // по сути сплайн это инструмент для редактирования Ground
        // остальные поля (не verts) планируется менять через UI'шку
        selection.selectedSplinePoint = clickedPoint;
        return;
    }
    // Нажатие куда либо, но существует незаконченный сплайн
    if (selection.selectedGroundSpline && !selection.selectedGroundSpline->isFinished())
    {
        sf::Vector2f pos = Game::getMousePos() - selection.selectedObject->getPosition() + selection.selectedObject->getOrigin();
        selection.selectedGroundSpline->addControlPoint(pos);
        selection.selectedSplinePoint = selection.selectedGroundSpline->getPointCount() - 2;
        // selection.selectedSplinePoint = -1;
        return;
    }

    // Без сплайна

    // Нажатие в пустоту
    if (!clickedObject)
    {
        selection.selectedObjectType = "";
        selection.selectedObject = nullptr;
        selection.selectedObjects.clear();
        selection.visualRect.setPosition(mouseState.leftDownPos);
        if (selection.selectedGroundSpline)
            selection.selectedGroundSpline->setFinished(true);
        selection.selectedGroundSpline = nullptr;
        selection.selectedSplinePoint = -1;
        return;
    }

    // Нажатие не в пустоту

    // Нажатие с Ctrl'ом
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    {
        if (selection.selectedGroundSpline)
            selection.selectedGroundSpline->setFinished(true);
        selection.selectedGroundSpline = nullptr;
        selection.selectedSplinePoint = -1;
        auto it = std::find(selection.selectedObjects.begin(), selection.selectedObjects.end(), clickedObject);
        if (it != selection.selectedObjects.end())
            selection.selectedObjects.erase(it);
        else
            selection.selectedObjects.push_back(clickedObject);
    }

    // Нажатие по новому (не выделенному ранее) объекту (надо сбросить выделение)
    else if (std::find(selection.selectedObjects.begin(), selection.selectedObjects.end(), clickedObject) == selection.selectedObjects.end())
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
        auto& config = getConfigForObject(clickedObject);
        // Проверка - может, нажали на Ground?
        if (ptr && config.contains("bezierVerts") && config["bezierVerts"].get<bool>())
        {
            std::vector<sf::Vector2f> verts;
            verts.reserve(config["verts"].size());

            for (const auto &point : config["verts"])
                verts.emplace_back(sf::Vector2f(point[0], point[1]));
            selection.selectedGroundSpline = std::make_shared<Spline>(verts);
            selection.selectedGroundSpline->setPosition(clickedObject->getPosition());
        }
    }
    // Нажатие по уже выделенному объекту
    else if (auto ptr = std::dynamic_pointer_cast<Ground>(clickedObject) && selection.selectedGroundSpline)
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
    // Если выделения нет, меняем рамку
    if (selection.selectedObjects.size() == 0)
    {
        selection.visualRect.setSize(mousePos - mouseState.leftDownPos);
        return;
    }

    // Выделена точка, двигаем её
    if (selection.selectedSplinePoint != -1)
    {
        auto &spline = selection.selectedGroundSpline;
        int pointIndex = selection.selectedSplinePoint;
        spline->movePoint(pointIndex, delta);
        return;
    }

    // Особый случай - выделен Ground, двигаем его и все точки сплайна
    if (auto ptr = std::dynamic_pointer_cast<Ground>(selection.selectedObject))
    {
        ptr->move(delta);
        if (!selection.selectedGroundSpline)
            return;
        selection.selectedGroundSpline->move(delta);
    }
    // Выделены объекты, двигаем их
    else if (selection.selectedObjects.size() >= 1)
    {
        for (auto &obj : selection.selectedObjects)
            obj->move(delta);
    }
}

void Editor::onLeftUp()
{
    // Не тот режим
    if (mode == Create)
        return;

    // Выделение уже было
    if (selection.selectedObjects.size() != 0)
    {
        // Двигали точку - меняли Ground
        auto ptr = std::dynamic_pointer_cast<Ground>(selection.selectedObject);
        if (ptr && selection.selectedGroundSpline && selection.selectedGroundSpline->isFinished() && selection.selectedSplinePoint != -1)
        {
            updateGroundShape();
            return;
        }
        for (auto &obj : selection.selectedObjects)
        {
            auto &config = getConfigForObject(obj);
            std::string objType = getObjectType(obj);
            std::string key = (objType == "player" ? "spawn_pos" : (objType == "door" ? "start_pos" : "pos"));
            config[key] = {obj->getPosition().x, obj->getPosition().y};
        }
        return;
    }

    // Делаем новое выделение
    auto mousePos = Game::getMousePos();
    selection.visualRect.setSize({0, 0});

    if (abs((mousePos - mouseState.leftDownPos).x) < 5 && abs((mousePos - mouseState.leftDownPos).y) < 5)
        return;

    sf::FloatRect selectionRect(
        std::min(mouseState.leftDownPos.x, mousePos.x),
        std::min(mouseState.leftDownPos.y, mousePos.y),
        std::abs(mousePos.x - mouseState.leftDownPos.x),
        std::abs(mousePos.y - mouseState.leftDownPos.y));
    const Hitbox &selectionHitbox = ConvexHitbox{.points = {sf::Vector2f(selectionRect.left, selectionRect.top),
                                                            sf::Vector2f(selectionRect.left + selectionRect.width, selectionRect.top),
                                                            sf::Vector2f(selectionRect.left + selectionRect.width, selectionRect.top + selectionRect.height),
                                                            sf::Vector2f(selectionRect.left, selectionRect.top + selectionRect.height)}};
    selection.selectedObjects.clear();
    for (const auto &obj : drawable)
    {
        auto ptr = std::dynamic_pointer_cast<Ground>(obj);
        // Не Ground воспринимает как квадраты
        if (!ptr && selectionRect.intersects(obj->getGlobalBounds()))
            selection.selectedObjects.push_back(obj);
        // С Ground просчитываем коллизию
        else if (ptr)
        {
            const Hitbox &secondHitbox = ptr->getHitbox();
            if (collide(selectionHitbox, secondHitbox))
                selection.selectedObjects.push_back(obj);
        }
    }
}

void Editor::onRightDown()
{
    auto clickedPoint = getClickedPoint();
    std::cout << clickedPoint << "\n";
    if (clickedPoint != -1 && selection.selectedGroundSpline)
    {
        int removedPoints = selection.selectedGroundSpline->removeControlPoint(clickedPoint);
        if (selection.selectedGroundSpline->isFinished())
            updateGroundShape();
        if (abs(selection.selectedSplinePoint - clickedPoint) <= 1)
            selection.selectedSplinePoint = -1;
        else if (!selection.selectedGroundSpline->isFinished() && clickedPoint == 0 && selection.selectedSplinePoint <= 2)
            selection.selectedSplinePoint = -1;
        else if (selection.selectedSplinePoint > clickedPoint)
            selection.selectedSplinePoint -= removedPoints;
    }
}

void Editor::onMiddleDrag()
{
    sf::Vector2f delta = Game::getLocalMousePos() - mouseState.prevMiddlePos;
    camera->move(-delta * camera->getZoom());
}

void Editor::onMouseScroll(float delta)
{
    auto &objectManager = Game::getObjectManager();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    {
        float factor = objectManager->getCamera()->getZoom() * (delta > 0 ? 0.9f : 1.1f);
        factor = std::clamp(factor, 0.1f, 10.0f);
        objectManager->getCamera()->setZoom(factor);
    }
    if (mode == Create)
    {
        if (delta > 0)
            newObjectType = newObjectTypes[(std::find(newObjectTypes, newObjectTypes + 15, newObjectType) - newObjectTypes + 1) % 15];
        else
            newObjectType = newObjectTypes[(std::find(newObjectTypes, newObjectTypes + 15, newObjectType) - newObjectTypes + 14) % 15];
        std::string realType = (newObjectType.find("trigger:") != std::string::npos) ? "trigger" : newObjectType;
        newObject = createObjectOfType(realType, getTemplateConfigForObjectType(newObjectType));
        std::cout << "New object type: " << newObjectType << "\n";
        // newObject->setOrigin(newObject->getLocalBounds().width / 2, newObject->getLocalBounds().height / 2);
        sf::Color oc = newObject->getColor();
        newObject->setColor(sf::Color(oc.r, oc.g, oc.b, oc.a / 4));
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
        std::string realType = (newObjectType.find("trigger:") != std::string::npos) ? "trigger" : newObjectType;
        newObject = createObjectOfType(realType, getTemplateConfigForObjectType(newObjectType));
        // newObject->setOrigin(newObject->getLocalBounds().width / 2, newObject->getLocalBounds().height / 2);
        sf::Color oc = newObject->getColor();
        newObject->setColor(sf::Color(oc.r, oc.g, oc.b, oc.a / 4));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete) && selection.selectedObjects.size() > 0)
    {
        for (auto &obj : selection.selectedObjects)
        {
            auto it = std::find(drawable.begin(), drawable.end(), obj);
            if (it != drawable.end())
            {
                if ((*it) == player)
                    player = nullptr;
                size_t index = std::distance(drawable.begin(), it);
                drawable.erase(it);
                configs.erase(configs.begin() + index);
            }
        }
        selection.selectedObjects.clear();
        selection.selectedObject = nullptr;
        selection.selectedObjectType = "";
        selection.selectedGroundSpline = nullptr;
        selection.selectedSplinePoint = -1;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) && selection.selectedObjects.size() > 0)
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
            auto& config = getConfigForObject(obj);
            std::string objType = getObjectType(obj);
            std::string key = (objType == "player" ? "spawn_pos" : (objType == "door" ? "start_pos" : "pos"));
            config[key] = {obj->getPosition().x + 20, obj->getPosition().y + 20};
            auto newObj = createObjectOfType(objType, config);
            if (newObj)
            {
                drawable.push_back(newObj);
                configs.push_back({objType, config});
                newSelectedObjects.push_back(newObj);
            }
        }
        selection.selectedObjects = newSelectedObjects;
    }
    // ctrl + S - save
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)))
    {
        this->save(levelPath);
        std::cout << "Level saved to " << levelPath << "\n";
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) && selection.selectedGroundSpline)
    {
        bool newFinished = !selection.selectedGroundSpline->isFinished();
        selection.selectedGroundSpline->setFinished(newFinished);
        if (newFinished == true)
        {
            updateGroundShape();
        }
        else
        {
            selection.selectedSplinePoint = selection.selectedGroundSpline->getPointCount() - 2;
        }
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
    else if (type == "trigger:win_zone")
        return triggerWinConfigTemplate;
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
        return simpleObjectConfigTemplate;
    else if (type == "platform")
        return platformConfigTemplate;
    else if (type == "jump_pad")
        return jumpPadConfigTemplate;
    else if (type == "spikes")
        return spikesConfigTemplate;
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
    throw std::runtime_error("Unknown object type: " + type);
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
    // replace Ground object in drawable with new one
    int dIndex = std::distance(drawable.begin(), std::find(drawable.begin(), drawable.end(), selection.selectedObject));
    int sIndex = std::distance(selection.selectedObjects.begin(), std::find(selection.selectedObjects.begin(), selection.selectedObjects.end(), selection.selectedObject));
    selection.selectedObject = createObjectOfType("ground", config);
    drawable.at(dIndex) = selection.selectedObject;
    selection.selectedObjects.at(sIndex) = selection.selectedObject;
}

// ЛКМ по объекту - select

// СКМ + drag - сдвинуть камеру
// СКМ up/down - приблизить/отдалить
// ЛКМ по пустоте ... ЛКМ up - выделить много объектов
