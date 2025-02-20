#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

class AssetManager
{
    friend class Game;

public:
    const sf::SoundBuffer &getSoundBuffer(const std::string &name) const;
    const sf::Texture &getTexture(const std::string &name) const;
    const sf::IntRect &getSpriteBounds(const std::string &textureName, const std::string &subtextureName) const;
    const std::vector<sf::IntRect> &getAnimationBounds(const std::string &textureName, const std::string &animationName) const;

private:
    void loadTextures(const std::string &path);
    void loadSounds(const std::string &path);
    void unloadTextures();
    void unloadSounds();

    void loadFromJSON(const fs::path &path);

    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, std::string> musicPaths;

    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, std::unordered_map<std::string, sf::IntRect>> spriteBounds;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<sf::IntRect>>> animationBounds;
};