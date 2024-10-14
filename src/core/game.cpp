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

std::unique_ptr<sf::RenderWindow> Game::window;
sf::View Game::camera;
sf::View Game::uiCamera;
const sf::Clock Game::globalClock;

void Game::init()
{
    settings::load(settings::settings_path);
    AssetManager::loadTextures(settings::textures_path);
    AssetManager::loadSounds(settings::sounds_path);
    sf::Vector2i windowSize = {settings::getInt("Screen", "screenWidth", 1280), settings::getInt("Screen", "screenHeight", 720)};
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), settings::title, sf::Style::Default);
    window->setFramerateLimit(settings::getInt("Screen", "maxFps", 60));
    // window->setFramerateLimit(1);
    window->setVerticalSyncEnabled(settings::getBool("Screen", "vSync", true));

    camera.setCenter(0, 0);
    camera.setSize(windowSize.x, windowSize.y);
    uiCamera.setCenter(0, 0);
    uiCamera.setSize(windowSize.x, windowSize.y);
    window->setView(camera);

    ObjectManager::load("idk for now");

    // std::vector<sf::Vertex> verts;
    // verts.push_back(sf::Vertex({0.0f, 0.0f}, {0.0f, 0.0f}));
    // verts.push_back(sf::Vertex({50.0f, -50.0f}, {50.0f, -50.0f}));
    // verts.push_back(sf::Vertex({75.0f, -40.0f}, {75.0f, -40.0f}));
    // verts.push_back(sf::Vertex({100.0f, -150.0f}, {100.0f, -150.0f}));
    // verts.push_back(sf::Vertex({150.0f, -30.0f}, {150.0f, -30.0f}));
    // verts.push_back(sf::Vertex({200.0f, -100.0f}, {200.0f, -100.0f}));
    // // test = std::make_shared<TestImpl>("testObj", std::make_shared<PrimitiveSprite>(asset_manager::getTexture("JumpPad"), asset_manager::getSpriteBounds("JumpPad", "0")));
    // test = std::make_shared<TestImpl>("testObj", std::make_shared<CurvedShape>(asset_manager::getTexture("HideZone1"), verts));
    // std::shared_ptr<TestImpl> test2 = std::make_shared<TestImpl>("testChild", std::make_shared<PrimitiveSprite>(asset_manager::getTexture("Redy")));
    // test->addChild(test2);
    // // test->setPositionV(640, 360);
    // auto tSize = asset_manager::getSpriteBounds("JumpPad", "0").getSize();
    // auto tSize2 = asset_manager::getTexture("Redy").getSize();
    // // test->setOriginV(tSize.x / 2.0f, tSize.y);
    // test2->setOriginV(tSize2.x / 2.0f, tSize2.y / 2.0f);

    // test2->setPositionV(0, -50);
    // window->setView(camera);
}

void Game::tick()
{
    processEvents();
    Timer::updateAll();
    music_player::update();
    ObjectManager::updateAll();
    render();
}

void Game::close()
{
    AssetManager::unloadTextures();
    AssetManager::unloadSounds();
    settings::save(settings::settings_path);
}

void Game::processEvents()
{
    for (auto event = sf::Event{}; window->pollEvent(event);)
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::F1)
                music_player::changeMusic("menu");
            if (event.key.code == sf::Keyboard::F2)
                music_player::changeMusic("level");
            if (event.key.code == sf::Keyboard::F3)
                music_player::changeMusic("level2");
            if (event.key.code == sf::Keyboard::F4)
                music_player::changeMusic("level3");
            if (event.key.code == sf::Keyboard::W)
            {
                camera.move(0, -16);
                window->setView(camera);
            }
            if (event.key.code == sf::Keyboard::S)
            {
                camera.move(0, 16);
                window->setView(camera);
            }
            if (event.key.code == sf::Keyboard::A)
            {
                camera.move(-16, 0);
                window->setView(camera);
            }
            if (event.key.code == sf::Keyboard::D)
            {
                camera.move(16, 0);
                window->setView(camera);
            }
            if (event.key.code == sf::Keyboard::F)
            {
                camera.setSize(camera.getSize() - sf::Vector2f(16, 9));
                uiCamera.setSize(uiCamera.getSize() - sf::Vector2f(16, 9));
            }
            if (event.key.code == sf::Keyboard::G)
            {
                camera.setSize(camera.getSize() + sf::Vector2f(16, 9));
                uiCamera.setSize(uiCamera.getSize() + sf::Vector2f(16, 9));
            }
            break;

        default:
            break;
        }
    }
}

void Game::render()
{
    window->clear(sf::Color::Black);
    window->setView(uiCamera);
    ObjectManager::drawBackground(*window);

    window->setView(camera);
    ObjectManager::drawObjects(*window);

    window->setView(uiCamera);
    ObjectManager::drawUI(*window);

    window->setView(camera);
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