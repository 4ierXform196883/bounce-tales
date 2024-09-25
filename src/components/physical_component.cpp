#include "physical_component.hpp"
#include <cmath>
#include "game_object.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

PhysicalComponent::PhysicalComponent(std::shared_ptr<GameObject> gameObject, float maxSpeed, float friction, float gravity)
    : GameObjectComponent(gameObject), maxSpeed(maxSpeed), friction(friction)
{
    if (gravity != 0.0f)
        this->addLongForce("gravity", {0.0f, 1.0f}, gravity);
}

void PhysicalComponent::addForce(const sf::Vector2f &direction, float power)
{
    sf::Vector2f norm = direction * std::sqrt(direction.x * direction.x + direction.y + direction.y);
    this->speed += norm * power;
}

void PhysicalComponent::addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration)
{
    sf::Vector2f norm = direction * std::sqrt(direction.x * direction.x + direction.y + direction.y);
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

void PhysicalComponent::modifyLongForce(const std::string &name, const sf::Vector2f &direction, float power)
{
    sf::Vector2f norm = direction * std::sqrt(direction.x * direction.x + direction.y + direction.y);
    longForces.at(name) = norm * power;
}

void PhysicalComponent::removeLongForce(const std::string &name)
{
    longForceTimers.erase(name);
    longForces.erase(name);
}

sf::Vector2f PhysicalComponent::calcFrictionVec() const
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

void PhysicalComponent::update()
{
    for (auto &[name, force] : longForces)
    {
        this->speed += force;
    }
    this->speed += calcFrictionVec();
    this->gameObject.lock()->moveV(speed);
}
