#include "background_object.hpp"

#include "game.hpp"

BackgroundObject::BackgroundObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable, const sf::Vector2f &pos, float speed)
    : GameObject(tag, drawable), initPos(pos)
{
    this->speed = speed > 1 ? 1 : (speed < 0 ? 0 : speed);
    this->setPositionV(pos);
    this->setScaleV(speed, speed);
}

void BackgroundObject::update()
{
    sf::Vector2f cPos = game::getCamera().getCenter();
    this->setPositionV(initPos + (1 - speed) * cPos);
}
