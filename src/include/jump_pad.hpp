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
    COLLIDABLE

    JumpPad(float power);
    virtual ~JumpPad() = default;

    float power;

protected:
    virtual void update() override;
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    void updateFrame(size_t newFrame);

    static const std::vector<std::string> frameSequence;
    static const std::vector<int> heights;
    size_t currentFrame = 0;
    std::shared_ptr<Timer> animTimer;
};