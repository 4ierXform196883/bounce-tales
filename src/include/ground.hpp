#pragma once

#include "game_object.hpp"

class Ground : public GameObject, ICollidable
{
public:
    Ground(std::vector<sf::Vector2f> verticies);
    virtual ~Ground() = default;

    // ICollidable
    inline virtual const Hitbox &getHitbox() const override { return collidable->getHitbox(); }
    inline virtual bool isTrigger() const override { return collidable->isTrigger(); }

    // IPhysical
};