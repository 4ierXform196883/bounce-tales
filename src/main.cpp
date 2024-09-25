#include <SFML/Graphics.hpp>

#include "game.hpp"

int main()
{
    game::init();
    while (!game::shouldClose())
    {
        game::tick();
    }
    game::close();
    return 0;
}
