#pragma once

#include <SFML/Graphics.hpp>

class PrimitiveSprite : public sf::Drawable
{
public:
    PrimitiveSprite() = default;
    PrimitiveSprite(const sf::Texture& texture);
    PrimitiveSprite(const sf::Texture& texture, const sf::IntRect& rect);
    virtual ~PrimitiveSprite() = default;

    void setTextureRect(const sf::IntRect& rect);
    const sf::IntRect& getTextureRect() const;

    void setTexture(const sf::Texture& texture, bool resetRect = true);
    const sf::Texture* getTexture() const;


protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::VertexArray verts;
    sf::IntRect textureRect;
    const sf::Texture* texture;
};

