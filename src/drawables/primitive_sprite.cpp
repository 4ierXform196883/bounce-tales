#include "primitive_sprite.hpp"

PrimitiveSprite::PrimitiveSprite(const sf::Texture &texture) : texture(&texture), verts{sf::TrianglesStrip, 4}
{
    sf::Vector2u tSize = texture.getSize();
    this->setTextureRect({0, 0, (int)tSize.x, (int)tSize.y});
}

PrimitiveSprite::PrimitiveSprite(const sf::Texture &texture, const sf::IntRect &rect) : texture(&texture), verts{sf::TrianglesStrip, 4}
{
    this->setTextureRect(rect);
}

void PrimitiveSprite::setTextureRect(const sf::IntRect &rect)
{
    this->textureRect = rect;
    verts[0].position = {0, 0};
    verts[1].position.x = rect.width;
    verts[2].position.y = rect.height;
    verts[3].position = {(float)rect.width, (float)rect.height};
    verts[0].texCoords = {(float)rect.left, (float)rect.top};
    verts[1].texCoords = {(float)rect.left + rect.width, (float)rect.top};
    verts[2].texCoords = {(float)rect.left, (float)rect.top + rect.height};
    verts[3].texCoords = {(float)rect.left + rect.width, (float)rect.top + rect.height};
}

const sf::IntRect &PrimitiveSprite::getTextureRect() const
{
    return this->textureRect;
}

void PrimitiveSprite::setTexture(const sf::Texture &texture, bool resetRect)
{
    this->texture = &texture;
    if (resetRect)
    {
        sf::Vector2u tSize = texture.getSize();
        this->setTextureRect({0, 0, (int)tSize.x, (int)tSize.y});
    }
}

const sf::Texture *PrimitiveSprite::getTexture() const
{
    return this->texture;
}

void PrimitiveSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.texture = texture;
    target.draw(verts, states);
}
