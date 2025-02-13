#pragma once

#include "game_object.hpp"
#include "primitive_sprite.hpp"
#include "animation.hpp"
#include "timer.hpp"
#include <vector>
#include <string>

class JumpPad : public GameObject, public ICollidable
{
public:
    JumpPad();
    virtual ~JumpPad() = default;

    virtual void update() override;
    virtual void onCollision(std::shared_ptr<IGameObject> other) override;

    // ICollidable
    inline virtual const Hitbox &getHitbox() const override { return collidable->getHitbox(); }
    inline virtual bool isTrigger() const override { return collidable->isTrigger(); }

private:
    void updateFrame(size_t newFrame);

    // std::shared_ptr<PrimitiveSprite> idleState;
    // std::shared_ptr<Animation> activeState;
    static const std::vector<std::string> frameSequence;
    static const std::vector<int> heights;
    size_t currentFrame = 0;
    std::vector<std::shared_ptr<IGameObject>> touching;
    std::shared_ptr<Timer> animTimer;
    // std::shared_ptr<Timer> jumpTimer;
    // bool shouldPush = false;
    // std::shared_ptr<Timer> reloadTimer;
    // bool reloading = false;
};