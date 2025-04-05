#pragma once

#include "game_object.hpp"
#include "timer.hpp"

class Wind : public GameObject, public ICollidable
{
public:
    COLLIDABLE

    Wind(const sf::Vector2f &size, const sf::Vector2f &direction);
    virtual ~Wind() = default;

protected:
    inline virtual void update() override {};
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    const sf::Vector2f size, direction;
    std::shared_ptr<Timer> particleTimer;
};