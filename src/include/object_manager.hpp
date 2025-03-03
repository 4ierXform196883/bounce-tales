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
    void addObjects(const std::vector<std::shared_ptr<GameObject>>& objects);
    sf::View getCamera() const;

private:
    void loadObject(std::shared_ptr<GameObject> obj, const nlohmann::json &data);
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
