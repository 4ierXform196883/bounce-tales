#pragma once

#include "game_object.hpp"
#include "camera.hpp"

class Background : public GameObject
{
public:
    Background(size_t islands, size_t clouds, float additional_distance = 0);
    virtual ~Background() = default;

protected:
    inline virtual void update() override {};
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};
};