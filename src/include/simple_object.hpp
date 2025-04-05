#pragma once

#include "game_object.hpp"

class SimpleObject : public GameObject
{
public:
    SimpleObject(const std::string &tag, const std::string &textureName);
    SimpleObject(const std::string &tag, const std::string &textureName, const std::string &subtextureName);
    SimpleObject(const std::string &tag, const std::string &textureName, const std::string &animationName, double fps);
    virtual ~SimpleObject() = default;

protected:
    inline virtual void update() override {};
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};
};