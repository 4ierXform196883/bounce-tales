#include "enemy.hpp"

#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include "ground.hpp"
#include "physical.hpp"
#include "randomizer.hpp"
#include "trigger_object.hpp"

#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))
#define norm(vec) (std::sqrt(vec.x * vec.x + vec.y * vec.y))
#define normalized(vec) (vec.x == 0 && vec.y == 0 ? sf::Vector2f(0, 0) : (1 / norm(vec)) * vec)

Enemy::Enemy(const sf::Vector2f &startPos, float walkDistance, float speed)
    : GameObject("enemy"), startPos(startPos), walkDistance(walkDistance), speed(speed)
{
    auto &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager->getTexture("enemy");
    const sf::IntRect &rect = assetManager->getSpriteBounds("enemy", "right");
    drawable = std::make_shared<PrimitiveSprite>(texture, rect);
    ConvexHitbox hitbox{.points = {{0.0f, 0.0f}, {(float)rect.width, 0.0f}, {(float)rect.width, (float)rect.height}, {0.0f, (float)rect.height}}};
    // hitbox.points = {{sf::Vector2f(0, 10), sf::Vector2f(109, 10), sf::Vector2f(109, 15), sf::Vector2f(0, 15)}};
    collidable = std::make_shared<Collidable>(hitbox, 0, true);
    this->setOrigin(rect.width / 2, rect.height / 2);
    this->fullHeight = rect.height;

    auto downState = std::make_shared<SimpleObject>("down_child", "enemy", "down", 6);
    downState->setOrigin(rect.width / 2, -rect.height / 2 + 10);
    downState->setScale(1.4, 1);
    children.push_back(downState);

    std::function<void()> timerCallback = [this]
    {
        this->isDown = !this->isDown;
        this->downTimer->duration = randomizer::getRandFloat(2, 5);
    };
    downTimer = Timer::create(randomizer::getRandFloat(2, 5), timerCallback);
    this->setPosition(startPos);
}

void Enemy::update()
{
    auto drw = std::dynamic_pointer_cast<PrimitiveSprite>(drawable);
    sf::IntRect rect = drw->getTextureRect();
    int h = rect.height;
    if (speed > 0 && this->getPosition().x - startPos.x > walkDistance)
    {
        rect = Game::getAssetManager()->getSpriteBounds("enemy", "left");
        speed = -speed;
    }
    else if (speed < 0 && this->getPosition().x - startPos.x < 0)
    {
        rect = Game::getAssetManager()->getSpriteBounds("enemy", "right");
        speed = -speed;
    }

    float localSpeed = speed;
    if (!isDown)
        localSpeed = 0;
    if (isDown && h > 0)
    {
        this->move(0, 2);
        children.at(0)->move(0, -2);
        h -= 2;
        localSpeed = 0;
    }
    else if (!isDown && h < fullHeight)
    {
        this->move(0, -2);
        children.at(0)->move(0, 2);
        h += 2;
        localSpeed = 0;
    }
    rect.height = h;
    drw->setTextureRect(rect);
    this->move(localSpeed, 0);
    children.at(0)->setHidden(h > 30);
}

void Enemy::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherPhys = std::dynamic_pointer_cast<IPhysical>(other);
    if (!otherPhys)
        return;
    if (std::find(collidable->prevColliding.begin(), collidable->prevColliding.end(), other) != collidable->prevColliding.end())
        return;

    sf::Vector2f forceDir = other->getPosition() - this->getPosition();
    forceDir = normalized(forceDir);
    otherPhys->addForce(-otherPhys->getSpeed());
    otherPhys->addForce(5.0f * forceDir);
    if (!isDown && abs(forceDir.x) < 0.35)
    {
        {
            auto egg = std::make_shared<TriggerObject>("egg", "egg");
            egg->setPosition(this->getPosition());
            Game::getObjectManager()->addObject(egg);
            this->alive = false;
        }
    }