#pragma once

#include "object_manager.hpp"
#include "spline.hpp"
#include <nlohmann/json.hpp>
#include "game.hpp"

class Editor : public ObjectManager
{
public:
    enum Mode
    {
        Create,
        Edit
    };

    virtual void load(const std::string &path) override;
    void save(const std::string &path);

protected:
    struct Selection
    {
        std::string selectedObjectType;
        std::shared_ptr<GameObject> selectedObject;
        std::shared_ptr<Spline> selectedGroundSpline;
        int selectedSplinePoint; // Spline, point index
        std::vector<std::shared_ptr<GameObject>> selectedObjects;
        sf::RectangleShape visualRect;
    };

    struct MouseState
    {
        sf::Vector2f leftDownPos;
        sf::Vector2f prevLeftPos;
        sf::Vector2f prevMiddlePos;
        bool prevLeftPressed = false;
        bool prevRightPressed = false;
    };

    virtual void updateAll() override;
    inline virtual void collideAll() override {};
    virtual void drawAll(sf::RenderTarget &target);

    int getClickedPoint();
    std::shared_ptr<GameObject> getClickedObject();

    virtual void handleEvent(const sf::Event &event) override;
    void onMouseScroll(float delta);

    void handleInput();
    void onLeftDown();
    void onLeftUp();
    void onLeftDrag();
    void onRightDown();
    void onMiddleDrag();
    void handleKeyboard();

    bool collide(const Hitbox &firstHitbox, const Hitbox &secondHitbox);
    nlohmann::json &getConfigForObject(const std::shared_ptr<GameObject> &object);
    const std::string &getObjectType(const std::shared_ptr<GameObject> &object);
    const nlohmann::json &getTemplateConfigForObjectType(const std::string &type);

    void updateGroundShape();

    Selection selection;
    MouseState mouseState;
    Mode mode = Edit;
    std::string levelName;
    std::string levelPath;

    std::string newObjectType = "player";
    std::shared_ptr<GameObject> newObject;
    std::string newObjectTypes[15] = {"player", "simple", "jump_pad", "spikes",
                                      "door", "switch", "wind", "water",
                                      "trigger:death_zone", "trigger:egg",
                                      "trigger:win_zone", "trigger:player_set_skin:light",
                                      "trigger:player_set_skin:normal", "trigger:player_set_skin:heavy",
                                      "ground"};

    std::vector<std::pair<std::string, nlohmann::json>> configs; // 1 к 1 с drawable
    nlohmann::json cameraConfig;
    nlohmann::json backgroundConfig;
};