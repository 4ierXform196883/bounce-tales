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
Stats Game::stats;
bool Game::paused = false;

std::unique_ptr<sf::RenderWindow> Game::window;
const sf::Clock Game::globalClock;

void Game::reinitWindow(const sf::Vector2i &resolution, bool fullscreen)
{
    sf::ContextSettings windowSettings = window->getSettings();
    window->close();
    auto style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
    window->create(sf::VideoMode(resolution.x, resolution.y), settings.title, style, windowSettings);
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(true);
    guiManager.gui->setTextSize(20 * resolution.x / 1280.f);
    guiManager.gui->setWindow(*window);
}

void Game::loadLevel(const std::string &name)
{
    objectManager.clear();
    stats.reset();
    stats.currentLevelName = name;
    objectManager.load("levels/" + name + ".json");
    if (name == "menu")
        guiManager.setUI(GuiManager::UI::MENU);
    else if (name == "editor")
        guiManager.setUI(GuiManager::UI::EDITOR);
    else
        guiManager.setUI(GuiManager::UI::LEVEL);
    guiManager.gui->setTextSize(20 * settings.getInt("Screen", "width", 1280) / 1280.f);
}

void Game::saveStats()
{
    float newTime = Game::getClock().getElapsedTime().asSeconds() - stats.currentStartTime;
    float bestTime = stats.get(stats.currentLevelName, "best_time");
    int bestEggs = stats.get(stats.currentLevelName, "best_eggs");
    stats.set(stats.currentLevelName, "best_time", newTime < bestTime || bestTime == 0 ? newTime : bestTime);
    stats.set(stats.currentLevelName, "best_eggs", stats.currentEggs > bestEggs ? stats.currentEggs : bestEggs);
    stats.set(stats.currentLevelName, "total_eggs", stats.currentTotalEggs);
}

void Game::init()
{
    settings.load(settings.settings_path);
    stats.load(settings.stats_path);
    assetManager.loadTextures(settings.textures_path);
    assetManager.loadSounds(settings.sounds_path);
    sf::Vector2i windowSize = {settings.getInt("Screen", "width", 1280), settings.getInt("Screen", "height", 720)};
    auto fullscreen = settings.getBool("Screen", "fullscreen", false) ? sf::Style::Fullscreen : sf::Style::Default;
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), settings.title, fullscreen);
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(true);
    guiManager.gui = std::make_unique<tgui::Gui>(*window);
    guiManager.init();

    loadLevel("menu");
}

void Game::tick()
{
    Game::processEvents();
    if (!objectManager.loaded)
        return;
    if (!paused)
    {
    objectManager.collideAll();
    Timer::updateAll();
    objectManager.updateAll();
    }
    Game::render();
}

void Game::close()
{
    Timer::globalTimers.clear();
    assetManager.unloadTextures();
    assetManager.unloadSounds();
    settings.save(settings.settings_path);
    stats.save(settings.stats_path);
    musicPlayer.stopMusic();
    window->close();
}

void Game::processEvents()
{
    for (auto event = sf::Event{}; window->pollEvent(event);)
    {
        guiManager.gui->handleEvent(event);
        switch (event.type)
        {
        case sf::Event::Closed:
            Game::close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape && guiManager.currentUI == GuiManager::UI::LEVEL)
            {
                guiManager.groups.at("pause")->setVisible(!guiManager.groups.at("pause")->isVisible());
                paused = !paused;
            }
            if (event.key.code == sf::Keyboard::F1)
                musicPlayer.setMusic("menu");
            if (event.key.code == sf::Keyboard::F2)
                musicPlayer.setMusic("level");
            if (event.key.code == sf::Keyboard::F3)
                musicPlayer.setMusic("level2");
            if (event.key.code == sf::Keyboard::F4)
                musicPlayer.setMusic("level3");
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
    guiManager.gui->draw();
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