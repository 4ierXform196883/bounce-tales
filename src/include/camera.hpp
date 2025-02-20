#pragma once

#include "game_object.hpp"

class Camera : public GameObject
{
public:
    Camera(const sf::Vector2f &pos, const sf::Vector2f &size, std::shared_ptr<GameObject> followObject = nullptr);
    virtual ~Camera() = default;

    void setFollowObject(std::shared_ptr<GameObject> followObject);
    inline const sf::View &getView() const { return view; }

protected:
    virtual void update() override;
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};

    std::shared_ptr<GameObject> followObject;
    sf::View view;
};