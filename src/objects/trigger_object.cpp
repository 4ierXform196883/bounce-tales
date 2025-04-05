#include "trigger_object.hpp"

#include "game.hpp"
#include "primitive_sprite.hpp"
#include "animation.hpp"

TriggerObject::TriggerObject(const sf::Vector2f &size, const std::function<void(std::shared_ptr<GameObject> other)> &callback)
    : GameObject("trigger"), callback(callback)
{
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {size.x, 0}, {size.x, size.y}, {0, size.y}}}, 0, true);
    this->setOrigin(size.x / 2.f, size.y / 2.f);
}

TriggerObject::TriggerObject(const std::string &textureName, const std::function<void(std::shared_ptr<GameObject> other)> &callback)
    : GameObject("trigger"), callback(callback)
{
    const AssetManager &am = Game::getAssetManager();
    const sf::Texture &texture = am.getTexture(textureName);
    const sf::Vector2f tSize = sf::Vector2f(texture.getSize().x, texture.getSize().y);
    this->drawable = std::make_shared<PrimitiveSprite>(texture);
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {tSize.x, 0}, tSize, {0, tSize.y}}}, 0, true);
    this->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
}

TriggerObject::TriggerObject(const std::string &textureName, const std::string &subtextureName, const std::function<void(std::shared_ptr<GameObject> other)> &callback)
    : GameObject("trigger"), callback(callback)
{
    const AssetManager &am = Game::getAssetManager();
    const sf::Texture &texture = am.getTexture(textureName);
    const auto &spriteBounds = am.getSpriteBounds(textureName, subtextureName);
    const sf::Vector2f tSize = sf::Vector2f(spriteBounds.width, spriteBounds.height);
    this->drawable = std::make_shared<PrimitiveSprite>(texture, spriteBounds);
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {tSize.x, 0}, tSize, {0, tSize.y}}}, 0, true);
    this->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
}

TriggerObject::TriggerObject(const std::string &textureName, const std::string &animationName, float fps, const std::function<void(std::shared_ptr<GameObject> other)> &callback)
    : GameObject("trigger"), callback(callback)
{
    const AssetManager &am = Game::getAssetManager();
    const sf::Texture &texture = am.getTexture(textureName);
    const auto &animationBounds = am.getAnimationBounds(textureName, animationName);
    const sf::Vector2f tSize = sf::Vector2f(animationBounds.at(0).width, animationBounds.at(0).height);
    this->drawable = std::make_shared<Animation>(texture, animationBounds, fps);
    this->collidable = std::make_shared<Collidable>(ConvexHitbox{{{0, 0}, {tSize.x, 0}, tSize, {0, tSize.y}}}, 0, true);
    this->setOrigin(tSize.x / 2.f, tSize.y / 2.f);
}
