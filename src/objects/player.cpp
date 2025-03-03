#include "player.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include "ground.hpp"
#include "particle.hpp"
#include <iostream>

Player::Player(float control_force)
    : GameObject("player"), control_force(control_force)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("redy");
    sf::Vector2u tSize = texture.getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture);
    this->setOrigin(tSize.x / 2, tSize.y / 2);
    collidable = std::make_shared<Collidable>(CircleHitbox{tSize.x / 2.0f, {tSize.x / 2.0f, tSize.x / 2.0f}});
    physical = std::make_shared<Physical>(10, 0.001);
    std::map<std::string, sf::Sound> sounds;
    sounds.emplace("win", assetManager.getSoundBuffer("win"));
    sounds.emplace("death", assetManager.getSoundBuffer("death"));
    soundPlayer = std::make_shared<SoundPlayer>(sounds);
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && onGround && curTime - lastJumpTime > 1)
        {
            this->addForce({0, -control_force * 50});
            lastJumpTime = curTime;
            Particle::spawnCircle("jump_pad", "jump", this->getPosition(), 2, 25, 30);
            // Particle::spawnScatter("egg", this->getPosition(), 1, sf::Vector2f(50, 50), 30);
            play("death");
        }
    }
    onGround = false;
}

void Player::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherCol = std::dynamic_pointer_cast<ICollidable>(other);
    onGround = otherCol && !otherCol->isTrigger() || onGround;
}
