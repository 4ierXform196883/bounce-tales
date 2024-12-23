#include "ground.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include <iostream>
#include "curved_shape.hpp"

Ground::Ground(std::vector<sf::Vector2f> verticies)
    : GameObject("ground", nullptr)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("HideZone1");
    sf::Vector2u tSize = texture.getSize();
    
    drawable = std::make_shared<CurvedShape>(Game::getAssetManager().getTexture("HideZone1"), verticies);
    collidable = std::make_shared<Collidable>(ConcaveHitbox{verticies});
}
