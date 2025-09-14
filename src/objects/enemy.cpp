#include "enemy.hpp"

#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include "ground.hpp"
#include <iostream>
#include "physical.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define normalized(vec) (vec.x == 0 && vec.y == 0 ? sf::Vector2f(0, 0) : (1 / norm(vec)) * vec)

Enemy::Enemy(float walkDistance)
    : GameObject("enemy"), walkDistance(walkDistance)
{
    auto &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager->getTexture("enemy");
    const sf::IntRect &rect = assetManager->getSpriteBounds("enemy", "right");
    drawable = std::make_shared<PrimitiveSprite>(texture, rect);
    ConvexHitbox hitbox{.points = {{0.0f, 0.0f}, {rect.width, 0.0f}, {rect.width, rect.height}, {0.0f, rect.height}}};
    hitbox.points = {{sf::Vector2f(0, 10), sf::Vector2f(109, 10), sf::Vector2f(109, 15), sf::Vector2f(0, 15)}};
    collidable = std::make_shared<Collidable>(hitbox, 0, true);
    this->setOrigin(rect.width / 2, rect.height);
}

void Enemy::update()
{

}

void Enemy::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherPhys = std::dynamic_pointer_cast<IPhysical>(other);
    if (!otherPhys)
        return;

    sf::Vector2f forceDir = other->getPosition() - this->getPosition();
    forceDir = normalized(forceDir);
    otherPhys->addForce(1.0f * forceDir);
}