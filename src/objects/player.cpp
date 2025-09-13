#include "player.hpp"
#include "asset_manager.hpp"
#include "game.hpp"
#include "primitive_sprite.hpp"
#include "ground.hpp"
#include "particle.hpp"
#include <iostream>

Player::Player(const sf::Vector2f &spawnPos, float control_force)
    : GameObject("player"), spawnPos(spawnPos), control_force(control_force)
{
    auto &assetManager = Game::getAssetManager();
    const sf::Texture &texture = assetManager->getTexture("redy");
    sf::Vector2u tSize = texture.getSize();
    drawable = std::make_shared<PrimitiveSprite>(texture);
    this->setOrigin(tSize.x / 2, tSize.y / 2);
    collidable = std::make_shared<Collidable>(CircleHitbox{tSize.x / 2.0f - 2, {tSize.x / 2.0f, tSize.x / 2.0f}});
    physical = std::make_shared<Physical>(10, 0.003);
    auto ptr = std::make_shared<SimpleObject>("dots", "redy_dots");
    children.push_back(ptr);
    ptr = std::make_shared<SimpleObject>("eyes_death", "redy_emotions", "eyes_ouch");
    // ptr->move({0.f, 0.f});
    ptr->setHidden(true);
    children.push_back(ptr);
    ptr = std::make_shared<SimpleObject>("eyes_win", "redy_emotions", "eyes_happy");
    // ptr->move({0.f, 0.f});
    ptr->setHidden(true);
    children.push_back(ptr);
    spawnTime = Game::getClock().getElapsedTime().asSeconds();
    this->setPosition(spawnPos);
}

void Player::setSkin(Skin skin)
{
    switch (skin)
    {
    default:
    case NORMAL:
        this->setRotation(0);
        std::dynamic_pointer_cast<PrimitiveSprite>(this->drawable)->setTexture(Game::getAssetManager()->getTexture("redy"));
        this->setMass(10);
        break;
    case HEAVY:
        std::dynamic_pointer_cast<PrimitiveSprite>(this->drawable)->setTexture(Game::getAssetManager()->getTexture("redy_heavy"));
        this->setMass(20);
        break;
    case LIGHT:
        std::dynamic_pointer_cast<PrimitiveSprite>(this->drawable)->setTexture(Game::getAssetManager()->getTexture("redy_light"));
        this->setMass(5);
        break;
    }
}

void Player::onDeath()
{
    auto followObject = Game::getObjectManager()->getCamera()->getFollowObject();
    float control_force = this->control_force;
    auto respawnFunc = [this, followObject, control_force]()
    {
        this->control_force = control_force;
        this->collidable->trigger = false;
        this->physical->speed = {0.f, 0.f};
        this->setPosition(spawnPos);
        auto child = std::find_if(children.begin(), children.end(),
            [](std::shared_ptr<GameObject> obj) { return obj->getTag() == "eyes_death"; });
        if (child != children.end())
            (*child)->setHidden(true);
        this->respawnTimer = nullptr;
        Game::getObjectManager()->getCamera()->setFollowObject(followObject);
        float volume = (float)Game::getSettings()->getDouble("Volume", "music", 50);
        Game::getSoundManager()->setMusic(Game::getSoundManager()->getMusicName(), volume);
    };
    respawnTimer = Timer::create(3, respawnFunc, false);
    this->setRotation(0);
    this->physical->speed = {0.f, 0.f};
    this->addForce({0, -5});
    this->collidable->trigger = true;
    this->control_force = 0;
    auto child = std::find_if(children.begin(), children.end(),
        [](std::shared_ptr<GameObject> obj) { return obj->getTag() == "eyes_death"; });
    if (child != children.end())
        (*child)->setHidden(false);
    Game::getSoundManager()->playSound("death", Game::getSettings()->getDouble("Volume", "sounds", 100));
    Game::getObjectManager()->getCamera()->setFollowObject(nullptr);
    Game::getSoundManager()->stopMusic();
}

void Player::onWin()
{
    if (hasWon)
        return;
    this->hasWon = true;
    float newTime = Game::getClock().getElapsedTime().asSeconds() - Game::getStats()->currentStartTime;
    saveNewRecord(newTime, Game::getStats()->currentEggs);
    reloadTimer = Timer::create(3, []()
                                { Game::loadLevel("menu"); }, false);
    this->setRotation(0);
    this->physical->speed = {0.f, 0.f};
    this->control_force = 0;
    auto child = std::find_if(children.begin(), children.end(),
        [](std::shared_ptr<GameObject> obj) { return obj->getTag() == "eyes_win"; });
    if (child != children.end())
        (*child)->setHidden(false);
    Game::getSoundManager()->playSound("win", Game::getSettings()->getDouble("Volume", "sounds", 100));
    Game::getObjectManager()->getCamera()->setFollowObject(nullptr);
    Game::getSoundManager()->stopMusic();
    Particle::spawnScatter("particles", "conf_red", 10, this->getPosition(), 1, {5.f, 5.f}, 10);
    Particle::spawnScatter("particles", "conf_blue", 10, this->getPosition(), 2, {15.f, 15.f}, 10);
    Particle::spawnScatter("particles", "star", 10, this->getPosition(), 0.5, {10.f, 10.f}, 10);
}

void Player::update()
{
    float curTime = Game::getClock().getElapsedTime().asSeconds();
    auto child = std::find_if(children.begin(), children.end(),
        [](std::shared_ptr<GameObject> obj) { return obj->getTag() == "dots"; });
    if (child != children.end())
        (*child)->rotate(this->physical->speed.x * 3);
    if (this->physical->mass != 10)
        this->rotate(this->physical->speed.x * 3);
    if (hasWon && onGround && onGround && curTime - lastJumpTime > 0.25)
    {
        this->addForce({0, -10});
        lastJumpTime = curTime;
    }

    if (control_force)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
            this->setSkin(NORMAL);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
            this->setSkin(HEAVY);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
            this->setSkin(LIGHT);
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
            this->addForce({0, -5});
            lastJumpTime = curTime;
            // Particle::spawnScatter("egg", this->getPosition(), 1, sf::Vector2f(50, 50), 30);
        }
    }
    onGround = false;
}

void Player::onCollision(std::shared_ptr<GameObject> other)
{
    auto otherCol = std::dynamic_pointer_cast<ICollidable>(other);
    onGround = otherCol && !otherCol->isTrigger() || onGround;
}

void Player::saveNewRecord(float time, int eggs)
{
    const auto &stats = Game::getStats();
    float bestTime = stats->get(stats->currentLevelName, "best_time");
    int bestEggs = stats->get(stats->currentLevelName, "best_eggs");
    stats->set(stats->currentLevelName, "best_time", time < bestTime || bestTime == 0 ? time : bestTime);
    stats->set(stats->currentLevelName, "best_eggs", eggs > bestEggs ? eggs : bestEggs);
}
