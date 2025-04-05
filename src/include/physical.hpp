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
    virtual void setMass(float value) = 0;
    virtual float getMass() const = 0;
    virtual void setAirResistance(float value) = 0;
    virtual float getAirResistance() const = 0;
};

class Physical : public IPhysical
{
public:
    Physical(float mass, float airResistance);
    virtual ~Physical() = default;

    virtual void addForce(const sf::Vector2f &force) override;
    virtual void addForce(const sf::Vector2f &direction, float power) override;
    virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) override;
    virtual const LongForce &getLongForce(const std::string &name) const override;
    virtual LongForce &getLongForce(const std::string &name) override;
    virtual void removeLongForce(const std::string &name) override;
    inline virtual const sf::Vector2f &getSpeed() const override { return speed; }
    virtual void setMass(float value) override;
    inline virtual float getMass() const override { return mass; }
    inline virtual void setAirResistance(float value) override { this->airResistance = value; }
    inline virtual float getAirResistance() const override { return airResistance; }

    void update();

    float mass, airResistance;
    sf::Vector2f speed;

    std::map<std::string, LongForce> longForces;
};