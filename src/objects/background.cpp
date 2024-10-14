#include "background.hpp"

#include "primitive_sprite.hpp"
#include "asset_manager.hpp"
#include "settings.hpp"
#include "game.hpp"
#include "simple_object.hpp"
#include "background_object.hpp"
#include <iostream>

Background::Background() : GameObject("background", nullptr)
{
    const sf::Texture &texture = AssetManager::getTexture("Background_0");
    sf::Vector2u tSize = texture.getSize();
    const sf::Vector2f &cSize = Game::getUiCamera().getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture, sf::IntRect(0, 0, cSize.x, tSize.y));
    this->setScale(1, cSize.y / tSize.y);
    this->setOrigin(cSize.x / 2, tSize.y / 2);

    children.push_back(std::make_shared<SimpleObject>("sun", std::make_shared<PrimitiveSprite>(AssetManager::getTexture("Sun")), sf::Vector2f(-500, -300)));
    children.push_back(std::make_shared<BackgroundObject>("test", std::make_shared<PrimitiveSprite>(AssetManager::getTexture("Sun")), sf::Vector2f(100, 100), 0.0f, 2.0f));
    children.push_back(std::make_shared<BackgroundObject>("test2", std::make_shared<PrimitiveSprite>(AssetManager::getTexture("Sun")), sf::Vector2f(100, 100), 0.0f, 2.0f));
    children.push_back(std::make_shared<BackgroundObject>("test2", std::make_shared<PrimitiveSprite>(AssetManager::getTexture("Sun")), sf::Vector2f(100, 100), 10.0f, 2.0f));
    children.push_back(std::make_shared<BackgroundObject>("test2", std::make_shared<PrimitiveSprite>(AssetManager::getTexture("Sun")), sf::Vector2f(100, 100), 30.0f, 2.0f));
    children.push_back(std::make_shared<BackgroundObject>("test2", std::make_shared<PrimitiveSprite>(AssetManager::getTexture("Sun")), sf::Vector2f(100, 100), 0.01f, 2.0f));
}