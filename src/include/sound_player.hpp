#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include "timer.hpp"

class ISoundPlayer
{
public:
    virtual void play(const std::string &name, float volume) = 0;
    virtual void playRandomly(const std::string &name, float intervalMin, float intervalMax = 0) = 0;
    virtual void stopPlayingRandomly(const std::string &name) = 0;
    virtual sf::Sound &getSound(const std::string &name) = 0;
    virtual const sf::Sound &getSound(const std::string &name) const = 0;
};

class SoundPlayer : public ISoundPlayer
{
public:
    SoundPlayer(const std::map<std::string, sf::Sound> &sounds);
    virtual ~SoundPlayer() = default;

    virtual void play(const std::string &name, float volume = 100.f) override;
    virtual void playRandomly(const std::string &name, float intervalMin, float intervalMax = 0) override;
    virtual void stopPlayingRandomly(const std::string &name) override;
    inline virtual sf::Sound &getSound(const std::string &name) override { return sounds.at(name); }
    inline virtual const sf::Sound &getSound(const std::string &name) const override { return sounds.at(name); }
    
    void update(const sf::Vector2f &pos);

    std::map<std::string, std::shared_ptr<Timer>> randomTimers;
    std::map<std::string, sf::Sound> sounds;
};