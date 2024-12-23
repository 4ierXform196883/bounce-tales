#pragma once

#include "game_object.hpp"

class Player : public GameObject, ICollidable, IPhysical
{
public:
    Player();
    virtual ~Player() = default;

    // ICollidable
    inline virtual const Hitbox &getHitbox() const override { return collidable->getHitbox(); }
    inline virtual bool isTrigger() const override { return collidable->isTrigger(); }

    // IPhysical
    virtual void addForce(const sf::Vector2f &force) override { physical->addForce(force); };
    inline virtual void addForce(const sf::Vector2f &direction, float power) { physical->addForce(direction, power); }
    inline virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) { physical->addLongForce(name, direction, power, duration); }
    inline virtual void modifyLongForce(const std::string &name, const sf::Vector2f &direction, float power) { physical->modifyLongForce(name, direction, power); }
    inline virtual void removeLongForce(const std::string &name) { physical->removeLongForce(name); }
    inline virtual const sf::Vector2f &getSpeed() const { return physical->getSpeed(); }
};