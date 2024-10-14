#pragma once

#include <string>
#include <unordered_map>
#include "background.hpp"
#include "collidable.hpp"

class ObjectManager
{
    friend class Game;

private:
    static void load(const std::string &path);
    static void save(const std::string &path);

    static void updateAll();
    static void drawBackground(sf::RenderTarget &target);
    static void drawObjects(sf::RenderTarget &target);
    static void drawUI(sf::RenderTarget &target);

    static std::shared_ptr<Background> background;
};
