#include "primitive_rectangle.hpp"

PrimitiveRectangle::PrimitiveRectangle()
{
    rectangle.setPrimitiveType(sf::Quads);
    rectangle.resize(4);
    outline.setPrimitiveType(sf::LineStrip);
    outline.resize(5);
    setSize({0, 0});
}

PrimitiveRectangle::PrimitiveRectangle(const sf::Vector2f &size, const sf::Color &color, float outlineThickness, const sf::Color &outlineColor)
    : outlineThickness(outlineThickness), fillColor(color), outlineColor(outlineColor)
{
    rectangle.setPrimitiveType(sf::Quads);
    rectangle.resize(4);
    outline.setPrimitiveType(sf::LineStrip);
    outline.resize(5);
    setSize(size);
}

void PrimitiveRectangle::setSize(const sf::Vector2f &size)
{
    rectangle.clear();
    rectangle.resize(4);
    outline.clear();
    outline.resize(5);

    rectangle[0].position = {0, 0};
    rectangle[1].position = {size.x, 0};
    rectangle[2].position = {size.x, size.y};
    rectangle[3].position = {0, size.y};

    outline[0].position = {0, 0};
    outline[1].position = {size.x, 0};
    outline[2].position = {size.x, size.y};
    outline[3].position = {0, size.y};
    outline[4].position = {0, 0};

    for (size_t i = 0; i < 4; i++)
        rectangle[i].color = fillColor;
    for (size_t i = 0; i < 5; i++)
        outline[i].color = outlineColor;

    this->size = size;
}

void PrimitiveRectangle::setColor(const sf::Color &color)
{
    fillColor = color;
    for (size_t i = 0; i < 4; i++)
        rectangle[i].color = fillColor;
}

void PrimitiveRectangle::setOutlineColor(const sf::Color &color)
{
    outlineColor = color;
    for (size_t i = 0; i < 5; i++)
        outline[i].color = outlineColor;
}

void PrimitiveRectangle::setOutlineThickness(float thickness)
{
    outlineThickness = thickness;
    setSize(getSize());
}

inline sf::FloatRect PrimitiveRectangle::getLocalBounds() const
{
    return sf::FloatRect(0, 0, rectangle[2].position.x, rectangle[2].position.y);
}

void PrimitiveRectangle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(rectangle, states);
    target.draw(outline, states);
}
