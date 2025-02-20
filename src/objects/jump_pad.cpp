#include "jump_pad.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include "ground.hpp"
#include <iostream>
#include "physical.hpp"

const std::vector<std::string> JumpPad::frameSequence = {"3", "2", "1", "2", "3", "0", "0", "3"};
const std::vector<int> JumpPad::heights = {58, 47, 44, 47, 58, 66, 66, 58};
// {66, 44, 47, 58}

JumpPad::JumpPad(const sf::Vector2f& pos, float power)
    : GameObject("jump_pad"), power(power)
{
    const AssetManager &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager.getTexture("JumpPad");
    const sf::IntRect &idleRect = assetManager.getSpriteBounds("JumpPad", "3");
    drawable = std::make_shared<PrimitiveSprite>(texture, idleRect);
    this->setOrigin(idleRect.width / 2, idleRect.height);
    ConvexHitbox hitbox;
    hitbox.points = {{sf::Vector2f(0, 10), sf::Vector2f(109, 10), sf::Vector2f(109, 15), sf::Vector2f(0, 15)}};
    collidable = std::make_shared<Collidable>(hitbox, true);
    this->setPosition(pos);
}

void JumpPad::update()
{
    if (currentFrame == 5)
    {
        for (auto &obj : touching)
        {
            std::dynamic_pointer_cast<IPhysical>(obj)->addForce({0, -power});
        }
        ++currentFrame;
    }
    if (currentFrame == 7)
    {
        animTimer = nullptr;
        currentFrame = 0;
    }
    touching.clear();
}

void JumpPad::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherPhys = std::dynamic_pointer_cast<IPhysical>(other);
    if (!otherPhys)
        return;

    touching.push_back(other);

    if (!animTimer)
    {
        animTimer = Timer::create(0.05, [this]()
                                  { updateFrame((++currentFrame) % frameSequence.size()); });
    }
    float thisTop = std::get<ConvexHitbox>(collidable->hitbox).points.at(0).y;
    float otherYSpeed = otherPhys->getSpeed().y;
    float otherXPos = other->getPosition().x;
    float otherYPos = other->getPosition().y;
    if (otherYSpeed > 0)
    {
        if (otherYPos + 8 - otherYSpeed <= thisTop && otherYPos + 8 > thisTop)
        {
            other->setPosition({otherXPos, thisTop - 8});
            otherPhys->addForce({0, -otherPhys->getSpeed().y});
        }
    }
}

void JumpPad::updateFrame(size_t newFrame)
{
    size_t oldFrame = newFrame == 0 ? frameSequence.size() - 1 : newFrame - 1;
    const sf::IntRect &newRect = Game::getAssetManager().getSpriteBounds("JumpPad", frameSequence.at(newFrame));
    int diff = heights.at(newFrame) - heights.at(oldFrame);
    std::dynamic_pointer_cast<PrimitiveSprite>(drawable)->setTextureRect(newRect);
    for (auto &point : std::get<ConvexHitbox>(collidable->hitbox).points)
    {
        point.y -= diff;
    }
    for (auto &obj : touching)
    {
        obj->move({0, (float)-diff});
    }
}
