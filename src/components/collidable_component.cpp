#include "collidable_component.hpp"

#include <memory>
#include "game_object.hpp"


CollidableComponent::CollidableComponent(std::shared_ptr<GameObject> gameObject, const Hitbox &hitbox, bool trigger)
    : GameObjectComponent(gameObject), hitbox(hitbox), trigger(trigger) {}

void CollidableComponent::calculateCollision(std::shared_ptr<CollidableComponent> first, std::shared_ptr<CollidableComponent> second)
{
    // Return if the first object isn't a circle (player)
    if (first->hitbox.index() != 0)
        return;

    switch (second->hitbox.index())
    {
    case 1:
        calculateRectCollision(first, second);
        break;

    case 2:
        calculateConcaveCollision(first, second);
        break;

    default:
        break;
    }
}

void CollidableComponent::update()
{
    auto ptr = gameObject.lock();
    switch (hitbox.index())
    {
    case 0:
    {
        std::get<CircleHitbox>(hitbox).center = ptr->getTransformV().transformPoint(std::get<CircleHitbox>(initHitbox).center);
        const sf::Vector2f &sc = ptr->getScaleV();
        std::get<CircleHitbox>(hitbox).radius = std::get<CircleHitbox>(initHitbox).radius * std::max(sc.x, sc.y);
        break;
    }

    case 1:
        for (size_t i = 0; i < 4; ++i)
            std::get<RectangleHitbox>(hitbox).points[i] = ptr->getTransformV().transformPoint(std::get<RectangleHitbox>(initHitbox).points[i]);
        break;

    case 2:
        for (size_t i = 0; i < std::get<ConcaveHitbox>(hitbox).points.size(); ++i)
            std::get<ConcaveHitbox>(hitbox).points[i] = ptr->getTransformV().transformPoint(std::get<ConcaveHitbox>(initHitbox).points[i]);
        break;

    default:
        break;
    }
}

void CollidableComponent::calculateRectCollision(std::shared_ptr<CollidableComponent> first, std::shared_ptr<CollidableComponent> second)
{
    CircleHitbox &circle = std::get<CircleHitbox>(first->hitbox);
    RectangleHitbox &rect = std::get<RectangleHitbox>(second->hitbox);
    // коллизия с прямоугольником
}

void CollidableComponent::calculateConcaveCollision(std::shared_ptr<CollidableComponent> first, std::shared_ptr<CollidableComponent> second)
{
    CircleHitbox &circle = std::get<CircleHitbox>(first->hitbox);
    ConcaveHitbox &concave = std::get<ConcaveHitbox>(second->hitbox);
    // коллизия с невыпуклым объектом
}
