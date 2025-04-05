#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "asset_manager.hpp"
#include "object_manager.hpp"
#include "settings.hpp"
#include "music_player.hpp"
#include "gui_manager.hpp"
#include "stats.hpp"

class Game
{
    friend int main();

public:
    Game() = delete;
    ~Game() = delete;

    inline static const sf::Clock &getClock() { return globalClock; }
    inline static sf::Vector2f getMousePos() { return window->mapPixelToCoords(sf::Mouse::getPosition(*window)); }

    inline static AssetManager &getAssetManager() { return assetManager; }
    inline static ObjectManager &getObjectManager() { return objectManager; }
    inline static MusicPlayer &getMusicPlayer() { return musicPlayer; }
    inline static Settings &getSettings() { return settings; }
    inline static Stats &getStats() { return stats; }

    static void reinitWindow(const sf::Vector2i &resolution, bool fullscreen);
    static void loadLevel(const std::string &name);
    static void saveStats();
    static void close();

    static bool paused;

private:
    static void init();
    static void tick();
    static void processEvents();
    static void render();

    static std::unique_ptr<sf::RenderWindow> window;
    static const sf::Clock globalClock;

    static AssetManager assetManager;
    static GuiManager guiManager;
    static ObjectManager objectManager;
    static MusicPlayer musicPlayer;
    static Settings settings;
    static Stats stats;
};