#pragma once

#include "game_object.hpp"

class Spikes : public GameObject, public ICollidable
{
public:
    COLLIDABLE

    Spikes(size_t count);
    virtual ~Spikes() = default;

protected:
    inline virtual void update() override {};
    virtual void onCollision(std::shared_ptr<GameObject> other) override;
    
};