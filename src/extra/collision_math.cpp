#include "collision_math.hpp"

bool collision_math::isSameSideOfLine(const sf::Vector2f &a, const sf::Vector2f &b, const sf::Vector2f &p1, const sf::Vector2f &p2)
{
    float cross_p1 = cross(b - a, p1 - a);
    float cross_p2 = cross(b - a, p2 - a);
    return (cross_p1 < 0) != (cross_p2 < 0);
}

bool collision_math::isInTriangle(const sf::Vector2f &p0, const sf::Vector2f &p1, const sf::Vector2f &p2, const sf::Vector2f &p)
{
    float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
    float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;
    if ((s < 0) != (t < 0))
        return false;

    float area = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;
    if (area < 0.0f)
    {
        s = -s;
        t = -t;
        area = -area;
    }
    return s > 0 && t > 0 && (s + t) <= area;
}

bool collision_math::isLineLineIntersection(const sf::Vector2f &vA, const sf::Vector2f &vB, const sf::Vector2f &uA, const sf::Vector2f &uB, sf::Vector2f &intersect_point)
{
    sf::Vector2f v_dir = vB - vA;
    sf::Vector2f u_dir = uB - uA;
    float dir_cross = cross(v_dir, u_dir);
    sf::Vector2f origin_diff = uA - vA;

    if (dir_cross == 0 && cross(origin_diff, v_dir) == 0)
    {
        float v_dot = dot(v_dir, v_dir);
        float t0 = dot(origin_diff, v_dir) / v_dot;
        float t1 = t0 + (dot(u_dir, v_dir) / v_dot);

        if (t0 >= 0 && t0 <= 1)
        {
            intersect_point = vA + t0 * v_dir;
            return true;
        }
        else if (t1 >= 0 && t1 <= 1)
        {
            intersect_point = vA + t1 * v_dir;
            return true;
        }
        return false;
    }
    else
    {
        float t = cross(origin_diff, u_dir) / dir_cross;
        float u = cross(origin_diff, v_dir) / dir_cross;
        if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
        {
            intersect_point = vA + t * v_dir;
            return true;
        }
    }
    return false;
}

bool collision_math::isRayLineIntersection(const Ray &ray, const sf::Vector2f &a, const sf::Vector2f &b)
{
    sf::Vector2f line_dir = b - a;
    float dir_cross = cross(ray.direction, line_dir);
    if (dir_cross == 0)
        return false;

    sf::Vector2f origin_diff = a - ray.origin;
    float t = cross(origin_diff, line_dir) / dir_cross;
    float u = cross(origin_diff, ray.direction) / dir_cross;
    return t >= 0 && u >= 0 && u <= 1;
}

bool collision_math::isReflex(const sf::Vector2f &p, const sf::Vector2f &prev, const sf::Vector2f &next, bool counter_clockwise)
{
    sf::Vector2f vec1 = prev - p;
    sf::Vector2f vec2 = next - p;
    return angle(vec1, vec2) < 0;
}

std::vector<sf::Vector2f> collision_math::invertShape(const std::vector<sf::Vector2f> &vertices, const sf::Vector2f &origin)
{
    std::vector<sf::Vector2f> result;
    result.reserve(vertices.size());
    for (const auto &vertex : vertices)
    {
        result.push_back(2.f * origin - vertex);
    }
    return result;
}

collision_math::SupportPoint collision_math::getCircleConvexSupportPoint(const sf::Vector2f &centerA, float radiusA, const std::vector<sf::Vector2f> &verticesB, const sf::Vector2f &dir)
{

    SupportPoint result;
    result.dir = dir;

    sf::Vector2f norm_dir = normalize(dir);
    result.originA = centerA + norm_dir * radiusA;

    float max_dot = -std::numeric_limits<float>::max();
    for (const auto &vertex : verticesB)
    {
        float dot_product = dot(-dir, vertex);
        if (dot_product > max_dot)
        {
            max_dot = dot_product;
            result.originB = vertex;
        }
    }

    result.position = result.originA - result.originB;
    return result;
}

collision_math::SupportPoint collision_math::getSupportPoint(const Hitbox &shapeA, const Hitbox &shapeB, const sf::Vector2f &dir)
{
    if (shapeA.index() == 0 && shapeB.index() == 0)
    {
        auto firstHitbox = std::get<CircleHitbox>(shapeA);
        auto secondHitbox = std::get<CircleHitbox>(shapeB);
        return getCircleCircleSupportPoint(firstHitbox.center, firstHitbox.radius, secondHitbox.center, secondHitbox.radius, dir);
    }
    else if (shapeA.index() == 0 && shapeB.index() == 1)
    {
        auto firstHitbox = std::get<CircleHitbox>(shapeA);
        auto secondHitbox = std::get<ConcaveHitbox>(shapeB);
        return getCircleConvexSupportPoint(firstHitbox.center, firstHitbox.radius, secondHitbox.points, dir);
    }
    else if (shapeA.index() == 1 && shapeB.index() == 0)
    {
        auto firstHitbox = std::get<ConcaveHitbox>(shapeA);
        auto secondHitbox = std::get<CircleHitbox>(shapeB);
        auto res = getCircleConvexSupportPoint(secondHitbox.center, secondHitbox.radius, firstHitbox.points, dir);
        std::swap(res.originA, res.originB);
        res.position = -res.position;
        return res;
    }
    else if (shapeA.index() == 1 && shapeB.index() == 1)
    {
        auto firstHitbox = std::get<ConcaveHitbox>(shapeA);
        auto secondHitbox = std::get<ConcaveHitbox>(shapeB);
        return getConvexConvexSupportPoint(firstHitbox.points, secondHitbox.points, dir);
    }
    return SupportPoint();
}

collision_math::SupportPoint collision_math::getConvexConvexSupportPoint(const std::vector<sf::Vector2f> &verticesA, const std::vector<sf::Vector2f> &verticesB, const sf::Vector2f &dir)
{
    SupportPoint result;
    result.dir = dir;

    float max_dot = -std::numeric_limits<float>::max();
    for (const auto &vertex : verticesA)
    {
        float dot_product = dot(dir, vertex);
        if (dot_product > max_dot)
        {
            max_dot = dot_product;
            result.originA = vertex;
        }
    }

    max_dot = -std::numeric_limits<float>::max();
    for (const auto &vertex : verticesB)
    {
        float dot_product = dot(-dir, vertex);
        if (dot_product > max_dot)
        {
            max_dot = dot_product;
            result.originB = vertex;
        }
    }

    result.position = result.originA - result.originB;
    return result;
}

collision_math::SupportPoint collision_math::getCircleCircleSupportPoint(const sf::Vector2f &centerA, float radiusA, const sf::Vector2f &centerB, float radiusB, const sf::Vector2f &dir)
{
    SupportPoint result;
    result.dir = dir;

    sf::Vector2f norm_dir = normalize(dir);
    result.originA = centerA + norm_dir * radiusA;
    result.originB = centerB - norm_dir * radiusB;

    result.position = result.originA - result.originB;
    return result;
}
