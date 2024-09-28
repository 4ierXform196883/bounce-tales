#pragma once

#include "game_object.hpp"

class Background : public GameObject
{
public:
    Background();
    virtual ~Background() = default;

protected:
    inline virtual void update() override {};
};