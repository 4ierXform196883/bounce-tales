#include "sound_player.hpp"

#include "randomizer.hpp"
#include "transformable.hpp"
#include <iostream>

SoundPlayer::SoundPlayer(const std::map<std::string, sf::Sound> &sounds)
    : sounds(sounds) {}

void SoundPlayer::play(const std::string &name)
{
    if (sounds.find(name) == sounds.end())
    {
        std::cerr << "[WARN] (SoundPlayer::play) No \"" + name + "\" sound found\n";
        return;
    }
    sounds.at(name).play();
}

void SoundPlayer::playRandomly(const std::string &name, float intervalMin, float intervalMax)
{
    if (sounds.find(name) == sounds.end())
    {
        std::cerr << "[WARN] (SoundPlayer::play) No \"" + name + "\" sound found\n";
        return;
    }
    intervalMin = std::max(sounds.at(name).getBuffer()->getDuration().asSeconds(), intervalMin);
    float duration = intervalMax ? randomizer::getRandFloat(intervalMin, intervalMax) : intervalMin;
    randomTimers[name] = Timer::create(duration, [this, name, intervalMin, intervalMax]()
                                       { sounds[name].play();
                                         if (!intervalMax) return;
                                         float duration = randomizer::getRandFloat(intervalMin, intervalMax);
                                         randomTimers[name]->duration = duration; });
}

void SoundPlayer::stopPlayingRandomly(const std::string &name)
{
    if (sounds.find(name) == sounds.end())
        std::cerr << "[WARN] (SoundPlayer::play) No \"" + name + "\" sound found\n";
    randomTimers.erase(name);
}

void SoundPlayer::posUpdate(const sf::Vector2f &pos)
{
    for (auto sound : sounds)
    {
        sound.second.setPosition(pos.x, pos.y, 0);
    }
}
