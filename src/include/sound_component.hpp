#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include "game_object_component.hpp"
#include "timer.hpp"

class SoundComponent : private GameObjectComponent
{
    friend GameObject;

public:
    SoundComponent(std::shared_ptr<GameObject> gameObject, const std::map<std::string, sf::Sound> &sounds);
    virtual ~SoundComponent() = default;

    void play(const std::string &name);
    void playRandomly(const std::string &name, float intervalMin, float intervalMax = 0);
    void stopPlayingRandomly(const std::string &name);
    sf::Sound &getSound(const std::string &name) { return sounds.at(name); }
    const sf::Sound &getSound(const std::string &name) const { return sounds.at(name); }

protected:
    void update();
    
    std::map<std::string, std::shared_ptr<Timer>> randomTimers;
    std::map<std::string, sf::Sound> sounds;
};