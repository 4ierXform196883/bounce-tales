#pragma once

#include <string>
#include <SFML/Audio.hpp>
#include <memory>
#include "timer.hpp"

class MusicPlayer
{
    friend class Game;

public:
    void setMusic(const std::string &name, float volume = 100.0f);
    inline void setVolume(float volume) { music.setVolume(volume); }
    inline void stopMusic() { music.stop(); }
    inline const std::string &getMusicName() { return musicName; }

    const float musicSwitchDuration = 1.5f;

private:
    void switchMusic(const std::string &name, float volume = 100.0f);

    sf::Music music;
    std::string musicName;
    std::shared_ptr<Timer> volumeTimer;
};
