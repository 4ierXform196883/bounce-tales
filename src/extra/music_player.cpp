#include "music_player.hpp"

#include <memory>
#include "timer.hpp"
#include <array>
#include "settings.hpp"
#include "game.hpp"
#include <iostream>

#define abs(x) ((x) > 0 ? (x) : -(x))

void MusicPlayer::changeMusic(const std::string &name, float volume)
{
    if (!newMusic->openFromFile(Game::getSettings().music_path + "/" + name + ".ogg"))
        return;

    newMusic->setLoop(true);
    newMusic->setVolume(0);
    if(curMusic->getStatus() != sf::Music::Playing)
        curMusic->setVolume(0);
    newMusic->play();
    targetVolume = volume > 100 ? 100 : (volume < 0 ? 0 : volume);
    deltaTime = musicSwitchDuration / targetVolume;
    startTime = Game::getClock().getElapsedTime().asSeconds();
}

void MusicPlayer::update()
{
    float curVolume = curMusic->getVolume();
    bool newPlaying = newMusic->getStatus() == sf::Music::Playing;
    if (!newPlaying && curVolume == targetVolume)
        return;
    if (Game::getClock().getElapsedTime().asSeconds() - startTime > deltaTime)
        return;

    if (newPlaying && curVolume != 0.0f)
        curMusic->setVolume(std::max(curVolume - 1, 0.0f));
    else if (newPlaying)
    {
        std::swap(curMusic, newMusic);
        newMusic->stop();
    }
    else
        curMusic->setVolume(std::min(curMusic->getVolume() + 1, targetVolume));
    startTime = Game::getClock().getElapsedTime().asSeconds();
}

std::shared_ptr<sf::Music> MusicPlayer::getCurrentMusic() const
{
    return curMusic;
}
