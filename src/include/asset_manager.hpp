#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace asset_manager
{
    void loadTextures(const std::string& path);
    void loadSounds(const std::string& path);
    void unloadTextures();
    void unloadSounds();

    const sf::SoundBuffer& getSoundBuffer(const std::string& name);
    
    sf::Texture& getTexture(const std::string& name);
    sf::IntRect getSpriteBounds(const std::string& textureName, const std::string& subtextureName);
    std::vector<sf::IntRect> getAnimationBounds(const std::string& textureName, const std::string& animationName);
}