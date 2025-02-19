#pragma once

#include "game_object.hpp"

class Player : public GameObject, public ICollidable, public IPhysical/*, public ISoundPlayer*/ // IMPLEMENT
{
public:
    Player();
    virtual ~Player() = default;

    virtual void update() override;
    virtual void onCollision(std::shared_ptr<IGameObject> other) override;

    // IMPLEMENT
    // void onDeath();
    // void onWin();

    // ICollidable
    inline virtual const Hitbox &getHitbox() const override { return collidable->getHitbox(); }
    inline virtual bool isTrigger() const override { return collidable->isTrigger(); }

    // IPhysical
    inline virtual void addForce(const sf::Vector2f &force) override { physical->addForce(force); };
    inline virtual void addForce(const sf::Vector2f &direction, float power) override { physical->addForce(direction, power); }
    inline virtual void addLongForce(const std::string &name, const sf::Vector2f &direction, float power, float duration = 0) override { physical->addLongForce(name, direction, power, duration); }
    inline virtual void modifyLongForce(const std::string &name, const sf::Vector2f &direction, float power) override { physical->modifyLongForce(name, direction, power); }
    inline virtual void modifyLongForce(const std::string &name, const sf::Vector2f &direction) override { physical->modifyLongForce(name, direction); }
    inline virtual void removeLongForce(const std::string &name) override { physical->removeLongForce(name); }
    inline virtual void setGravity(bool state) override { physical->setGravity(state); }
    inline virtual float getGravity() const override { return physical->getGravity(); }
    inline virtual const sf::Vector2f &getSpeed() const override { return physical->getSpeed(); }

private:
    bool onGround = false;
    bool controllable = true; // IMPLEMENT
    float lastJumpTime = 0;
};