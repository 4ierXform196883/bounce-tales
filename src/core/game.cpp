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
#include "theme.hpp"

AssetManager Game::assetManager;
ObjectManager Game::objectManager;
MusicPlayer Game::musicPlayer;
Settings Game::settings;

std::unique_ptr<sf::RenderWindow> Game::window;
sf::View Game::camera;
sf::View Game::uiCamera;
std::unique_ptr<tgui::Gui> Game::gui;
const sf::Clock Game::globalClock;

void Game::init()
{
    settings.load(settings.settings_path);
    assetManager.loadTextures(settings.textures_path);
    assetManager.loadSounds(settings.sounds_path);
    sf::Vector2i windowSize = {settings.getInt("Screen", "screenWidth", 1920), settings.getInt("Screen", "screenHeight", 1080)};
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), settings.title, sf::Style::Fullscreen);
    window->setFramerateLimit(settings.getInt("Screen", "maxFps", 60));
    // window->setFramerateLimit(1);
    window->setVerticalSyncEnabled(settings.getBool("Screen", "vSync", true));

    camera.setCenter(0, 0);
    camera.setSize(1920, 1080);
    uiCamera.setCenter(0, 0);
    uiCamera.setSize(1920, 1080);
    window->setView(camera);

    objectManager.load("idk for now");
    gui = std::make_unique<tgui::Gui>(*window);
    // tgui::Theme::getDefault()->replace(Theme());
    gui->loadWidgetsFromFile("form.txt");
    // auto w1 = tgui::Button::create("Play");
    // w1->setPosition(300, 300);
    // w1->setSize(300, 100);
    // w1->setTextSize(30);
    // w1->onPress([&] {window->close(); });

    // gui->add(w1);

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
    musicPlayer.update();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        camera.move(0, -10);
        window->setView(camera);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        camera.move(0, 10);
        window->setView(camera);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        camera.move(-10, 0);
        window->setView(camera);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        camera.move(10, 0);
        window->setView(camera);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
    {
        camera.setSize(camera.getSize() - sf::Vector2f(16, 9));
        uiCamera.setSize(uiCamera.getSize() - sf::Vector2f(16, 9));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G))
    {
        camera.setSize(camera.getSize() + sf::Vector2f(16, 9));
        uiCamera.setSize(uiCamera.getSize() + sf::Vector2f(16, 9));
    }
    objectManager.updateAll();
    objectManager.collideAll();
    objectManager.moveAll();
    render();
}

void Game::close()
{
    assetManager.unloadTextures();
    assetManager.unloadSounds();
    settings.save(settings.settings_path);
}

void Game::processEvents()
{
    for (auto event = sf::Event{}; window->pollEvent(event);)
    {
        gui->handleEvent(event);
        switch (event.type)
        {
        case sf::Event::Closed:
            window->close();
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
    window->setView(uiCamera);
    objectManager.drawBackground(*window);

    window->setView(camera);
    objectManager.drawObjects(*window);

    window->setView(uiCamera);
    objectManager.drawUI(*window);
    gui->draw();

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