#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "timer.hpp"

class Physical
{
    friend class GameObject;
    friend class ObjectManager; // this->move(physical->speedUpdate());
    friend class Collidable; // has to change speed directly

public:
    Physical(float maxSpeed, float friction, float gravity = 0.0f);
    virtual ~Physical() = default;

    virtual void addForce(const sf::Vector2f &direction, float power);
    virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0);
    virtual void modifyLongForce(const std::string &name, const sf::Vector2f &direction, float power);
    virtual void removeLongForce(const std::string &name);
    inline virtual const sf::Vector2f &getSpeed() const { return speed; }

private:
    const sf::Vector2f &speedUpdate();
    sf::Vector2f calcFrictionVec() const;

    float maxSpeed, friction;
    sf::Vector2f speed;
    std::map<std::string, std::shared_ptr<Timer>> longForceTimers;
    std::map<std::string, sf::Vector2f> longForces;
};