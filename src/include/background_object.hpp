#pragma once

#include "game_object.hpp"

class BackgroundObject : public GameObject
{
public:
    BackgroundObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable,const sf::Vector2f& pos, float speed);
    virtual ~BackgroundObject() = default;

    virtual void update() override;
    virtual void onCollision(std::shared_ptr<const GameObject> other) override {};

protected:
    float speed;
    const sf::Vector2f initPos;
};