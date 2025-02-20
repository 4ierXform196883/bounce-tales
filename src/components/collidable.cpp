#include "collidable.hpp"

#include <memory>
#include "transformable.hpp"
#include "physical.hpp"

Collidable::Collidable(const Hitbox &hitbox, bool trigger)
    : initHitbox(hitbox), hitbox(hitbox), trigger(trigger) {}

void Collidable::update(const sf::Transform &trans, const sf::Vector2f &scale)
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
    {
        for (size_t i = 0; i < std::get<ConvexHitbox>(hitbox).points.size(); ++i)
            std::get<ConvexHitbox>(hitbox).points[i] = trans.transformPoint(std::get<ConvexHitbox>(initHitbox).points[i]);
        break;
    }

    case 2:
    {
        for (size_t i = 0; i < std::get<ConcaveHitbox>(hitbox).triangles.size(); ++i)
            for (size_t j = 0; j < 3; ++j)
                std::get<ConcaveHitbox>(hitbox).triangles[i].points[j] = trans.transformPoint(std::get<ConcaveHitbox>(initHitbox).triangles[i].points[j]);
        break;
    }

    default:
        break;
    }
}