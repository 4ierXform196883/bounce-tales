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
    friend class Game;

public:
    sf::View getCamera() const;

private:
    void load(const std::string &path);
    void save(const std::string &path);

    void updateAll();
    void collideAll();
    void drawAll(sf::RenderTarget &target);

    const sf::View defaultView;
    std::shared_ptr<Background> background;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Player> player;
    std::map<std::string, std::shared_ptr<GameObject>> physical;
    std::map<std::string, std::shared_ptr<GameObject>> collidable;
    std::map<std::string, std::shared_ptr<GameObject>> updatable;
    std::map<std::string, std::shared_ptr<GameObject>> drawable;
};
