#include "player.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include <iostream>

Player::Player()
    : GameObject("player", nullptr)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("Redy");
    sf::Vector2u tSize = texture.getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture);
    // this->setOrigin(tSize.x / 2, tSize.y / 2);
    // collidable = std::make_shared<Collidable>(ConcaveHitbox{{{-25, 25}, {0, -25}, {25, 25}}});
    collidable = std::make_shared<Collidable>(CircleHitbox{tSize.x / 2.0f, {tSize.x / 2.0f, tSize.x / 2.0f}});
    physical = std::make_shared<Physical>(10, 0.1, 0.5);
}
