#include "curved_shape.hpp"
#include <iostream>

CurvedShape::CurvedShape(const sf::Texture &texture, const std::vector<sf::Vector2f> &vertices, sf::VertexBuffer::Usage usage)
    : texture(&texture), verts{sf::PrimitiveType::Triangles, usage}
{
    std::vector<sf::Vertex> triangleVertices;
    size_t i = 0;
    for (const sf::Vector2f &vert : vertices)
    {
        triangleVertices.emplace_back(vert, vert);
        ++i;
    }
    this->verts.create(triangleVertices.size());
    this->verts.update(&triangleVertices[0]);
    float bounds[4];
    for (const sf::Vertex &vert : triangleVertices)
    {
        bounds[0] = std::min(bounds[0], vert.position.x);
        bounds[1] = std::min(bounds[1], vert.position.y);
        bounds[2] = std::max(bounds[2], vert.position.x);
        bounds[3] = std::max(bounds[3], vert.position.y);
    }
    this->bounds = sf::FloatRect(bounds[0], bounds[1], bounds[2] - bounds[0], bounds[3] - bounds[1]);
}

void CurvedShape::setTexture(const sf::Texture &texture)
{
    this->texture = &texture;
}

const sf::Texture *CurvedShape::getTexture() const
{
    return this->texture;
}

const sf::FloatRect &CurvedShape::getBounds() const
{
    return this->bounds;
}

void CurvedShape::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.texture = texture;
    target.draw(verts, states);
}
