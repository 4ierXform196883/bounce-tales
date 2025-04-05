#pragma once

#include "game_object.hpp"

class Player : public GameObject, public ICollidable, public IPhysical, public ISoundPlayer
{
public:
    COLLIDABLE
    PHYSICAL
    SOUND_PLAYER

    enum Skin
    {
        NORMAL,
        HEAVY,
        LIGHT
    };

    Player(const sf::Vector2f &spawnPos, float control_force);
    virtual ~Player() = default;

    void setSkin(Skin skin);

        void onDeath();
    void onWin();

    float control_force = 0.1;

protected:
    virtual void update() override;
    virtual void onCollision(std::shared_ptr<GameObject> other) override;

    sf::Vector2f spawnPos;
    float spawnTime;
    std::shared_ptr<Timer> respawnTimer;
    std::shared_ptr<Timer> reloadTimer;
    bool onGround = false, hasWon = false;
    float lastJumpTime = 0;
};