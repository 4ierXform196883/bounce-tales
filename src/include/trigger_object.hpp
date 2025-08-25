#pragma once

#include "game_object.hpp"

class TriggerObject : public GameObject, public ICollidable
{
public:
    COLLIDABLE

    TriggerObject(const sf::Vector2f &size, const std::string& callbackType);
    TriggerObject(const std::string &textureName, const std::string& callbackType);
    TriggerObject(const std::string &textureName, const std::string &subtextureName, const std::string& callbackType);
    TriggerObject(const std::string &textureName, const std::string &animationName, float fps, const std::string& callbackType);
    virtual ~TriggerObject() = default;

protected:
    inline virtual void update() override {};
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override { this->callback(other); }
    std::function<void(std::shared_ptr<GameObject> other)> getCallback(const std::string& callbackType);

    const std::function<void(std::shared_ptr<GameObject> other)> callback;
};
