#pragma once

#include <SFML/Graphics.hpp>
#include "drawable.hpp"

class CurvedShape : public IDrawable
{
public:
    CurvedShape() = default;
    CurvedShape(const sf::Texture &texture, const std::vector<sf::Vector2f> &vertices);
    virtual ~CurvedShape() = default;

    void setTexture(const sf::Texture &texture);

    const sf::Texture *getTexture() const;
    inline virtual const sf::Color &getColor() const override { return color; }
    virtual void setColor(const sf::Color &color) override;
    inline virtual sf::FloatRect getLocalBounds() const override { return verts.getBounds(); }

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::VertexArray verts;
    sf::Color color = sf::Color::White;
    const sf::Texture *texture;
};
