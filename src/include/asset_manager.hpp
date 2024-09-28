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
    AssetManager() = delete;
    ~AssetManager() = delete;

    static const sf::SoundBuffer &getSoundBuffer(const std::string &name);

    static const sf::Texture &getTexture(const std::string &name);
    static const sf::IntRect &getSpriteBounds(const std::string &textureName, const std::string &subtextureName);
    static const std::vector<sf::IntRect> &getAnimationBounds(const std::string &textureName, const std::string &animationName);

private:
    static void loadTextures(const std::string &path);
    static void loadSounds(const std::string &path);
    static void unloadTextures();
    static void unloadSounds();
    static void loadFromJSON(const fs::path &path);

    static std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    static std::unordered_map<std::string, std::string> musicPaths;

    static std::unordered_map<std::string, sf::Texture> textures;
    static std::unordered_map<std::string, std::unordered_map<std::string, sf::IntRect>> spriteBounds;
    static std::unordered_map<std::string, std::unordered_map<std::string, std::vector<sf::IntRect>>> animationBounds;
};