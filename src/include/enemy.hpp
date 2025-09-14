#pragma once

#include "game_object.hpp"
#include "primitive_sprite.hpp"
#include "animation.hpp"
#include "timer.hpp"
#include <vector>
#include <string>

class Enemy : public GameObject, public ICollidable
{
public:
    COLLIDABLE

    Enemy(float walkDistance);
    virtual ~Enemy() = default;

protected:
    virtual void update() override;
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    std::shared_ptr<Timer> hideTimer;
    float walkDistance;
    bool isHidden = false;
};