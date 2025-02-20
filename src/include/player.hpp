#pragma once

#include "game_object.hpp"

class Player : public GameObject, public ICollidable, public IPhysical /*, public ISoundPlayer*/ // IMPLEMENT
{
public:
    COLLIDABLE
    PHYSICAL

    Player(const sf::Vector2f& pos, float control_force, float maxSpeed, float friction, float graivty);
    virtual ~Player() = default;

    // IMPLEMENT
    // void onDeath();
    // void onWin();

    float control_force = 0.1;

protected:
    virtual void update() override;
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    bool onGround = false;
    float lastJumpTime = 0;
};