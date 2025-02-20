#include "game.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <iostream>
#include "settings.hpp"
#include "asset_manager.hpp"
#include "timer.hpp"
#include "primitive_sprite.hpp"
#include "game_object.hpp"
#include "animation.hpp"
#include "curved_shape.hpp"
#include "music_player.hpp"
#include "object_manager.hpp"
#include "background.hpp"

AssetManager Game::assetManager;
GuiManager Game::guiManager;
ObjectManager Game::objectManager;
MusicPlayer Game::musicPlayer;
Settings Game::settings;

std::unique_ptr<sf::RenderWindow> Game::window;
const sf::Clock Game::globalClock;

void Game::init()
{
    settings.load(settings.settings_path);
    assetManager.loadTextures(settings.textures_path);
    assetManager.loadSounds(settings.sounds_path);
    sf::Vector2i windowSize = {settings.getInt("Screen", "screenWidth", 1280), settings.getInt("Screen", "screenHeight", 720)};
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), settings.title, sf::Style::Default);
    window->setFramerateLimit(settings.getInt("Screen", "maxFps", 60));
    window->setVerticalSyncEnabled(settings.getBool("Screen", "vSync", true));

    objectManager.load("levels/debug.json");
}

void Game::tick()
{
    Game::processEvents();
    musicPlayer.update();
    objectManager.collideAll();
    Timer::updateAll();
    objectManager.updateAll();
    Game::render();
}

void Game::close()
{
    assetManager.unloadTextures();
    assetManager.unloadSounds();
    settings.save(settings.settings_path);
    window->close();
}

void Game::processEvents()
{
    for (auto event = sf::Event{}; window->pollEvent(event);)
    {
        // guiManager.gui->handleEvent(event);
        switch (event.type)
        {
        case sf::Event::Closed:
            Game::close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::F1)
                musicPlayer.changeMusic("menu");
            if (event.key.code == sf::Keyboard::F2)
                musicPlayer.changeMusic("level");
            if (event.key.code == sf::Keyboard::F3)
                musicPlayer.changeMusic("level2");
            if (event.key.code == sf::Keyboard::F4)
                musicPlayer.changeMusic("level3");
            break;

        default:
            break;
        }
    }
}

void Game::render()
{
    window->clear(sf::Color::Black);
    objectManager.drawAll(*window);
    window->display();
}

int main()
{
    Game::init();
    while (Game::window->isOpen())
        Game::tick();
    Game::close();
    return 0;
}