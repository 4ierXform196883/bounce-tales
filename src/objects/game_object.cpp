#include "game_object.hpp"

#include <queue>
#include <iostream>

GameObject::GameObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable)
    : tag(tag), drawable(drawable), transformable(std::make_shared<Transformable>()) {}

std::shared_ptr<IGameObject> GameObject::findChild(const std::string &tag)
{
    for (auto child : this->children)
    {
        if (child->tag == tag)
            return child;
        if (auto subchild = child->findChild(tag))
            return subchild;
    }
    return nullptr;
}

void GameObject::setPosition(float x, float y)
{
    transformable->setPosition(x, y);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::setPosition(const sf::Vector2f &position)
{
    transformable->setPosition(position);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::setRotation(float angle)
{
    transformable->setRotation(angle);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::setScale(float factorX, float factorY)
{
    transformable->setScale(factorX, factorY);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::setScale(const sf::Vector2f &factors)
{
    transformable->setScale(factors);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::setOrigin(float x, float y)
{
    transformable->setOrigin(x, y);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::setOrigin(const sf::Vector2f &origin)
{
    transformable->setOrigin(origin);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::move(float offsetX, float offsetY)
{
    transformable->move(offsetX, offsetY);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::move(const sf::Vector2f &offset)
{
    transformable->move(offset);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::rotate(float angle)
{
    transformable->rotate(angle);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::scale(float factorX, float factorY)
{
    transformable->scale(factorX, factorY);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)
        collidable->transUpdate(getTransform(), getScale());
}

void GameObject::scale(const sf::Vector2f &factor)
{
    transformable->scale(factor);
    if (soundPlayer)
        soundPlayer->posUpdate(getPosition());
    if (collidable)   
        collidable->transUpdate(getTransform(), getScale());
}
