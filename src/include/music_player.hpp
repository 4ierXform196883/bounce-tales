#pragma once

#include <string>
#include <SFML/Audio.hpp>
#include <memory>

namespace music_player
{
    const float musicSwitchDuration = 3.0f;
    void changeMusic(const std::string& name, float volume = 100.0f);
    void update();
    std::shared_ptr<sf::Music> getCurrentMusic();
}
