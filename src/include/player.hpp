#pragma once

#include "game_object.hpp"

class Player : public GameObject, public ICollidable, public IPhysical, public ISoundPlayer
{
public:
    COLLIDABLE
    PHYSICAL
    SOUND_PLAYER

    Player(float control_force);
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