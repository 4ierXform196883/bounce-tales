#pragma once

#include "game_object.hpp"

class Camera : public GameObject
{
public:
    Camera(const sf::Vector2f &size, std::shared_ptr<IGameObject> followObject = nullptr);
    virtual ~Camera() = default;

    virtual void update() override;

    void setFollowObject(std::shared_ptr<IGameObject> followObject);
    inline const sf::View &getView() const { return view; }

private:
    std::shared_ptr<IGameObject> followObject;
    sf::View view;
};