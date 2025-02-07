#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "timer.hpp"

class IPhysical
{
public:
    virtual void addForce(const sf::Vector2f &force) = 0;
    virtual void addForce(const sf::Vector2f &direction, float power) = 0;
    virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) = 0;
    virtual void modifyLongForce(const std::string &name, const sf::Vector2f &direction, float power) = 0;
    virtual void removeLongForce(const std::string &name) = 0;
    virtual const sf::Vector2f &getSpeed() const = 0;
};

class Physical : public IPhysical
{
public:
    Physical(float maxSpeed, float friction, float gravity = 0.0f);
    virtual ~Physical() = default;

    virtual void addForce(const sf::Vector2f &force) override;
    virtual void addForce(const sf::Vector2f &direction, float power) override;
    virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) override;
    virtual void modifyLongForce(const std::string &name, const sf::Vector2f &direction, float power) override;
    virtual void removeLongForce(const std::string &name) override;
    inline virtual const sf::Vector2f &getSpeed() const override { return speed; }

    const sf::Vector2f &speedUpdate();
    sf::Vector2f calcFrictionVec() const;

    float maxSpeed, friction;
    sf::Vector2f speed;
    std::map<std::string, std::shared_ptr<Timer>> longForceTimers;
    std::map<std::string, sf::Vector2f> longForces;
};