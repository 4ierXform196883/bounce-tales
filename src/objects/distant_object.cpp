#include "distant_object.hpp"

#include "game.hpp"

DistantObject::DistantObject(const std::string &tag, std::shared_ptr<sf::Drawable> drawable, const sf::Vector2f &pos, float speed)
    : GameObject(tag, drawable), initPos(pos)
{
    this->speed = speed > 1 ? 1 : (speed < 0 ? 0 : speed);
    this->setPosition(pos);
    this->setScale(speed, speed);
}

void DistantObject::update()
{
    sf::Vector2f cPos = Game::getCamera().getCenter();
    this->setPosition(initPos + (1 - speed) * cPos);
}
