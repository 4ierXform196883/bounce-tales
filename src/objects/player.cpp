#include "player.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include "ground.hpp"
#include <iostream>

Player::Player()
    : GameObject("player", nullptr)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("Redy");
    sf::Vector2u tSize = texture.getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture);
    this->setOrigin(tSize.x / 2, tSize.y / 2);
    collidable = std::make_shared<Collidable>(CircleHitbox{tSize.x / 2.0f, {tSize.x / 2.0f, tSize.x / 2.0f}});
    physical = std::make_shared<Physical>(10, 0.05, 0.2);
}

void Player::update()
{   
    // this->physical->setGravity(!onGround);
    // if (!onGround)
    // {
    //     std::cout << "A";
    // }
    float curTime = Game::getClock().getElapsedTime().asSeconds();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        this->addForce({-0.1, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        this->addForce({0.1, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround && curTime - lastJumpTime > 2)
    {
        this->addForce({0, -5});
        lastJumpTime = curTime;
    }
    onGround = false;
}

void Player::onCollision(std::shared_ptr<IGameObject> other)
{
    // onGround = std::dynamic_pointer_cast<const Ground>(other) != nullptr;
    onGround = true;
}
