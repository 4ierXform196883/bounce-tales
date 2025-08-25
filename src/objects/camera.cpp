#include "camera.hpp"
#include <iostream>
#include <cmath>

#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))

Camera::Camera(const sf::Vector2f &size, std::shared_ptr<GameObject> followObject)
    : GameObject("camera"), followObject(followObject)
{
    view.setSize(size);
}

void Camera::setZoom(float factor)
{
    sf::Vector2f unzoomedSize = view.getSize() * (1.0f / zoom);
    zoom = factor;
    view.setSize(unzoomedSize * zoom);
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