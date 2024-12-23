#pragma once

#include <string>
#include <SFML/Audio.hpp>
#include <memory>

class MusicPlayer
{
public:
    void changeMusic(const std::string &name, float volume = 100.0f);
    void update();

    std::shared_ptr<sf::Music> getCurrentMusic() const;

    const float musicSwitchDuration = 3.0f;

private:
    std::shared_ptr<sf::Music> curMusic = std::make_shared<sf::Music>();
    std::shared_ptr<sf::Music> newMusic = std::make_shared<sf::Music>();
    float deltaTime;
    float startTime;
    float targetVolume;
};
