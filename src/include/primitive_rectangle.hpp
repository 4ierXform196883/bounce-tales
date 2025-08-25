#pragma once

#include "drawable.hpp"

class PrimitiveRectangle : public Drawable
{
public:
    PrimitiveRectangle();
    PrimitiveRectangle(const sf::Vector2f &size = {0, 0}, const sf::Color &color = sf::Color::White, float outlineThickness = 0.0f, const sf::Color &outlineColor = sf::Color::Black);
    
    virtual ~PrimitiveRectangle() = default;

    inline void setSize(const sf::Vector2f &size);
    inline const sf::Vector2f &getSize() const { return size; }

    virtual void setColor(const sf::Color &color) override;
    inline virtual const sf::Color &getColor() const override { return fillColor; }
    void setOutlineColor(const sf::Color &color);
    inline const sf::Color &getOutlineColor() const { return outlineColor; }
    void setOutlineThickness(float thickness);
    inline float getOutlineThickness() const { return outlineThickness; }

    inline virtual sf::FloatRect getLocalBounds() const override;
protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::Vector2f size;
    sf::VertexArray rectangle;
    sf::VertexArray outline;
    sf::Color fillColor, outlineColor;
    float outlineThickness = 0.0f;
};