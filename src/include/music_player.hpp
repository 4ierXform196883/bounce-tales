#pragma once

#include <string>
#include <SFML/Audio.hpp>
#include <memory>

class MusicPlayer
{
    friend class Game;

public:
    void changeMusic(const std::string &name, float volume = 100.0f);

    std::shared_ptr<sf::Music> getCurrentMusic();

    const float musicSwitchDuration = 3.0f;

private:
    void update();

    std::shared_ptr<sf::Music> curMusic = std::make_shared<sf::Music>();
    std::shared_ptr<sf::Music> newMusic = std::make_shared<sf::Music>();
    float deltaTime;
    float startTime;
    float targetVolume;
};
