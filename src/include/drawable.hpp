#pragma once

#include <SFML/Graphics.hpp>

class Drawable : public sf::Drawable
{
public:
    virtual const sf::Color& getColor() const = 0;
    virtual void setColor(const sf::Color& color) = 0;
    virtual sf::FloatRect getLocalBounds() const = 0;
};