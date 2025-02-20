#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class ITransformable
{
public:
    virtual void setPosition(float x, float y) = 0;
    virtual void setPosition(const sf::Vector2f &position) = 0;
    virtual void setRotation(float angle) = 0;
    virtual void setScale(float factorX, float factorY) = 0;
    virtual void setScale(const sf::Vector2f &factors) = 0;
    virtual void setOrigin(float x, float y) = 0;
    virtual void setOrigin(const sf::Vector2f &origin) = 0;
    virtual void move(float offsetX, float offsetY) = 0;
    virtual void move(const sf::Vector2f &offset) = 0;
    virtual void rotate(float angle) = 0;
    virtual void scale(float factorX, float factorY) = 0;
    virtual void scale(const sf::Vector2f &factor) = 0;
    virtual const sf::Vector2f &getPosition() const = 0;
    virtual float getRotation() const = 0;
    virtual const sf::Vector2f &getScale() const = 0;
    virtual const sf::Vector2f &getOrigin() const = 0;
    virtual const sf::Transform &getTransform() const = 0;
    virtual const sf::Transform &getInverseTransform() const = 0;
};