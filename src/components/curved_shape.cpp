#include "curved_shape.hpp"
#include <iostream>

CurvedShape::CurvedShape(const sf::Texture &texture, const std::vector<sf::Vector2f> &vertices)
    : texture(&texture), verts{sf::PrimitiveType::Triangles}
{
    std::vector<sf::Vertex> triangleVertices;
    size_t i = 0;
    for (const sf::Vector2f &vert : vertices)
    {
        triangleVertices.emplace_back(vert, vert);
        ++i;
    }
    this->verts.resize(triangleVertices.size());
    for (size_t j = 0; j < triangleVertices.size(); ++j)
        this->verts[j] = triangleVertices[j];
}

void CurvedShape::setTexture(const sf::Texture &texture)
{
    this->texture = &texture;
}

const sf::Texture *CurvedShape::getTexture() const
{
    return this->texture;
}

void CurvedShape::setColor(const sf::Color &color)
{
    this->color = color;
    for (size_t i = 0; i < verts.getVertexCount(); ++i)
        verts[i].color = color;
}

void CurvedShape::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.texture = texture;
    target.draw(verts, states);
}
