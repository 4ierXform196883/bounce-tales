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

    Enemy(const sf::Vector2f &startPos, float walkDistance, float speed);
    virtual ~Enemy() = default;

protected:
    virtual void update() override;
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    std::shared_ptr<Timer> downTimer;
    float walkDistance;
    float fullHeight;
    bool isDown = true;
    float speed = 1.0f;
    sf::Vector2f startPos;
};