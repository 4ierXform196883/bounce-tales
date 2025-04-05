#include "platform.hpp"
#include <cmath>
#include <iostream>
#include "game.hpp"
#include "earcut.hpp"
#include "curved_shape.hpp"
#include "primitive_sprite.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define normalized(vec) (vec.x == 0 && vec.y == 0 ? sf::Vector2f(0, 0) : (1 / norm(vec)) * vec)

#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))

Platform::Platform(const std::string& textureName, const std::vector<sf::Vector2f> &verts, const std::vector<sf::Vector2f> &path, float speedMult)
    : GameObject("platform"), path(path), speedMult(speedMult)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture(textureName);
    sf::Vector2u tSize = texture.getSize();
    this->setOrigin(sf::Vector2f(tSize.x / 2.0f, tSize.y / 2.0f));

    // std::vector<sf::Vector2f> shape = mapbox::earcutShape(verts);
    // drawable = std::make_shared<CurvedShape>(texture, shape);
    drawable = std::make_shared<PrimitiveSprite>(texture);
    collidable = std::make_shared<Collidable>(ConvexHitbox{verts});
    physical = std::make_shared<Physical>(10, 0);
    physical->getLongForce("gravity").power = 0;
}

void Platform::update()
{
    if (path.size() == 0)
        return;
    sf::Vector2f dir = sf::Vector2f(path.at(cp).x, path.at(cp).y) - this->getPosition();
    if (norm(dir) < 5)
    {
        cp = (cp + 1) % path.size();
        speed = speedMult * (1.0f / 60.0f) * norm((sf::Vector2f(path.at(cp).x, path.at(cp).y) - this->getPosition()));
    }
    else
        this->physical->speed = dir * (speed / norm(dir));
}
