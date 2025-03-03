#pragma once

#include "game_object.hpp"

class Platform : public GameObject, public ICollidable, public IPhysical
{
public:
    COLLIDABLE
    PHYSICAL

    Platform(const std::vector<sf::Vector2f> &verts, const std::string& textureName, const std::vector<sf::Vector2f> &path, float speedMult = 1);
    virtual ~Platform() = default;

protected:
    virtual void update() override;
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};

    size_t cp = 0;
    float speed, speedMult;
    std::vector<sf::Vector2f> path;
};