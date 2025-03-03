#include "ground.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include <iostream>
#include "curved_shape.hpp"
#include "earcut.hpp"

Ground::Ground(const std::vector<sf::Vector2f> &verts, const std::string& textureName)
    : GameObject("ground")
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture(textureName);
    sf::Vector2u tSize = texture.getSize();

    auto [shape, hitbox] = mapbox::earcutShapeAndHitbox(verts);
    drawable = std::make_shared<CurvedShape>(texture, shape);
    collidable = std::make_shared<Collidable>(hitbox, 0.1);
}
