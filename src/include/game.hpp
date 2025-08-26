#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "asset_manager.hpp"
#include "object_manager.hpp"
#include "settings.hpp"
#include "sound_manager.hpp"
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
    inline static sf::Vector2f getLocalMousePos() { return {(float)sf::Mouse::getPosition(*window).x, (float)sf::Mouse::getPosition(*window).y}; }

    inline static std::unique_ptr<AssetManager> &getAssetManager() { return assetManager; }
    inline static std::unique_ptr<ObjectManager> &getObjectManager() { return objectManager; }
    inline static std::unique_ptr<SoundManager> &getSoundManager() { return soundManager; }
    inline static std::unique_ptr<Settings> &getSettings() { return settings; }
    inline static std::unique_ptr<Stats> &getStats() { return stats; }
    inline static bool isEditorMode() { return editorMode; }

    static void reinitWindow(const sf::Vector2i &resolution, bool fullscreen);
    static void loadLevel(const std::string &name, bool editorMode = false);
    static void close();

    static bool paused;
    static bool running;

private:
    static void init();
    static void tick();
    static void processEvents();
    static void render();

    static bool editorMode;

    static std::unique_ptr<sf::RenderWindow> window;
    static const sf::Clock globalClock;

    static std::unique_ptr<AssetManager> assetManager;
    static std::unique_ptr<GuiManager> guiManager;
    static std::unique_ptr<ObjectManager> objectManager;
    static std::unique_ptr<SoundManager> soundManager;
    static std::unique_ptr<Settings> settings;
    static std::unique_ptr<Stats> stats;
};