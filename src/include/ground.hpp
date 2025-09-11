#pragma once

#include "game_object.hpp"

class Ground : public GameObject, public ICollidable
{
  public:
    COLLIDABLE

      Ground(const std::vector<sf::Vector2f> &verts, const std::string& textureName, bool bezierVerts = false);
      Ground(const std::vector<sf::Vector2f> &verts, const sf::Color& color, bool bezierVerts = false);
    virtual ~Ground() = default;

  protected:
    inline virtual void update() override {};
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};


    std::vector<sf::Vector2f> getSplineVerts(const std::vector<sf::Vector2f> rawVerts);
};
