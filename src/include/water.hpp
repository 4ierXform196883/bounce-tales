#pragma once

#include "game_object.hpp"
#include "timer.hpp"

class Water : public GameObject, public ICollidable
{
public:
    COLLIDABLE

    Water(const sf::Vector2f &size);
    virtual ~Water() = default;

protected:
    inline virtual void update() override {};
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    const sf::Vector2f size;
    std::shared_ptr<Timer> particleTimer;
};