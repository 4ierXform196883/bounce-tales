#include "physical.hpp"
#include <cmath>
#include <iostream>

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

Physical::Physical(float maxSpeed, float friction, float gravity)
    : maxSpeed(maxSpeed), friction(friction), gravity(gravity)
{
    if (gravity != 0.0f)
    {
        this->addLongForce("gravity", {0.0f, 1.0f}, gravity);
    }
}

void Physical::addForce(const sf::Vector2f &force)
{
    this->speed += force;
}

void Physical::addForce(const sf::Vector2f &direction, float power)
{
    sf::Vector2f norm = direction * std::sqrt(direction.x * direction.x + direction.y + direction.y);
    this->speed += norm * power;
}

void Physical::addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration)
{
    sf::Vector2f norm = direction * (1.0f / std::sqrt(direction.x * direction.x + direction.y + direction.y));
    longForces.emplace(name, norm * power);
    if (duration == 0)
        return;
    auto forceCor = Timer::create(
        duration, [name, this]()
        {
                    this->longForceTimers.erase(name);
                    this->longForces.erase(name); },
        false);
    longForceTimers.emplace(name, forceCor);
}

void Physical::modifyLongForce(const std::string &name, const sf::Vector2f &direction)
{
    longForces[name] = direction;
}

void Physical::modifyLongForce(const std::string &name, const sf::Vector2f &direction, float power)
{
    sf::Vector2f norm = direction * (1.0f / std::sqrt(direction.x * direction.x + direction.y + direction.y));
    longForces[name] = norm * power;
}

void Physical::removeLongForce(const std::string &name)
{
    longForceTimers.erase(name);
    longForces.erase(name);
}

void Physical::setGravity(bool state)
{
    if (state && this->gravity != 0.0f)
    {
        this->addLongForce("gravity", {0.0f, 1.0f}, gravity);
    }
    else
    {
        longForces.erase("gravity");
    }
}

float Physical::getGravity() const
{
    return gravity;
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

const sf::Vector2f &Physical::speedUpdate()
{
    for (auto &[name, force] : longForces)
    {
        this->speed += force;
    }
    this->speed += calcFrictionVec();
    this->speed.x = std::min(this->speed.x, maxSpeed);
    this->speed.y = std::min(this->speed.y, maxSpeed);
    return this->speed;
}
