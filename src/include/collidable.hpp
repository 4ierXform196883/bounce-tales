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

struct RectangleHitbox
{
    std::array<sf::Vector2f, 4> points;
};

struct ConcaveHitbox
{
    std::vector<sf::Vector2f> points;
};

using Hitbox = std::variant<CircleHitbox, RectangleHitbox, ConcaveHitbox>;

class Collidable
{
    friend class Transformable;
    friend class ObjectManager;

public:
    Collidable(const Hitbox &hitbox, bool trigger = false);
    virtual ~Collidable() = default;

    inline const Hitbox &getHitbox() const { return hitbox; }
    inline bool isTrigger() const { return trigger; }

private:
    virtual void onCollision(std::shared_ptr<const Collidable> other) = 0;
    void transUpdate(const sf::Transform &trans, const sf::Vector2f &scale);
    
    static void calculateCollision(std::shared_ptr<Collidable> first, std::shared_ptr<Collidable> second);
    static void calculateCircleCollision(std::shared_ptr<Collidable> first, std::shared_ptr<Collidable> second);
    static void calculateRectangleCollision(std::shared_ptr<Collidable> circle, std::shared_ptr<Collidable> rectangle);
    static void calculateConcaveCollision(std::shared_ptr<Collidable> circle, std::shared_ptr<Collidable> concave);

    bool trigger;
    Hitbox hitbox;
    const Hitbox initHitbox;
};