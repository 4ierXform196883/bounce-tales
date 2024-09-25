#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <variant>
#include <memory>
#include "game_object_component.hpp"

struct CircleHitbox
{
    float radius;
    sf::Vector2f center{0, 0};
};

struct RectangleHitbox
{
    std::array<sf::Vector2f, 4> points;
};

struct ConcaveHitbox
{
    std::vector<sf::Vector2f> points;
};

using Hitbox = std::variant<CircleHitbox, RectangleHitbox, ConcaveHitbox>;

class CollidableComponent : private GameObjectComponent
{
    friend GameObject;

public:
    CollidableComponent(std::shared_ptr<GameObject> gameObject, const Hitbox &hitbox, bool trigger = false);
    virtual ~CollidableComponent() = default;

    const Hitbox &getHitbox() const { return hitbox; }

    static void calculateCollision(std::shared_ptr<CollidableComponent> first, std::shared_ptr<CollidableComponent> second);

protected:
    void update();
    
    static void calculateRectCollision(std::shared_ptr<CollidableComponent> first, std::shared_ptr<CollidableComponent> second);
    static void calculateConcaveCollision(std::shared_ptr<CollidableComponent> first, std::shared_ptr<CollidableComponent> second);

    bool trigger;
    Hitbox hitbox;
    const Hitbox initHitbox;
};