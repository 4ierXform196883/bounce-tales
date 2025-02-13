#pragma once

#include "game_object.hpp"
#include "camera.hpp"

class Background : public GameObject
{
public:
    Background();
    virtual ~Background() = default;
};