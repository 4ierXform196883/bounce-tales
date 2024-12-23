#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "collision_math.hpp"
#include "collidable.hpp"

namespace collision_calculator
{
    struct Edge
    {
        sf::Vector2f a;
        sf::Vector2f b;
        sf::Vector2f normal;
        int index;
        float distance;
    };

    std::vector<collision_math::SupportPoint> getGJKCollisionSimplex(const Hitbox &shapeA, const Hitbox &shapeB);
    Edge findClosestEdge(const std::vector<collision_math::SupportPoint> &simplex);
    sf::Vector2f getPenetrationVector(const Hitbox &shapeA, const Hitbox &shapeB, const std::vector<collision_math::SupportPoint> &simplex);
}