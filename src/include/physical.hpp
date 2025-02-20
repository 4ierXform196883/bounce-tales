#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "timer.hpp"

struct LongForce
{
    sf::Vector2f direction;
    float power;
    std::shared_ptr<Timer> timer;
};

class IPhysical
{
public:
    virtual void addForce(const sf::Vector2f &force) = 0;
    virtual void addForce(const sf::Vector2f &direction, float power) = 0;
    virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) = 0;
    virtual const LongForce &getLongForce(const std::string &name) const = 0;
    virtual LongForce &getLongForce(const std::string &name) = 0;
    virtual void removeLongForce(const std::string &name) = 0;
    virtual const sf::Vector2f &getSpeed() const = 0;
    virtual void setGravity(float power) = 0;
    virtual void setFriction(float power) = 0;
    virtual void setMaxSpeed(float value) = 0;
    virtual void setMass(float value) = 0;
    virtual float getGravity() const = 0;
    virtual float getFriction() const = 0;
    virtual float getMaxSpeed() const = 0;
    virtual float getMass() const = 0;
};

class Physical : public IPhysical
{
public:
    Physical(float mass, float maxSpeed, float friction, float gravity = 0.0f);
    virtual ~Physical() = default;

    virtual void addForce(const sf::Vector2f &force) override;
    virtual void addForce(const sf::Vector2f &direction, float power) override;
    virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) override;
    virtual const LongForce &getLongForce(const std::string &name) const override;
    virtual LongForce &getLongForce(const std::string &name) override;
    virtual void removeLongForce(const std::string &name) override;
    inline virtual const sf::Vector2f &getSpeed() const override { return speed; }
    inline virtual void setGravity(float power) override { longForces.at("gravity").power = power; }
    inline virtual void setFriction(float power) override { this->friction = power; }
    inline virtual void setMaxSpeed(float value) override { this->maxSpeed = value; }
    inline virtual void setMass(float value) override { this->mass = value; }
    inline virtual float getGravity() const override { return longForces.at("gravity").power; }
    inline virtual float getFriction() const override { return friction; }
    inline virtual float getMaxSpeed() const override { return maxSpeed; }
    inline virtual float getMass() const override { return mass; }

    void update();
    sf::Vector2f calcFrictionVec() const;

    float maxSpeed, friction;
    float mass;
    sf::Vector2f speed;

    std::map<std::string, LongForce> longForces;
};