#include "background.hpp"

#include "primitive_sprite.hpp"
#include "asset_manager.hpp"
#include "settings.hpp"
#include "game.hpp"
#include "simple_object.hpp"
#include <iostream>

Background::Background() : GameObject("background", nullptr)
{
    const sf::Texture &texture = AssetManager::getTexture("Background_0");
    sf::Vector2u tSize = texture.getSize();
    const sf::Vector2f &cSize = Game::getUiCamera().getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture, sf::IntRect(0, 0, cSize.x, tSize.y));
    this->setScale(1, cSize.y / tSize.y);
    this->setOrigin(cSize.x / 2, tSize.y / 2);

    auto sun = std::make_shared<SimpleObject>("sun", std::make_shared<PrimitiveSprite>(AssetManager::getTexture("Sun")), sf::Vector2f(-500, -300));
    children.push_back(sun);
}