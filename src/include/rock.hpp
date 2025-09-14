#pragma once

#include "game_object.hpp"

class Rock : public GameObject, public ICollidable, public IPhysical
{
public:
    COLLIDABLE
    PHYSICAL

    Rock();
    virtual ~Rock() = default;

protected:
    inline virtual void update() override {};
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};
};