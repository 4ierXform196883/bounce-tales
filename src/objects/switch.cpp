#include "switch.hpp"

#include "player.hpp"
#include "door.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"

Switch::Switch(const std::vector<std::shared_ptr<Door>> &doors)
    : GameObject("a_switch"), doors(doors)
{
    auto &am = Game::getAssetManager();
    const auto &texture = am->getTexture("switch");
    const auto tSize = texture.getSize();
    const sf::IntRect &frame = am->getSpriteBounds("switch", "off");
    const sf::Vector2f hSize = sf::Vector2f(frame.width - 1, frame.height - 1);
    this->drawable = std::make_shared<PrimitiveSprite>(texture, frame);
    ConvexHitbox hitbox = ConvexHitbox{{{1, 1}, {hSize.x, 1}, hSize, {1, hSize.y}}};
    this->collidable = std::make_shared<Collidable>(hitbox, 0, true);
    this->setOrigin(frame.width / 2.f, frame.height / 2.f);
}

void Switch::setState(bool state)
{
    this->isOn = state;
    for (auto it = doors.begin(); it != doors.end();)
    {
        if (!(*it))
        {
            it = doors.erase(it);
            continue;
        }
        (*it)->isOpened = this->isOn;
        ++it;
    }
    const sf::IntRect &bounds = Game::getAssetManager()->getSpriteBounds("switch", this->isOn ? "on": "off");
    std::dynamic_pointer_cast<PrimitiveSprite>(drawable)->setTextureRect(bounds);
}

void Switch::switchState()
{
    this->isOn = !this->isOn;
    for (auto it = doors.begin(); it != doors.end();)
    {
        if (!(*it))
        {
            it = doors.erase(it);
            continue;
        }
        (*it)->isOpened = !(*it)->isOpened;
        ++it;
    }
    const sf::IntRect &bounds = Game::getAssetManager()->getSpriteBounds("switch", this->isOn ? "on": "off");
    std::dynamic_pointer_cast<PrimitiveSprite>(drawable)->setTextureRect(bounds);
}

void Switch::onCollision(std::shared_ptr<GameObject> other)
{
    if (!std::dynamic_pointer_cast<Player>(other))
        return;
    if (std::find(collidable->prevColliding.begin(), collidable->prevColliding.end(), other) != collidable->prevColliding.end())
        return;
    this->switchState();
}
