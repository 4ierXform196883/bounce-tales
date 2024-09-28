#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

class Game
{
    friend int main();

public:
    Game() = delete;
    ~Game() = delete;

    inline static const sf::Clock &getClock() { return globalClock; }
    inline static const sf::View &getCamera() { return camera; }
    inline static const sf::View &getUiCamera() { return uiCamera; }

private:
    static void init();
    static void tick();
    static void processEvents();
    static void render();
    static void close();

    static std::unique_ptr<sf::RenderWindow> window;
    static sf::View camera;
    static sf::View uiCamera;
    static const sf::Clock globalClock;
};