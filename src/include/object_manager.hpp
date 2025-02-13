#pragma once

#include <string>
#include <unordered_map>
#include "background.hpp"
#include "collidable.hpp"
#include "player.hpp"
#include "simple_object.hpp"
#include "ground.hpp"
#include "camera.hpp"

class ObjectManager
{
public:
    void load(const std::string &path);
    void save(const std::string &path);

    void updateAll();
    void collideAll();
    void moveAll();
    void drawAll(sf::RenderTarget &target);

    sf::View getCamera() const;

private:
    void recursiveDraw(std::shared_ptr<GameObject> object, sf::RenderTarget &target);
    void recursiveUpdate(std::shared_ptr<GameObject> object);
    void recursiveMove(std::shared_ptr<GameObject> object);
    void recursiveCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second);

    void calculateCollision(std::shared_ptr<GameObject> first, std::shared_ptr<GameObject> second, bool notify = true);

    static const sf::View defaultView;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Background> background;
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Ground>> ground;
    std::vector<std::shared_ptr<GameObject>> collidables;
    std::vector<std::shared_ptr<GameObject>> objects;
};
