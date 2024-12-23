#pragma once

#include <string>
#include <unordered_map>
#include "background.hpp"
#include "collidable.hpp"
#include "player.hpp"
#include "simple_object.hpp"
#include "ground.hpp"

class ObjectManager
{
public:
    void load(const std::string &path);
    void save(const std::string &path);

    void updateAll();
    void collideAll();
    void moveAll();
    void drawBackground(sf::RenderTarget &target);
    void drawObjects(sf::RenderTarget &target);
    void drawUI(sf::RenderTarget &target);

private:
    void recursiveDraw(std::shared_ptr<GameObject> object, sf::RenderTarget &target);
    void recursiveUpdate(std::shared_ptr<GameObject> object);
    void recursiveMove(std::shared_ptr<GameObject> object);
    void recursiveCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second);

    void calculateCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second);

    std::shared_ptr<Background> background;
    std::shared_ptr<Player> player;
    std::shared_ptr<Player> test;
    std::shared_ptr<Ground> testGround;
};
