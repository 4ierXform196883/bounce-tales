#pragma once

#include "game_object.hpp"

class TriggerObject : public GameObject, public ICollidable, public ISoundPlayer
{
public:
    COLLIDABLE
    SOUND_PLAYER

    TriggerObject(const sf::Vector2f &size, const std::function<void(std::shared_ptr<GameObject> other)> &callback);
    TriggerObject(const std::string &textureName, const std::function<void(std::shared_ptr<GameObject> other)> &callback);
    TriggerObject(const std::string &textureName, const std::string &subtextureName, const std::function<void(std::shared_ptr<GameObject> other)> &callback);
    TriggerObject(const std::string &textureName, const std::string &animationName, float fps, const std::function<void(std::shared_ptr<GameObject> other)> &callback);
    virtual ~TriggerObject() = default;

protected:
    inline virtual void update() override {};
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override { this->callback(other); }

    const std::function<void(std::shared_ptr<GameObject> other)> callback;
};