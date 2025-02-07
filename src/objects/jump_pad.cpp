#include "jump_pad.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include "ground.hpp"
#include <iostream>
#include "physical.hpp"

JumpPad::JumpPad()
    : GameObject("jump_pad", nullptr)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("JumpPad");
    const sf::IntRect &idleRect = assetManager.getSpriteBounds("JumpPad", "3");
    idleState = std::make_shared<PrimitiveSprite>(texture, idleRect);
    activeState = std::make_shared<Animation>(texture, assetManager.getAnimationBounds("JumpPad", "jump"), 1);
    sf::Vector2u tSize = texture.getSize();
    drawable = idleState;
    this->setOrigin(idleRect.width / 2, idleRect.height);
    ConcaveHitbox hitbox;
    hitbox.triangles.push_back(TriangleHitbox{{sf::Vector2f(0, 10), sf::Vector2f(109, 10), sf::Vector2f(0, 15)}});
    hitbox.triangles.push_back(TriangleHitbox{{sf::Vector2f(109, 10), sf::Vector2f(0, 15), sf::Vector2f(109, 15)}});
    collidable = std::make_shared<Collidable>(hitbox, true);
}

void JumpPad::update()
{
    float curTime = Game::getClock().getElapsedTime().asSeconds();
    shouldPush = false;
    if (reloading && !reloadTimer)
    {
        reloadTimer = Timer::create(0.5, [this]()
                                    { reloading = false; }, false);
        jumpTimer = nullptr;
    }
}

void JumpPad::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherPhys = std::dynamic_pointer_cast<IPhysical>(other);
    if (!otherPhys)
        return;

    if (shouldPush)
    {
        
        otherPhys->addForce({0, -15});
        shouldPush = false;
    }
    else
    {
        otherPhys->addForce({0, -otherPhys->getSpeed().y - 0.15});
    }

    if (!jumpTimer && !reloading)
    {
        activeState->restart();
        drawable = activeState;
        reloadTimer = nullptr;
        jumpTimer = Timer::create(6, [this]()
                                  {   drawable = idleState;
                                      shouldPush = true;
                                      reloading = true; }, false);
    }
}
