#include "earcut.hpp"

std::vector<sf::Vector2f> mapbox::earcutShape(const std::vector<sf::Vector2f> &verts)
{
    std::vector<std::array<double, 2>> stdVerts(verts.size());
    for (size_t i = 0; i < verts.size(); ++i)
    {
        stdVerts[i] = {(float)verts[i].x, (float)verts[i].y};
    }
    std::vector<std::vector<std::array<double, 2>>> polygon{std::move(stdVerts)};
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    std::vector<sf::Vector2f> outVerts(indices.size());
    for (size_t i = 0; i < indices.size(); ++i)
    {
        outVerts[i] = {(float)polygon[0][indices[i]][0], (float)polygon[0][indices[i]][1]};
    }

    return outVerts;
}

ConcaveHitbox mapbox::earcutHitbox(const std::vector<sf::Vector2f> &verts)
{
    std::vector<std::array<double, 2>> stdVerts(verts.size());
    for (size_t i = 0; i < verts.size(); ++i)
    {
        stdVerts[i] = {(float)verts[i].x, (float)verts[i].y};
    }
    std::vector<std::vector<std::array<double, 2>>> polygon{std::move(stdVerts)};
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    ConcaveHitbox hitbox{std::vector<ConvexHitbox>(indices.size() / 3)};
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        hitbox.triangles[i / 3] = ConvexHitbox{{{(float)polygon[0][indices[i]][0], (float)polygon[0][indices[i]][1]},
                                                {(float)polygon[0][indices[i + 1]][0], (float)polygon[0][indices[i + 1]][1]},
                                                {(float)polygon[0][indices[i + 2]][0], (float)polygon[0][indices[i + 2]][1]}}};
    }
    return hitbox;
}

std::pair<std::vector<sf::Vector2f>, ConcaveHitbox> mapbox::earcutShapeAndHitbox(const std::vector<sf::Vector2f> &verts)
{
    std::vector<std::array<double, 2>> stdVerts(verts.size());
    for (size_t i = 0; i < verts.size(); ++i)
    {
        stdVerts[i] = {(float)verts[i].x, (float)verts[i].y};
    }
    std::vector<std::vector<std::array<double, 2>>> polygon{std::move(stdVerts)};
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    std::vector<sf::Vector2f> outVerts(indices.size());
    ConcaveHitbox hitbox{std::vector<ConvexHitbox>(indices.size() / 3)};
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        outVerts[i] = {(float)polygon[0][indices[i]][0], (float)polygon[0][indices[i]][1]};
        outVerts[i + 1] = {(float)polygon[0][indices[i + 1]][0], (float)polygon[0][indices[i + 1]][1]};
        outVerts[i + 2] = {(float)polygon[0][indices[i + 2]][0], (float)polygon[0][indices[i + 2]][1]};
        hitbox.triangles[i / 3] = ConvexHitbox{{{(float)polygon[0][indices[i]][0], (float)polygon[0][indices[i]][1]},
                                                {(float)polygon[0][indices[i + 1]][0], (float)polygon[0][indices[i + 1]][1]},
                                                {(float)polygon[0][indices[i + 2]][0], (float)polygon[0][indices[i + 2]][1]}}};
    }
    return {outVerts, hitbox};
}