#include "camera.hpp"
#include <iostream>
#include <cmath>

#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))

Camera::Camera(const sf::Vector2f &pos, const sf::Vector2f &size, std::shared_ptr<GameObject> followObject)
    : GameObject("camera"), followObject(followObject)
{
    this->setPosition(pos);
    view.setSize(size);
}

void Camera::update()
{
    if (followObject)
    {
        sf::Vector2f force = followObject->getPosition() - this->getPosition();
        this->move(0.1f * force);
    }
    view.setCenter(this->getPosition());
}

void Camera::setFollowObject(std::shared_ptr<GameObject> followObject)
{
    this->followObject = followObject;
}
