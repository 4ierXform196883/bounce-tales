#include "ground.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include <iostream>
#include "curved_shape.hpp"
#include "ear_clipping.hpp"

Ground::Ground(std::vector<sf::Vector2f> verticies)
    : GameObject("ground", nullptr)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("HideZone1");
    sf::Vector2u tSize = texture.getSize();
    // verticies = ear_clipping::processConcaveShape(verticies);
    drawable = std::make_shared<CurvedShape>(Game::getAssetManager().getTexture("HideZone1"), verticies);
    ConcaveHitbox hitbox;
    for (size_t i = 0; i < verticies.size(); i += 3)
    {
        hitbox.triangles.push_back(TriangleHitbox{{verticies[i], verticies[i + 1], verticies[i + 2]}});
        // std::cout << "Triangle: {" << hitbox.triangles.at(i / 3).points.at(0).x << " " << hitbox.triangles.at(i / 3).points.at(0).y << "}, ";
        // std::cout << "{" << hitbox.triangles.at(i / 3).points.at(1).x << " " << hitbox.triangles.at(i / 3).points.at(1).y << "}, ";
        // std::cout << "{" << hitbox.triangles.at(i / 3).points.at(2).x << " " << hitbox.triangles.at(i / 3).points.at(2).y << "}\n";
    }
    collidable = std::make_shared<Collidable>(hitbox);
}
