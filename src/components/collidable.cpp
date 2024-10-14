#include "collidable.hpp"

#include <memory>
#include "transformable.hpp"
#include "physical.hpp"

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

void Collidable::calculateCollision(std::shared_ptr<Collidable> first, std::shared_ptr<Collidable> second)
{
    if (first->hitbox.index() != 0)
        return;
    switch (second->hitbox.index())
    {
    case 0:
        calculateCircleCollision(first, second);
        break;

    case 1:
        calculateRectangleCollision(first, second);
        break;

    case 2:
        calculateConcaveCollision(first, second);
        break;

    default:
        break;
    }
}

void Collidable::calculateCircleCollision(std::shared_ptr<Collidable> first, std::shared_ptr<Collidable> second)
{
    CircleHitbox &firstHitbox = std::get<CircleHitbox>(first->hitbox);
    CircleHitbox &secondHitbox = std::get<CircleHitbox>(second->hitbox);
    std::shared_ptr<Transformable> firstTrans = std::dynamic_pointer_cast<Transformable>(first);
    std::shared_ptr<Transformable> secondTrans = std::dynamic_pointer_cast<Transformable>(second);
    std::shared_ptr<Physical> firstPhys = std::dynamic_pointer_cast<Physical>(first);
    std::shared_ptr<Physical> secondPhys = std::dynamic_pointer_cast<Physical>(second);
    if (!firstTrans || !secondTrans || !firstPhys)
        return;
    // firstTrans->getPosition(); firstTrans->move(10, 10); ...
    // firstPhys->speed; firstPhys->addForce({1.0, 2.0}, 1000.0f); ...

    // checking collision...

    // if(noCollision) return;

    if (second->isTrigger())
    {
        // No movement because collision is with a trigger
    }
    else
    {
        // Move objects appropriately
    }
    first->onCollision(second);
    second->onCollision(first);
}

void Collidable::calculateRectangleCollision(std::shared_ptr<Collidable> circle, std::shared_ptr<Collidable> rectangle)
{
}

void Collidable::calculateConcaveCollision(std::shared_ptr<Collidable> circle, std::shared_ptr<Collidable> concave)
{
}
