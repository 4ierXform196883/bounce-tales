#pragma once

#include "game_object.hpp"
#include "primitive_sprite.hpp"
#include "animation.hpp"
#include "timer.hpp"

class JumpPad : public GameObject, public ICollidable
{
public:
    JumpPad();
    virtual ~JumpPad() = default;

    virtual void update() override;
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    // ICollidable
    inline virtual const Hitbox &getHitbox() const override { return collidable->getHitbox(); }
    inline virtual bool isTrigger() const override { return collidable->isTrigger(); }

private:
    std::shared_ptr<PrimitiveSprite> idleState;
    std::shared_ptr<Animation> activeState;
    std::shared_ptr<Timer> jumpTimer;
    bool shouldPush = false;
    std::shared_ptr<Timer> reloadTimer;
    bool reloading = false;
};