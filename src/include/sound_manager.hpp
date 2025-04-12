#pragma once

#include <string>
#include <SFML/Audio.hpp>
#include <memory>
#include "timer.hpp"

class SoundManager
{
    friend class Game;

public:
    void setMusic(const std::string &name, float volume = 100.0f);
    inline void setMusicVolume(float volume) { music->setVolume(volume); }
    inline void stopMusic() { music->stop(); }
    inline const std::string &getMusicName() { return musicName; }

    void playSound(const std::string &name, float volume = 100.0f);
    inline void setSoundVolume(float volume) { sound->setVolume(volume); }
    inline void stopSound() { sound->stop(); }

    const float musicSwitchDuration = 1.5f;

private:
    void init();
    void switchMusic(const std::string &name, float volume = 100.0f);

    std::shared_ptr<sf::Music> music;
    std::shared_ptr<sf::Sound> sound;
    std::string musicName;
    std::shared_ptr<Timer> volumeTimer;
};
