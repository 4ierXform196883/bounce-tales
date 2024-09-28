#pragma once

#include "game_object.hpp"

class DistantObject : public GameObject
{
public:
    DistantObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable,const sf::Vector2f& pos, float speed);
    virtual ~DistantObject() = default;

protected:
    virtual void update() override;

    float speed;
    const sf::Vector2f initPos;
};