#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "timer.hpp"
#include "game_object_component.hpp"

class PhysicalComponent : private GameObjectComponent
{
    friend GameObject;

public:
    PhysicalComponent(std::shared_ptr<GameObject> gameObject, float maxSpeed, float friction, float gravity = 0.0f);
    virtual ~PhysicalComponent() = default;

    void addForce(const sf::Vector2f &direction, float power);
    void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0);
    void modifyLongForce(const std::string &name, const sf::Vector2f &direction, float power);
    void removeLongForce(const std::string &name);

    inline const sf::Vector2f &getSpeed() const { return speed; }

protected:
    sf::Vector2f calcFrictionVec() const;
    void update();

    float maxSpeed, friction;
    sf::Vector2f speed;
    std::map<std::string, std::shared_ptr<Timer>> longForceTimers;
    std::map<std::string, sf::Vector2f> longForces;
};