#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class VirtualTransformable : private sf::Transformable
{
public:
    VirtualTransformable() = default;
    virtual ~VirtualTransformable() = default;

    virtual inline void setPositionV(float x, float y) { setPosition(x, y); }
    virtual inline void setPositionV(const sf::Vector2f &position) { setPosition(position); }
    virtual inline void setRotationV(float angle) { setRotation(angle); }
    virtual inline void setScaleV(float factorX, float factorY) { setScale(factorX, factorY); }
    virtual inline void setScaleV(const sf::Vector2f &factors) { setScale(factors); }
    virtual inline void setOriginV(float x, float y) { setOrigin(x, y); }
    virtual inline void setOriginV(const sf::Vector2f &origin) { setOrigin(origin); }
    const virtual inline sf::Vector2f &getPositionV() const { return getPosition(); }
    virtual inline float getRotationV() const { return getRotation(); }
    const virtual inline sf::Vector2f &getScaleV() const { return getScale(); }
    const virtual inline sf::Vector2f &getOriginV() const { return getOrigin(); }
    virtual inline void moveV(float offsetX, float offsetY) { move(offsetX, offsetY); }
    virtual inline void moveV(const sf::Vector2f &offset) { move(offset); }
    virtual inline void rotateV(float angle) { rotate(angle); }
    virtual inline void scaleV(float factorX, float factorY) { scale(factorX, factorY); }
    virtual inline void scaleV(const sf::Vector2f &factor) { scale(factor); }
    const virtual inline sf::Transform &getTransformV() const { return getTransform(); }
    const virtual inline sf::Transform &getInverseTransformV() const { return getInverseTransform(); }
};