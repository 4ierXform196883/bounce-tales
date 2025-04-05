#pragma once

#include "game_object.hpp"

class Door : public GameObject, public ICollidable
{
public:
    COLLIDABLE

    Door(const std::string &tag, const sf::Vector2f &startPos, float elevation);
    virtual ~Door() = default;

    bool isOpened = false;

    virtual void setScale(float factorX, float factorY) override;
    virtual void setScale(const sf::Vector2f &factors) override;
    virtual void scale(float factorX, float factorY) override;
    virtual void scale(const sf::Vector2f &factor) override;

protected:
    virtual void update() override;
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};

    const sf::Vector2f startPos;
    const float elevation;
};