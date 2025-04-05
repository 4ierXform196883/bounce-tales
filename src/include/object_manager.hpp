#pragma once

#include <string>
#include <unordered_map>
#include "background.hpp"
#include "collidable.hpp"
#include "player.hpp"
#include "simple_object.hpp"
#include "ground.hpp"
#include "camera.hpp"
#include <nlohmann/json.hpp>

class ObjectManager
{
    friend class Game;

public:
    void addObject(std::shared_ptr<GameObject> object);
    void addObjects(const std::vector<std::shared_ptr<GameObject>> &objects);
    sf::View getView() const;
    inline std::shared_ptr<Camera> getCamera() const { return camera; }
    inline std::shared_ptr<Player> getPlayer() const { return player; }

private:
    void loadObject(std::shared_ptr<GameObject> obj, const nlohmann::json &data);
    void load(const std::string &path);
    void save(const std::string &path);
    void clear();

    void updateAll();
    void collideAll();
    void drawAll(sf::RenderTarget &target);

    bool loaded = false;
    const sf::View defaultView;
    std::shared_ptr<Background> background;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<GameObject>> physical;
    std::vector<std::shared_ptr<GameObject>> collidable;
    std::vector<std::shared_ptr<GameObject>> updatable;
    std::vector<std::shared_ptr<GameObject>> drawable;
};
