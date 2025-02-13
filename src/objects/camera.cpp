#include "camera.hpp"
#include <iostream>
#include <cmath>

#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))

Camera::Camera(const sf::Vector2f &size, std::shared_ptr<IGameObject> followObject)
    : GameObject("camera", nullptr), followObject(followObject)
{
    view.setSize(size);
}

void Camera::update()
{
    sf::Vector2f force = followObject->getPosition() - this->getPosition();
    this->move(0.1f * force);
    view.setCenter(this->getPosition());
}

void Camera::setFollowObject(std::shared_ptr<IGameObject> followObject)
{
    this->followObject = followObject;
}
