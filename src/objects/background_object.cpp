#include "background_object.hpp"

#include "game.hpp"
#include <iostream>
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

BackgroundObject::BackgroundObject(const std::string &tag, std::shared_ptr<PrimitiveSprite> sprite, const sf::Vector2f &pos, float speed, float distance)
    : GameObject(tag, sprite), speed(speed < 0 ? 0 : speed), distance(distance < 1 ? 1 : distance), tSize(sprite->getTexture()->getSize())
{
    this->setPosition(pos);
    this->setOrigin({(float)tSize.x / 2.0f, (float)tSize.y / 2.0f});
    this->setScale(1 / this->distance, 1 / this->distance);
}

void BackgroundObject::update()
{
    const sf::Vector2f &cSize = Game::getObjectManager().getCamera().getSize();
    if (this->speed == 0)
    {
        const sf::Vector2f &cPos = Game::getObjectManager().getCamera().getCenter();
        this->move(-(cPos - prevCPos) / distance);
        this->prevCPos = cPos;
    }
    else
    {
        this->move(speed / distance, 0);
    }
    const sf::Vector2f &thisPos = this->getPosition();
    if (abs(thisPos.x) > cSize.x + (float)tSize.x / 2.0f)
        this->setPosition(thisPos.x - sign(thisPos.x) * (2 * cSize.x + tSize.x), thisPos.y);
    if (abs(thisPos.y) > cSize.y + (float)tSize.y / 2.0f)
        this->setPosition(thisPos.y - sign(thisPos.y) * (2 * cSize.y + tSize.y), thisPos.y);
}
