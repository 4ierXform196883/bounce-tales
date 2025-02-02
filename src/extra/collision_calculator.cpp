#include "collision_calculator.hpp"

bool doSimplexEdge(int aIndex, int bIndex, std::vector<collision_math::SupportPoint> &simplex, sf::Vector2f &dir)
{
    sf::Vector3f newDir;
    collision_math::SupportPoint a = simplex[aIndex];
    collision_math::SupportPoint b = simplex[bIndex];
    sf::Vector2f ab = b.position - a.position;
    std::vector<collision_math::SupportPoint> newSimplex;
    if (collision_math::isSameDirection(ab, -a.position))
    {
        newDir = collision_math::cross(collision_math::cross(sf::Vector3f(ab.x, ab.y, 0), -sf::Vector3f(a.position.x, a.position.y, 0)), sf::Vector3f(ab.x, ab.y, 0));
        dir.x = newDir.x;
        dir.y = newDir.y;

        newSimplex.push_back(b);
        newSimplex.push_back(a);
        simplex = newSimplex;

        return true;
    }
    else
    {
        dir = -a.position;
        newSimplex.push_back(a);
        simplex = newSimplex;

        return false;
    }
}

bool doSimplex(std::vector<collision_math::SupportPoint> &simplex, sf::Vector2f &dir)
{
    sf::Vector3f newDir;
    if (simplex.size() == 1)
    {
        dir = -simplex[0].position;
    }
    else if (simplex.size() == 2)
    {
        doSimplexEdge(1, 0, simplex, dir);
    }
    else
    {
        // Triangle
        sf::Vector3f a = sf::Vector3f(simplex[2].position.x, simplex[2].position.y, 0);
        sf::Vector3f b = sf::Vector3f(simplex[1].position.x, simplex[1].position.y, 0);
        sf::Vector3f c = sf::Vector3f(simplex[0].position.x, simplex[0].position.y, 0);

        sf::Vector3f ac = c - a;
        sf::Vector3f ab = b - a;

        sf::Vector3f abc = collision_math::cross(ab, ac);

        // If AC is closest to origin
        if (collision_math::isSameDirection(collision_math::cross(abc, ac), -a))
        {
            if (collision_math::isSameDirection(ac, -a))
            {
                newDir = collision_math::cross(collision_math::cross(ac, -a), ac);
                dir.x = newDir.x;
                dir.y = newDir.y;

                std::swap(simplex[1], simplex[2]);
                simplex.pop_back();
            }
            else
            {
                doSimplexEdge(2, 1, simplex, dir);
            }
        }
        else
        {
            // If AB is closest to origin
            if (collision_math::isSameDirection(collision_math::cross(ab, abc), -a))
            {
                doSimplexEdge(2, 1, simplex, dir);
            }
            else
                return true;
        }
    }
    return false;
}

std::vector<collision_math::SupportPoint> collision_calculator::getGJKCollisionSimplex(const Hitbox &shapeA, const Hitbox &shapeB)
{
    sf::Vector2f dir;
    collision_math::SupportPoint nextSupportPoint;

    std::vector<collision_math::SupportPoint> simplex;

    float radius = shapeA.index() == 0 ? std::get<CircleHitbox>(shapeA).radius : 0;
    dir = shapeA.index() == 0 ? sf::Vector2f(radius, radius) : std::get<TriangleHitbox>(shapeA).points[0];
    nextSupportPoint = collision_math::getSupportPoint(shapeA, shapeB, dir);
    simplex.push_back(nextSupportPoint);

    dir = dir * -1.f;

    while (simplex.size() < 100)
    {
        nextSupportPoint = collision_math::getSupportPoint(shapeA, shapeB, dir);
        // No intersection
        if (collision_math::dot(nextSupportPoint.position, dir) <= 0)
            return std::vector<collision_math::SupportPoint>();
        simplex.push_back(nextSupportPoint);
        if (doSimplex(simplex, dir))
            break;
    }

    return simplex;
}

collision_calculator::Edge collision_calculator::findClosestEdge(const std::vector<collision_math::SupportPoint> &simplex)
{

    Edge result;
    int nextI;
    float minDist = std::numeric_limits<float>::max(), currDist;
    sf::Vector2f norm, edge;
    sf::Vector3f norm3d;
    for (int i = 0; i < simplex.size(); ++i)
    {

        nextI = (i + 1) % simplex.size();

        edge = simplex[nextI].position - simplex[i].position;
        norm3d = collision_math::cross(collision_math::cross(sf::Vector3f(edge.x, edge.y, 0), sf::Vector3f(simplex[i].position.x, simplex[i].position.y, 0)), sf::Vector3f(edge.x, edge.y, 0));
        norm.x = norm3d.x;
        norm.y = norm3d.y;

        norm = collision_math::normalize(norm);
        currDist = collision_math::dot(norm, simplex[i].position);
        if (currDist < minDist)
        {
            minDist = currDist;
            result.a = simplex[i].position;
            result.b = simplex[nextI].position;
            result.normal = norm;
            result.distance = currDist;
            result.index = i;
        }
    }

    return result;
}

sf::Vector2f collision_calculator::getPenetrationVector(const Hitbox &shapeA, const Hitbox &shapeB, const std::vector<collision_math::SupportPoint> &simplex)
{
    std::vector<collision_math::SupportPoint> newSimplex = simplex;
    collision_math::SupportPoint nextSuppPoint;
    if (newSimplex.size() == 1)
    {
        const sf::Vector2f searchDirs[] =
            {
                sf::Vector2f(1.0f, 0.0f),
                sf::Vector2f(-1.0f, 0.0f),
                sf::Vector2f(0.0f, 1.0f),
                sf::Vector2f(0.0f, -1.0f)};

        for (int i = 0; i < 4; ++i)
        {
            nextSuppPoint = collision_math::getSupportPoint(shapeA, shapeB, searchDirs[i]);

            if (collision_math::lengthSquared(nextSuppPoint.position - newSimplex[0].position) >= 0.0001f)
            {
                newSimplex.push_back(nextSuppPoint);

                break;
            }
        }
    }

    if (newSimplex.size() == 2)
    {
        sf::Vector2f ab = newSimplex[0].position - newSimplex[1].position;
        sf::Vector3f searchDir3 = collision_math::cross(collision_math::cross(sf::Vector3f(ab.x, ab.y, 0), -sf::Vector3f(newSimplex[1].position.x, newSimplex[1].position.y, 0)), sf::Vector3f(ab.x, ab.y, 0));
        sf::Vector2f searchDir;
        searchDir.x = searchDir3.x;
        searchDir.y = searchDir3.y;
        nextSuppPoint = collision_math::getSupportPoint(shapeA, shapeB, searchDir);

        if (collision_math::lengthSquared(nextSuppPoint.position - newSimplex[0].position) < 0.0001f)
        {
            nextSuppPoint = collision_math::getSupportPoint(shapeA, shapeB, -searchDir);
        }
        newSimplex.push_back(nextSuppPoint);
    }

    Edge e;
    while (newSimplex.size() < 100)
    {
        e = findClosestEdge(newSimplex);
        nextSuppPoint = collision_math::getSupportPoint(shapeA, shapeB, e.normal);

        double d = collision_math::dot(nextSuppPoint.position, e.normal);

        if (d - e.distance < 0.0001f)
            break;
        else
            newSimplex.insert(newSimplex.begin() + e.index + 1, nextSuppPoint);
    }

    sf::Vector2f lineVec = e.a - e.b;
    sf::Vector2f projO = collision_math::dot(lineVec, -e.b) * lineVec / collision_math::lengthSquared(lineVec);
    float s, t;
    s = sqrt(collision_math::lengthSquared(projO) / collision_math::lengthSquared(lineVec));
    t = 1 - s;
    int nextI = (e.index + 1) % newSimplex.size();
    sf::Vector2f posA = s * newSimplex[e.index].originA + t * newSimplex[nextI].originA;
    sf::Vector2f posB = s * newSimplex[e.index].originB + t * newSimplex[nextI].originB;

    return posA - posB;
}
