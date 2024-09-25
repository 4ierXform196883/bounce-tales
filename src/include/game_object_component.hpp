#pragma once

#include <memory>

class GameObject;

class GameObjectComponent
{
public:
    GameObjectComponent(std::shared_ptr<GameObject> gameObject) : gameObject(gameObject) {}
    virtual ~GameObjectComponent() = default;

protected:
    std::weak_ptr<GameObject> gameObject;
};