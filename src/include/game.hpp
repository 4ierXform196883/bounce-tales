#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace game
{
    void init();
    bool shouldClose();
    void tick();
    void close();

    const sf::Clock& getClock();
    const sf::View& getCamera();
}