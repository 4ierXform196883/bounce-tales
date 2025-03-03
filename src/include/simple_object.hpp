#pragma once

#include "game_object.hpp"

class SimpleObject : public GameObject
{
public:
    SimpleObject(const std::string &tag, const std::string &textureName);
    virtual ~SimpleObject() = default;

protected:
    inline virtual void update() override {};
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};
};