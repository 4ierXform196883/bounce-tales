#include "ground.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include <iostream>
#include "curved_shape.hpp"
#include "earcut.hpp"

Ground::Ground(const std::vector<std::array<double, 2>> &verts)
    : GameObject("ground")
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("HideZone1");
    sf::Vector2u tSize = texture.getSize();

    // Bad earcut adaptation
    std::vector<sf::Vector2f> sfmlVerts;
    std::vector<std::vector<std::array<double, 2>>> polygon;
    polygon.push_back(verts);
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        sfmlVerts.push_back({(float)polygon[0][indices[i]][0], (float)polygon[0][indices[i]][1] - 600});
    }
    // ==================

    drawable = std::make_shared<CurvedShape>(Game::getAssetManager().getTexture("HideZone1"), sfmlVerts);
    ConcaveHitbox hitbox;
    for (size_t i = 0; i < sfmlVerts.size(); i += 3)
    {
        hitbox.triangles.push_back(ConvexHitbox{{sfmlVerts[i], sfmlVerts[i + 1], sfmlVerts[i + 2]}});
    }
    collidable = std::make_shared<Collidable>(hitbox);
}
