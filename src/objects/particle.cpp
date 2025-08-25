#include "particle.hpp"

#include "game.hpp"
#include "primitive_sprite.hpp"
#include "randomizer.hpp"
#include "animation.hpp"

#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define normalized(vec) (vec.x == 0 && vec.y == 0 ? sf::Vector2f(0, 0) : (1 / norm(vec)) * vec)

void Particle::spawnCircle(const std::string &textureName, const sf::Vector2f &pos, float speed, float radius, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float phi = i * 2 * M_PI / amount;
        sf::Vector2f circlePoint = {radius * std::cos(phi), radius * std::sin(phi)};
        auto ptr = std::make_shared<Particle>(textureName, circlePoint, speed);
        ptr->setPosition(pos + circlePoint);
        Game::getObjectManager()->addObject(ptr);
    }
}

void Particle::spawnScatter(const std::string &textureName, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float x = randomizer::getRandFloat(pos.x - delta.x, pos.x + delta.x);
        float y = randomizer::getRandFloat(pos.y - delta.y, pos.y + delta.y);
        float dx = randomizer::getRandFloat(-1, 1);
        float dy = randomizer::getRandFloat(-1, 1);
        auto ptr = std::make_shared<Particle>(textureName, sf::Vector2f(dx, dy), speed);
        ptr->setPosition(x, y);
        Game::getObjectManager()->addObject(ptr);
    }
}

void Particle::spawnScatterDirected(const std::string &textureName, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, const sf::Vector2f &dir, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float x = randomizer::getRandFloat(pos.x - delta.x, pos.x + delta.x);
        float y = randomizer::getRandFloat(pos.y - delta.y, pos.y + delta.y);
        auto ptr = std::make_shared<Particle>(textureName, dir, speed);
        ptr->setPosition(x, y);
        Game::getObjectManager()->addObject(ptr);
    }
}

void Particle::spawnCircle(const std::string &textureName, const std::string &subtextureName, const sf::Vector2f &pos, float speed, float radius, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float phi = i * 2 * M_PI / amount;
        sf::Vector2f circlePoint = {radius * std::cos(phi), radius * std::sin(phi)};
        auto ptr = std::make_shared<Particle>(textureName, subtextureName, circlePoint, speed);
        ptr->setPosition(pos + circlePoint);
        Game::getObjectManager()->addObject(ptr);
    }
}

void Particle::spawnScatter(const std::string &textureName, const std::string &subtextureName, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float x = randomizer::getRandFloat(pos.x - delta.x, pos.x + delta.x);
        float y = randomizer::getRandFloat(pos.y - delta.y, pos.y + delta.y);
        float dx = randomizer::getRandFloat(-1, 1);
        float dy = randomizer::getRandFloat(-1, 1);
        auto ptr = std::make_shared<Particle>(textureName, subtextureName, sf::Vector2f(dx, dy), speed);
        ptr->setPosition(x, y);
        Game::getObjectManager()->addObject(ptr);
    }
}

void Particle::spawnScatterDirected(const std::string &textureName, const std::string &subtextureName, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, const sf::Vector2f &dir, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float x = randomizer::getRandFloat(pos.x - delta.x, pos.x + delta.x);
        float y = randomizer::getRandFloat(pos.y - delta.y, pos.y + delta.y);
        auto ptr = std::make_shared<Particle>(textureName, subtextureName, dir, speed);
        ptr->setPosition(x, y);
        Game::getObjectManager()->addObject(ptr);
    }
}

void Particle::spawnCircle(const std::string &textureName, const std::string &animationName, float fps, const sf::Vector2f &pos, float speed, float radius, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float phi = i * 2 * M_PI / amount;
        sf::Vector2f circlePoint = {radius * std::cos(phi), radius * std::sin(phi)};
        auto ptr = std::make_shared<Particle>(textureName, animationName, fps, circlePoint, speed);
        ptr->setPosition(pos + circlePoint);
        Game::getObjectManager()->addObject(ptr);
    }
}

void Particle::spawnScatter(const std::string &textureName, const std::string &animationName, float fps, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float x = randomizer::getRandFloat(pos.x - delta.x, pos.x + delta.x);
        float y = randomizer::getRandFloat(pos.y - delta.y, pos.y + delta.y);
        float dx = randomizer::getRandFloat(-1, 1);
        float dy = randomizer::getRandFloat(-1, 1);
        auto ptr = std::make_shared<Particle>(textureName, animationName, fps, sf::Vector2f(dx, dy), speed);
        ptr->setPosition(x, y);
        Game::getObjectManager()->addObject(ptr);
    }
}

void Particle::spawnScatterDirected(const std::string &textureName, const std::string &animationName, float fps, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, const sf::Vector2f &dir, size_t amount)
{
  if(Game::isEditorMode()) return;
    for (size_t i = 0; i < amount; ++i)
    {
        float x = randomizer::getRandFloat(pos.x - delta.x, pos.x + delta.x);
        float y = randomizer::getRandFloat(pos.y - delta.y, pos.y + delta.y);
        auto ptr = std::make_shared<Particle>(textureName, animationName, fps, dir, speed);
        ptr->setPosition(x, y);
        Game::getObjectManager()->addObject(ptr);
    }
}

Particle::Particle(const std::string &textureName, const sf::Vector2f &dir, float speed)
    : GameObject("particle"), dir(normalized(dir)), speed(speed), isAnimation(false)
{
    const sf::Texture &texture = Game::getAssetManager()->getTexture(textureName);
    sf::Vector2u tSize = texture.getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture);
    this->setOrigin({(float)tSize.x / 2.f, (float)tSize.y / 2.f});
}

Particle::Particle(const std::string &textureName, const std::string &subtextureName, const sf::Vector2f &dir, float speed)
    : GameObject("particle"), dir(normalized(dir)), speed(speed), isAnimation(false)
{
    const sf::Texture &texture = Game::getAssetManager()->getTexture(textureName);
    const auto &spriteBounds = Game::getAssetManager()->getSpriteBounds(textureName, subtextureName);
    sf::Vector2f tSize = sf::Vector2f(spriteBounds.width, spriteBounds.height);
    drawable = std::make_shared<PrimitiveSprite>(texture, spriteBounds);
    this->setOrigin({(float)tSize.x / 2.f, (float)tSize.y / 2.f});
}

Particle::Particle(const std::string &textureName, const std::string &animationName, float fps, const sf::Vector2f &dir, float speed)
    : GameObject("particle"), dir(normalized(dir)), speed(speed), isAnimation(true)
{
    const sf::Texture &texture = Game::getAssetManager()->getTexture(textureName);
    const auto &animationBounds = Game::getAssetManager()->getAnimationBounds(textureName, animationName);
    sf::Vector2i tSize = animationBounds.at(0).getSize();
    drawable = std::make_shared<Animation>(texture, animationBounds, fps);
    this->setOrigin({(float)tSize.x / 2.f, (float)tSize.y / 2.f});
}

void Particle::update()
{
    this->move(speed * dir);
    if (this->alpha - 2.5 * speed <= 0)
        this->alive = false;
    this->alpha -= 2.5 * speed;
    if (!isAnimation)
        std::dynamic_pointer_cast<PrimitiveSprite>(drawable)->setColor(sf::Color(255, 255, 255, alpha));
    else
        std::dynamic_pointer_cast<Animation>(drawable)->setColor(sf::Color(255, 255, 255, alpha));
}
