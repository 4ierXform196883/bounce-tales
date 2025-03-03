#include "physical.hpp"
#include <cmath>
#include <iostream>

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define normalized(vec) (vec.x == 0 && vec.y == 0 ? sf::Vector2f(0, 0) : (1 / norm(vec)) * vec)

Physical::Physical(float mass, float airResistance)
    : mass(mass), airResistance(airResistance)
{
    this->addLongForce("gravity", sf::Vector2f(0.0f, 1.0f), mass * 0.02);
    this->addLongForce("NULL", sf::Vector2f(1.0f, 1.0f), 0);
}

void Physical::addForce(const sf::Vector2f &force)
{
    this->speed += force;
}

void Physical::addForce(const sf::Vector2f &direction, float power)
{
    if (direction.x == 0 && direction.y == 0)
        return;
    sf::Vector2f norm = direction * (1.0f / norm(direction));
    this->speed += norm * power;
}

void Physical::addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration)
{
    if (direction.x == 0 && direction.y == 0)
        return;
    sf::Vector2f normalized = direction * (1.0f / norm(direction));
    std::shared_ptr<Timer> timer = duration == 0 ? nullptr : Timer::create(duration, [name, this]()
                                                                           { this->longForces.erase(name); }, false);
    longForces.emplace(name, LongForce{normalized, power, timer});
}

const LongForce &Physical::getLongForce(const std::string &name) const
{
    if (longForces.find(name) == longForces.end())
        return longForces.at("NULL");
    return longForces.at(name);
}

LongForce &Physical::getLongForce(const std::string &name)
{
    if (longForces.find(name) == longForces.end())
        return longForces.at("NULL");
    return longForces.at(name);
}

void Physical::removeLongForce(const std::string &name)
{
    longForces.erase(name);
}

// sf::Vector2f Physical::calcFrictionVec() const
// {
//     sf::Vector2f fric = -sf::Vector2f(sign(speed.x), sign(speed.y)) * friction;
    
//     if (abs(fric.x) > abs(speed.x))
//         fric.x = -speed.x;
//     else if (abs(speed.x) > maxSpeed)
//         fric.x = std::clamp(fric.x * 10, -abs(speed.x - maxSpeed), abs(speed.x - maxSpeed));
    
//     if (abs(fric.y) > abs(speed.y))
//         fric.y = -speed.y;
//     else if (abs(speed.y) > maxSpeed)
//         fric.y = std::clamp(fric.y * 10, -abs(speed.y - maxSpeed), abs(speed.y - maxSpeed));
    
//     return fric;
// }

void Physical::update()
{
    for (auto &[name, force] : longForces)
    {
        this->speed += force.direction * force.power;
    }
    this->speed += -normalized(speed) * (1/2.f) * norm(speed) * norm(speed) * airResistance;
    // this->speed += calcFrictionVec();
    // this->speed.x = std::min(this->speed.x, maxSpeed);
    // this->speed.y = std::min(this->speed.y, maxSpeed);
}
