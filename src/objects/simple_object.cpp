#include "simple_object.hpp"

#include "game.hpp"
#include "primitive_sprite.hpp"
#include "curved_shape.hpp"
#include "animation.hpp"

SimpleObject::SimpleObject(const std::string &tag, const std::string &textureName)
    : GameObject(tag)
{
    const sf::Texture &texture = Game::getAssetManager()->getTexture(textureName);
    sf::Vector2u tSize = texture.getSize();
    this->setOrigin(sf::Vector2f(tSize.x / 2.0f, tSize.y / 2.0f));
    drawable = std::make_shared<PrimitiveSprite>(texture);
}

SimpleObject::SimpleObject(const std::string &tag, const std::string &textureName, const std::string &subtextureName)
    : GameObject(tag)
{
    const auto &am = Game::getAssetManager();
    const sf::Texture &texture = am->getTexture(textureName);
    sf::Vector2u tSize = texture.getSize();
    sf::IntRect bounds = am->getSpriteBounds(textureName, subtextureName);
    this->setOrigin(sf::Vector2f(bounds.width / 2.0f, bounds.height / 2.0f));
    drawable = std::make_shared<PrimitiveSprite>(texture, bounds);
}

SimpleObject::SimpleObject(const std::string &tag, const std::string &textureName, const std::string &animationName, double fps)
    : GameObject(tag)
{
    const auto &am = Game::getAssetManager();
    const sf::Texture &texture = am->getTexture(textureName);
    sf::Vector2u tSize = texture.getSize();
    this->setOrigin(sf::Vector2f(tSize.x / 2.0f, tSize.y / 2.0f));
    drawable = std::make_shared<Animation>(texture, am->getAnimationBounds(textureName, animationName), fps);
}
