#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <variant>
#include <memory>

struct CircleHitbox
{
    float radius;
    sf::Vector2f center;
};

struct ConvexHitbox
{
    std::vector<sf::Vector2f> points;
};

struct ConcaveHitbox
{
    std::vector<ConvexHitbox> triangles;
};

using Hitbox = std::variant<CircleHitbox, ConvexHitbox, ConcaveHitbox>;

class ICollidable
{
public:
    virtual const Hitbox &getHitbox() const = 0;
    virtual bool isTrigger() const = 0;
};

class Collidable : public ICollidable
{
public:
    Collidable(const Hitbox &hitbox, bool trigger = false);
    virtual ~Collidable() = default;

    inline virtual const Hitbox &getHitbox() const override { return hitbox; }
    inline virtual bool isTrigger() const override { return trigger; }

    void update(const sf::Transform &trans, const sf::Vector2f &scale);

    bool trigger;
    Hitbox hitbox;
    const Hitbox initHitbox;
};