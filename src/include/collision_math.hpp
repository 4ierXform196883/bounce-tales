#pragma once

#include <cmath>
#include <limits>
#include <SFML/Graphics.hpp>
#include "collidable.hpp"

namespace collision_math
{
    struct Ray
    {
        sf::Vector2f origin;
        sf::Vector2f direction;
    };

    struct SupportPoint
    {
        sf::Vector2f position;
        sf::Vector2f originA;
        sf::Vector2f originB;
        sf::Vector2f dir;
    };

    inline float dot(const sf::Vector2f &v1, const sf::Vector2f &v2) { return v1.x * v2.x + v1.y * v2.y; }
    inline float dot(const sf::Vector3f &v1, const sf::Vector3f &v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
    inline float lengthSquared(const sf::Vector2f &v) { return v.x * v.x + v.y * v.y; }
    inline float length(const sf::Vector2f &v) { return std::sqrt(lengthSquared(v)); }
    inline float cross(const sf::Vector2f &v1, const sf::Vector2f &v2) { return v1.x * v2.y - v1.y * v2.x; }
    inline sf::Vector3f cross(const sf::Vector3f &v1, const sf::Vector3f &v2) { return sf::Vector3f(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); }
    inline int mod(int i, int base) { return ((i % base) + base) % base; }
    inline float angle(const sf::Vector2f &v1, const sf::Vector2f &v2) { return atan2(cross(v1, v2), dot(v1, v2)); }
    inline sf::Vector2f normalize(const sf::Vector2f &vec) { return vec / length(vec); }
    inline bool isSameDirection(const sf::Vector2f &a, const sf::Vector2f &b) { return dot(a, b) > 0; }
    inline bool isSameDirection(const sf::Vector3f &a, const sf::Vector3f &b) { return dot(a, b) > 0; }

    bool isSameSideOfLine(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &p1, const sf::Vector2f &p2);
    bool isInTriangle(const sf::Vector2f &p0, const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p);
    bool isLineLineIntersection(const sf::Vector2f &vA, const sf::Vector2f &vB, const sf::Vector2f &uA, const sf::Vector2f &uB, sf::Vector2f &intersect_point);
    bool isRayLineIntersection(const Ray &ray, const sf::Vector2f &a, const sf::Vector2f &b);
    bool isReflex(const sf::Vector2f &p, const sf::Vector2f &prev, const sf::Vector2f &next, bool counter_clockwise = true);

    std::vector<sf::Vector2f> invertShape(const std::vector<sf::Vector2f> &vertices, const sf::Vector2f &origin);

    SupportPoint getSupportPoint(const Hitbox &shapeA, const Hitbox &shapeB, const sf::Vector2f &dir);
    SupportPoint getConvexConvexSupportPoint(const std::vector<sf::Vector2f> &verticesA, const std::vector<sf::Vector2f> &verticesB, const sf::Vector2f &dir);
    SupportPoint getCircleCircleSupportPoint(const sf::Vector2f &centerA, float radiusA, const sf::Vector2f &centerB, float radiusB, const sf::Vector2f &dir);
    SupportPoint getCircleConvexSupportPoint(const sf::Vector2f &centerA, float radiusA, const std::vector<sf::Vector2f> &verticesB, const sf::Vector2f &dir);

}
