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

class Transformable : public ITransformable
{
public:
    inline virtual void setPosition(float x, float y) override { return trans.setPosition(x, y); }
    inline virtual void setPosition(const sf::Vector2f &position) override { return trans.setPosition(position); }
    inline virtual void setRotation(float angle) override { return trans.setRotation(angle); }
    inline virtual void setScale(float factorX, float factorY) override { return trans.setScale(factorX, factorY); }
    inline virtual void setScale(const sf::Vector2f &factors) override { return trans.setScale(factors); }
    inline virtual void setOrigin(float x, float y) override { return trans.setOrigin(x, y); }
    inline virtual void setOrigin(const sf::Vector2f &origin) override { return trans.setOrigin(origin); }
    inline virtual void move(float offsetX, float offsetY) override { return trans.move(offsetX, offsetY); }
    inline virtual void move(const sf::Vector2f &offset) override { return trans.move(offset); }
    inline virtual void rotate(float angle) override { return trans.rotate(angle); }
    inline virtual void scale(float factorX, float factorY) override { return trans.scale(factorX, factorY); }
    inline virtual void scale(const sf::Vector2f &factor) override { return trans.scale(factor); }
    inline virtual const sf::Vector2f &getPosition() const override { return trans.getPosition(); }
    inline virtual float getRotation() const override { return trans.getRotation(); }
    inline virtual const sf::Vector2f &getScale() const override { return trans.getScale(); }
    inline virtual const sf::Vector2f &getOrigin() const override { return trans.getOrigin(); }
    inline virtual const sf::Transform &getTransform() const override { return trans.getTransform(); }
    inline virtual const sf::Transform &getInverseTransform() const override { return trans.getInverseTransform(); }

    sf::Transformable trans;
};