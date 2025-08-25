#include "sound_manager.hpp"

#include <memory>
#include "timer.hpp"
#include <array>
#include "settings.hpp"
#include "game.hpp"
#include <iostream>

void SoundManager::setMusic(const std::string &name, float volume)
{
    this->musicName = name;
    if (music->getStatus() == sf::Music::Playing && music->getVolume() != 0)
    {
        auto callback = [this, name, volume]
        {
            music->setVolume(music->getVolume() - 1);
            if (music->getVolume() <= 1)
                switchMusic(name, volume);
        };
        volumeTimer = Timer::create(musicSwitchDuration / music->getVolume(), callback);
    }
    else
    {
        switchMusic(name, volume);
    }
}

void SoundManager::playSound(const std::string &name, float volume)
{
    this->sound->setBuffer(Game::getAssetManager()->getSoundBuffer(name));
    this->sound->setVolume(volume);
    this->sound->play();
}

void SoundManager::init()
{
    this->music = std::make_shared<sf::Music>();
    this->sound = std::make_shared<sf::Sound>();
}

void SoundManager::switchMusic(const std::string &name, float volume)
{
    music->stop();
    music->openFromFile(Game::getSettings()->music_path + "/" + name + ".ogg");
    music->setLoop(true);
    music->setVolume(0);
    music->play();
    auto callback = [this, name, volume]
    {
        music->setVolume(music->getVolume() + 1);
        if (music->getVolume() >= volume - 1)
            volumeTimer = nullptr;
    };
    volumeTimer = Timer::create(musicSwitchDuration / volume, callback);
}