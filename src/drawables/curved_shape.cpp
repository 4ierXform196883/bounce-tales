#include "curved_shape.hpp"

// CurvedShape::CurvedShape(const sf::Texture &texture) : texture(&texture), verts{sf::TrianglesStrip, 4}
// {
//     sf::Vector2u tSize = texture.getSize();
//     this->setTextureRect({0, 0, (int)tSize.x, (int)tSize.y});
// }

// CurvedShape::CurvedShape(const sf::Texture &texture, const sf::IntRect &rect) : texture(&texture), verts{sf::TrianglesStrip, 4}
// {
//     this->setTextureRect(rect);
// }

CurvedShape::CurvedShape(const sf::Texture &texture, const std::vector<sf::Vertex> &verts, sf::VertexBuffer::Usage usage)
    : texture(&texture), verts{sf::PrimitiveType::TriangleStrip, usage}
{
    this->verts.create(verts.size());
    this->verts.update(&verts[0]);
    float bounds[4];
    for (const sf::Vertex& vert : verts)
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
