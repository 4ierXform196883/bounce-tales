#pragma once

#include "game_object.hpp"
#include "primitive_sprite.hpp"

class BackgroundObject : public GameObject
{
public:
    BackgroundObject(const std::string &tag, std::shared_ptr<PrimitiveSprite> sprite, const sf::Vector2f &pos, float speed, float distance);
    virtual ~BackgroundObject() = default;

protected:
    virtual void update() override;
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};

    float speed;
    float distance;
    sf::Vector2f prevCPos;
    const sf::Vector2u tSize;
};