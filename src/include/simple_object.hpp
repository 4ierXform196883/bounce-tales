#pragma once

#include "game_object.hpp"

class SimpleObject : public GameObject
{
public:
    SimpleObject(
        const std::string &tag,
        std::shared_ptr<sf::Drawable> drawable,
        const sf::Vector2f &pos = {0.0f, 0.0f},
        const sf::Vector2f &factors = {1.0f, 1.0f},
        float angle = 0);
    virtual ~SimpleObject() = default;

protected:
    inline virtual void update() override {};
};