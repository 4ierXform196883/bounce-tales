#include "transformable.hpp"

#include "sound_player.hpp"
#include "collidable.hpp"
#include "collidable.hpp"

void Transformable::setPosition(float x, float y)
{
    trans.setPosition(x, y);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::setPosition(const sf::Vector2f &position)
{
    trans.setPosition(position);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::setRotation(float angle)
{
    trans.setRotation(angle);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::setScale(float factorX, float factorY)
{
    trans.setScale(factorX, factorY);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::setScale(const sf::Vector2f &factors)
{
    trans.setScale(factors);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::setOrigin(float x, float y)
{
    trans.setOrigin(x, y);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::setOrigin(const sf::Vector2f &origin)
{
    trans.setOrigin(origin);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::move(float offsetX, float offsetY)
{
    trans.move(offsetX, offsetY);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::move(const sf::Vector2f &offset)
{
    trans.move(offset);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::rotate(float angle)
{
    trans.rotate(angle);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::scale(float factorX, float factorY)
{
    trans.scale(factorX, factorY);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}

void Transformable::scale(const sf::Vector2f &factor)
{
    trans.scale(factor);
    if (auto soundPlayer = dynamic_cast<SoundPlayer*>(this))
        soundPlayer->posUpdate(trans.getPosition());
    if (auto collidable = dynamic_cast<Collidable*>(this))
        collidable->transUpdate(trans.getTransform(), trans.getScale());
}
