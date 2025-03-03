#pragma once

#include "game_object.hpp"

class Particle : public GameObject
{
public:
    static void spawnCircle(const std::string &textureName, const sf::Vector2f &pos, float speed, float radius, size_t amount);
    static void spawnScatter(const std::string &textureName, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, size_t amount);
    static void spawnScatterDirected(const std::string &textureName, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, const sf::Vector2f &dir, size_t amount);

    static void spawnCircle(const std::string &textureName, const std::string &animationName, const sf::Vector2f &pos, float speed, float radius, size_t amount);
    static void spawnScatter(const std::string &textureName, const std::string &animationName, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, size_t amount);
    static void spawnScatterDirected(const std::string &textureName, const std::string &animationName, const sf::Vector2f &pos, float speed, const sf::Vector2f &delta, const sf::Vector2f &dir, size_t amount);

    Particle(const std::string &textureName, const std::string &animationName, const sf::Vector2f &dir, float speed);
    Particle(const std::string &textureName, const sf::Vector2f &dir, float speed);
    virtual ~Particle() = default;

protected:
    virtual void update() override;
    inline virtual void onCollision(std::shared_ptr<GameObject> other) override {};

    bool isAnimation;
    sf::Uint8 alpha = 255;
    sf::Vector2f dir;
    float speed;
};