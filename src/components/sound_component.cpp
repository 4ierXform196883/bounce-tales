#include "sound_component.hpp"
#include "randomizer.hpp"
#include "game_object.hpp"
#include <iostream>

SoundComponent::SoundComponent(std::shared_ptr<GameObject> gameObject, const std::map<std::string, sf::Sound> &sounds)
    : GameObjectComponent(gameObject), sounds(sounds) {}

void SoundComponent::play(const std::string &name)
{
    if (sounds.find(name) == sounds.end())
    {
        std::cerr << "[WARN] (SoundComponent::play) No \"" + name + "\" sound found\n";
        return;
    }
    sounds.at(name).play();
}

void SoundComponent::playRandomly(const std::string &name, float intervalMin, float intervalMax)
{
    if (sounds.find(name) == sounds.end())
    {
        std::cerr << "[WARN] (SoundComponent::play) No \"" + name + "\" sound found\n";
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

void SoundComponent::stopPlayingRandomly(const std::string &name)
{
    if (sounds.find(name) == sounds.end())
        std::cerr << "[WARN] (SoundComponent::play) No \"" + name + "\" sound found\n";
    randomTimers.erase(name);
}
void SoundComponent::update()
{
    auto pos = gameObject.lock()->getPositionV();
    for (auto sound : sounds)
    {
        sound.second.setPosition(pos.x, pos.y, 0);
    }
}
