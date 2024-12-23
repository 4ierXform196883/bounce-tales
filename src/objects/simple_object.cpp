#include "simple_object.hpp"

SimpleObject::SimpleObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable, const sf::Vector2f &pos, const sf::Vector2f &factors, float angle)
    : GameObject(tag, drawable)
{
    this->setPosition(pos);
    this->setScale(factors);
    this->setRotation(angle);
}