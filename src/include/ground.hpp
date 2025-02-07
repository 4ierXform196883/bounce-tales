#pragma once

#include "game_object.hpp"

class Ground : public GameObject, public ICollidable
{
public:
    Ground(const std::vector<std::array<double, 2>>& verts);
    virtual ~Ground() = default;

    // ICollidable
    inline virtual const Hitbox &getHitbox() const override { return collidable->getHitbox(); }
    inline virtual bool isTrigger() const override { return collidable->isTrigger(); }

    // IPhysical
};