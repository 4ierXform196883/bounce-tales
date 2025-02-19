#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "asset_manager.hpp"
#include "object_manager.hpp"
#include "settings.hpp"
#include "music_player.hpp"
#include "gui_manager.hpp"

class Game
{
    friend int main();

public:
    Game() = delete;
    ~Game() = delete;

    inline static const sf::Clock &getClock() { return globalClock; }

    inline static const AssetManager &getAssetManager() { return assetManager; }
    inline static const ObjectManager &getObjectManager() { return objectManager; }
    // inline static const MusicPlayer &getMusicPlayer() { return musicPlayer; }
    inline static const Settings &getSettings() { return settings; }
    inline static sf::Vector2f getMousePos() { return window->mapPixelToCoords(sf::Mouse::getPosition(*window)); }
    inline static void changeMusic(const std::string &name, float volume = 100.0f) { musicPlayer.changeMusic(name, volume); }

private:
    static void init();
    static void tick();
    static void processEvents();
    static void render();
    static void close();

    static std::unique_ptr<sf::RenderWindow> window;
    static const sf::Clock globalClock;
    static double dtime;

    static AssetManager assetManager;
    static GuiManager guiManager;
    static ObjectManager objectManager;
    static MusicPlayer musicPlayer;
    static Settings settings;
};