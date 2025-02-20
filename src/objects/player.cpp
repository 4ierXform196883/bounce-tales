#include "player.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include "ground.hpp"
#include <iostream>

Player::Player(const sf::Vector2f &pos, float control_force, float maxSpeed, float friction, float graivty)
    : GameObject("player"), control_force(control_force)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("Redy");
    sf::Vector2u tSize = texture.getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture);
    this->setOrigin(tSize.x / 2, tSize.y / 2);
    collidable = std::make_shared<Collidable>(CircleHitbox{tSize.x / 2.0f, {tSize.x / 2.0f, tSize.x / 2.0f}});
    physical = std::make_shared<Physical>(10, maxSpeed, friction, graivty);
    this->setPosition(pos);
}

void Player::update()
{
    float curTime = Game::getClock().getElapsedTime().asSeconds();
    if (control_force)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            this->addForce({-control_force, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            this->addForce({control_force, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround && curTime - lastJumpTime > 2)
        {
            this->addForce({0, -control_force * 50});
            lastJumpTime = curTime;
        }
    }
    onGround = false;
}

void Player::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherCol = std::dynamic_pointer_cast<ICollidable>(other);
    onGround = otherCol && !otherCol->isTrigger() || onGround;
}
