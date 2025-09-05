#pragma once

#include <SFML/Graphics.hpp>
#include "drawable.hpp"

class PrimitiveSprite : public IDrawable
{
public:
    PrimitiveSprite() = default;
    PrimitiveSprite(const sf::Texture &texture);
    PrimitiveSprite(const sf::Texture &texture, const sf::IntRect &rect);
    virtual ~PrimitiveSprite() = default;

    void setTextureRect(const sf::IntRect &rect);
    const sf::IntRect &getTextureRect() const;

    void setTexture(const sf::Texture &texture, bool resetRect = true);
    const sf::Texture *getTexture() const;

    virtual void setColor(const sf::Color &color) override;
    virtual const sf::Color &getColor() const override;

    inline virtual sf::FloatRect getLocalBounds() const override
    {
        return sf::FloatRect(0, 0, (float)textureRect.width, (float)textureRect.height);
    }

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::VertexArray verts;
    sf::IntRect textureRect;
    const sf::Texture *texture;
};
