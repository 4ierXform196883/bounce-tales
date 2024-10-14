#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include "timer.hpp"

class SoundPlayer
{
    friend class Transformable;

public:
    SoundPlayer(const std::map<std::string, sf::Sound> &sounds);
    virtual ~SoundPlayer() = default;

    void play(const std::string &name);
    void playRandomly(const std::string &name, float intervalMin, float intervalMax = 0);
    void stopPlayingRandomly(const std::string &name);
    inline sf::Sound &getSound(const std::string &name) { return sounds.at(name); }
    inline const sf::Sound &getSound(const std::string &name) const { return sounds.at(name); }

private:
    void posUpdate(const sf::Vector2f &pos);

    std::map<std::string, std::shared_ptr<Timer>> randomTimers;
    std::map<std::string, sf::Sound> sounds;
};