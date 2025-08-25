#include "ground.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include <iostream>
#include "curved_shape.hpp"
#include "earcut.hpp"
#include "spline.hpp"

Ground::Ground(const std::vector<sf::Vector2f> &verts, const std::string &textureName, bool bezierVerts)
    : GameObject("ground")
{
    auto &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager->getTexture(textureName);
    sf::Vector2u tSize = texture.getSize();

    std::vector<sf::Vector2f> evaluatedVerts;
    if (bezierVerts)
    {
        Spline bezierSpline(verts, true);
        for (size_t i = 0; i < bezierSpline.getSplineCount(); ++i)
        {
            float xDelta = bezierSpline.getPoint((i + 1) * 3).x - bezierSpline.getPoint(i * 3).x;
            float yDelta = bezierSpline.getPoint((i + 1) * 3).y - bezierSpline.getPoint(i * 3).y;
            float splineLen = std::sqrt(xDelta * xDelta + yDelta * yDelta);
            int numPoints = (int)splineLen / 10;
            for (float t = 0; t <= 1; t += 1.f / numPoints)
            {
                sf::Vector2f pos = bezierSpline.evaluate(i, t);
                evaluatedVerts.push_back(pos);
            }
        }
    }
    // std::cout << "Aboba\n";
    auto [shape, hitbox] = mapbox::earcutShapeAndHitbox(bezierVerts ? evaluatedVerts : verts);
    drawable = std::make_shared<CurvedShape>(texture, shape);
    collidable = std::make_shared<Collidable>(hitbox, 0.1);
    // std::cout << "Abobak\n";
}
