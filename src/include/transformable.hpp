#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Transformable
{
public:
    virtual void setPosition(float x, float y);
    virtual void setPosition(const sf::Vector2f &position);
    virtual void setRotation(float angle);
    virtual void setScale(float factorX, float factorY);
    virtual void setScale(const sf::Vector2f &factors);
    virtual void setOrigin(float x, float y);
    virtual void setOrigin(const sf::Vector2f &origin);
    virtual void move(float offsetX, float offsetY);
    virtual void move(const sf::Vector2f &offset);
    virtual void rotate(float angle);
    virtual void scale(float factorX, float factorY);
    virtual void scale(const sf::Vector2f &factor);
    inline virtual const sf::Vector2f &getPosition() const { return trans.getPosition(); }
    inline virtual float getRotation() const { return trans.getRotation(); }
    inline virtual const sf::Vector2f &getScale() const { return trans.getScale(); }
    inline virtual const sf::Vector2f &getOrigin() const { return trans.getOrigin(); }
    inline virtual const sf::Transform &getTransform() const { return trans.getTransform(); }
    inline virtual const sf::Transform &getInverseTransform() const { return trans.getInverseTransform(); }

private:
    sf::Transformable trans;
};