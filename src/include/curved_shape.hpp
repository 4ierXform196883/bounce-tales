#pragma once

#include <SFML/Graphics.hpp>

class CurvedShape : public sf::Drawable
{
public:
    CurvedShape() = default;
    CurvedShape(const sf::Texture& texture, const std::vector<sf::Vertex>& verts, sf::VertexBuffer::Usage usage = sf::VertexBuffer::Static);
    virtual ~CurvedShape() = default;

    void setTexture(const sf::Texture& texture);
    const sf::Texture* getTexture() const;
    const sf::FloatRect& getBounds() const;

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::FloatRect bounds;
    sf::VertexBuffer verts;
    const sf::Texture* texture;
};