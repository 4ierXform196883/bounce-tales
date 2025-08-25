#include "trigger_object.hpp"

#include "game.hpp"
#include "primitive_sprite.hpp"
#include "animation.hpp"
#include <SFML/Graphics.hpp>
#include "primitive_rectangle.hpp"
#include "particle.hpp"

std::function<void(std::shared_ptr<GameObject> other)> TriggerObject::getCallback(const std::string& callbackType)
{
    if (callbackType == "death_zone")
    {
        return [](std::shared_ptr<GameObject> other)
        {
            if (auto otherPtr = std::dynamic_pointer_cast<Player>(other))
                otherPtr->onDeath();
        };
    }
    else if (callbackType == "win_zone")
    {
        return [](std::shared_ptr<GameObject> other)
        {
            if (auto otherPtr = std::dynamic_pointer_cast<Player>(other))
                otherPtr->onWin();
        };
    }
    else if (callbackType == "egg")
    {
        return [this](std::shared_ptr<GameObject> other)
        {
            Particle::spawnCircle("particles", "star", 2, this->getPosition(), 1, 10, 10);
            this->alive = false;
            Game::getStats()->currentEggs += 1;
        };
        Game::getStats()->currentTotalEggs += 1;
    }
    // player_set_skin:heavy, player_set_skin:light, player_set_skin:normal
    else if (callbackType.substr(0, 15) == "player_set_skin")
    {
        std::string skinType = callbackType.substr(16);
        Player::Skin type = skinType == "heavy" ? Player::Skin::HEAVY : (skinType == "light" ? Player::Skin::LIGHT : Player::Skin::NORMAL);
        return [type](std::shared_ptr<GameObject> other)
        {
            if (auto otherPtr = std::dynamic_pointer_cast<Player>(other))
                otherPtr->setSkin(type);
        };
    }
    return [](std::shared_ptr<GameObject> other) {};
}

TriggerObject::TriggerObject(const sf::Vector2f &size, const std::string &callbackType)
    : GameObject("trigger"), callback(getCallback(callbackType))
{
    if (Game::isEditorMode())
    {
        this->drawable = std::make_shared<PrimitiveRectangle>(size, sf::Color(255, 255, 255, 0), 1.0f, sf::Color::Red);
    }
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {size.x, 0}, {size.x, size.y}, {0, size.y}}}, 0, true);
    this->setOrigin(size.x / 2.f, size.y / 2.f);
}

TriggerObject::TriggerObject(const std::string &textureName, const std::string &callbackType)
    : GameObject("trigger"), callback(getCallback(callbackType))
{
    const auto &am = Game::getAssetManager();
    const sf::Texture &texture = am->getTexture(textureName);
    const sf::Vector2f tSize = sf::Vector2f(texture.getSize().x, texture.getSize().y);
    this->drawable = std::make_shared<PrimitiveSprite>(texture);
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {tSize.x, 0}, tSize, {0, tSize.y}}}, 0, true);
    this->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
}

TriggerObject::TriggerObject(const std::string &textureName, const std::string &subtextureName, const std::string &callbackType)
    : GameObject("trigger"), callback(getCallback(callbackType))
{
    const auto &am = Game::getAssetManager();
    const sf::Texture &texture = am->getTexture(textureName);
    const auto &spriteBounds = am->getSpriteBounds(textureName, subtextureName);
    const sf::Vector2f tSize = sf::Vector2f(spriteBounds.width, spriteBounds.height);
    this->drawable = std::make_shared<PrimitiveSprite>(texture, spriteBounds);
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {tSize.x, 0}, tSize, {0, tSize.y}}}, 0, true);
    this->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
}

TriggerObject::TriggerObject(const std::string &textureName, const std::string &animationName, float fps, const std::string &callbackType)
    : GameObject("trigger"), callback(getCallback(callbackType))
{
    const auto &am = Game::getAssetManager();
    const sf::Texture &texture = am->getTexture(textureName);
    const auto &animationBounds = am->getAnimationBounds(textureName, animationName);
    const sf::Vector2f tSize = sf::Vector2f(animationBounds.at(0).width, animationBounds.at(0).height);
    this->drawable = std::make_shared<Animation>(texture, animationBounds, fps);
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {tSize.x, 0}, tSize, {0, tSize.y}}}, 0, true);
    this->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
}
