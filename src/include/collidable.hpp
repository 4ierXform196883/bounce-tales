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

struct ConcaveHitbox
{
    std::vector<sf::Vector2f> points;
};

using Hitbox = std::variant<CircleHitbox, ConcaveHitbox>;

class ICollidable
{
public:
    virtual const Hitbox &getHitbox() const = 0;
    virtual bool isTrigger() const = 0;
};

class Collidable : public ICollidable
{
    friend class Transformable;
    friend class ObjectManager;

public:
    Collidable(const Hitbox &hitbox, bool trigger = false);
    virtual ~Collidable() = default;

    inline virtual const Hitbox &getHitbox() const override { return hitbox; }
    inline virtual bool isTrigger() const override { return trigger; }

    void transUpdate(const sf::Transform &trans, const sf::Vector2f &scale);

    bool trigger;
    Hitbox hitbox;
    const Hitbox initHitbox;
};