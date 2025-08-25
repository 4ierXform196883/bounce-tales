#pragma once

#include "game_object.hpp"

class Camera : public GameObject
{
public:
    Camera(const sf::Vector2f &size, std::shared_ptr<GameObject> followObject = nullptr);
    virtual ~Camera() = default;

    inline std::shared_ptr<GameObject> getFollowObject() const { return followObject; }
    inline void setFollowObject(std::shared_ptr<GameObject> followObject) { this->followObject = followObject; }
    inline const sf::View &getView() const { return view; }
    void setZoom(float factor);
    inline float getZoom() const { return zoom; }

protected:
    virtual void update() override;
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};

    std::shared_ptr<GameObject> followObject;
    sf::View view;
    float zoom = 1.0f;
};