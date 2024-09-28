#include "collidable.hpp"

#include <memory>

Collidable::Collidable(const Hitbox &hitbox, bool trigger)
    : hitbox(hitbox), trigger(trigger) {}

void Collidable::transUpdate(const sf::Transform &trans, const sf::Vector2f &scale)
{
    switch (hitbox.index())
    {
    case 0:
    {
        std::get<CircleHitbox>(hitbox).center = trans.transformPoint(std::get<CircleHitbox>(initHitbox).center);
        std::get<CircleHitbox>(hitbox).radius = std::get<CircleHitbox>(initHitbox).radius * std::max(scale.x, scale.y);
        break;
    }

    case 1:
        for (size_t i = 0; i < 4; ++i)
            std::get<RectangleHitbox>(hitbox).points[i] = trans.transformPoint(std::get<RectangleHitbox>(initHitbox).points[i]);
        break;

    case 2:
        for (size_t i = 0; i < std::get<ConcaveHitbox>(hitbox).points.size(); ++i)
            std::get<ConcaveHitbox>(hitbox).points[i] = trans.transformPoint(std::get<ConcaveHitbox>(initHitbox).points[i]);
        break;

    default:
        break;
    }
}