#pragma once

#include "game_object.hpp"

class Door;

class Switch : public GameObject, public ICollidable
{
public:
    COLLIDABLE

    Switch(const std::vector<std::shared_ptr<Door>> &doors);
    virtual ~Switch() = default;

    void setState(bool state);
    void switchState();

protected:
    inline virtual void update() override {};
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    bool isOn = false;
    std::vector<std::shared_ptr<Door>> doors;
};