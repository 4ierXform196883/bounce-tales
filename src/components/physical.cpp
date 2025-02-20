#include "physical.hpp"
#include <cmath>
#include <iostream>

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))

Physical::Physical(float mass, float maxSpeed, float friction, float gravity)
    : mass(mass), maxSpeed(maxSpeed), friction(friction)
{
    this->addLongForce("gravity", sf::Vector2f(0.0f, 1.0f), gravity);
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

sf::Vector2f Physical::calcFrictionVec() const
{
    float fricX = -sign(speed.x) * friction;
    if (abs(fricX) > abs(speed.x))
        fricX = -speed.x;
    else if (abs(speed.x) > maxSpeed)
        fricX = abs(speed.x) - maxSpeed > abs(fricX) * 10 ? fricX * 10 : -sign(speed.x) * (abs(speed.x) - maxSpeed);

    float fricY = -sign(speed.y) * friction;
    if (abs(fricY) > abs(speed.y))
        fricY = -speed.y;
    else if (speed.y > maxSpeed)
        fricY = speed.y - maxSpeed > abs(fricY) * 10 ? fricY * 10 : -(speed.y - maxSpeed);
    else if (speed.y < -maxSpeed)
        fricY = abs(speed.y) - maxSpeed > abs(fricY) * 10 ? fricY * 10 : -(speed.y + maxSpeed);

    return {fricX, fricY};
}

void Physical::update()
{
    for (auto &[name, force] : longForces)
    {
        this->speed += force.direction * force.power;
    }
    this->speed += calcFrictionVec();
    // this->speed.x = std::min(this->speed.x, maxSpeed);
    // this->speed.y = std::min(this->speed.y, maxSpeed);
}
