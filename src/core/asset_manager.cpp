#include "asset_manager.hpp"

#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;

std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
std::unordered_map<std::string, std::string> musicPaths;

std::unordered_map<std::string, sf::Texture> textures;
std::unordered_map<std::string, std::unordered_map<std::string, sf::IntRect>> spriteBounds;
std::unordered_map<std::string, std::unordered_map<std::string, std::vector<sf::IntRect>>> animationBounds;

void loadFromJSON(const fs::path &path)
{
    std::string filename = path.stem().generic_string();
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] (asset_manager::loadTextures) Couldn't open file\n";
        return;
    }
    json j;
    file >> j;
    for (auto &[key, value] : j.items())
    {
        if (value.is_array() && value[0].is_string())
        {
            for (auto subkey : value)
            {
                if (spriteBounds.find(filename) == spriteBounds.end() || spriteBounds.at(filename).find(subkey) == spriteBounds.at(filename).end())
                    std::cerr << "[WARN] (asset_manager::loadTextures) No \"" + static_cast<std::string>(subkey) + "\" frame found for \"" + filename + "\"\n";
                animationBounds[filename][key].push_back(spriteBounds[filename][static_cast<std::string>(subkey)]);
            }
        }
        else if (value.is_array() && value.size() == 4)
        {
            spriteBounds[filename][key] = sf::IntRect(value[0], value[1], value[2], value[3]);
        }
        else
        {
            std::cerr << "[WARN] (asset_manager::loadTextures) Incorrect JSON entry with key \"" + key + "\"\n";
            continue;
        }
    }
    file.close();
}

void asset_manager::loadTextures(const std::string &path)
{
    textures["NULL"] = sf::Texture();
    for (auto &entry : fs::directory_iterator(path))
    {
        if (entry.path().extension() == ".json")
        {
            loadFromJSON(entry.path());
        }
        else if (entry.path().extension() == ".png")
        {
            std::string filename = entry.path().stem().generic_string();
            textures[filename].loadFromFile(entry.path().generic_string());
            textures[filename].setRepeated(true);
        }
    }
}

void asset_manager::loadSounds(const std::string &path)
{
    for (auto &entry : fs::directory_iterator(path))
    {
        if (entry.path().extension() != ".ogg")
            continue;

        std::string filename = entry.path().stem().generic_string();
        soundBuffers[filename].loadFromFile(entry.path().generic_string());
    }
}

void asset_manager::unloadTextures()
{
    textures.clear();
}

void asset_manager::unloadSounds()
{
    soundBuffers.clear();
}

const sf::SoundBuffer &asset_manager::getSoundBuffer(const std::string &name)
{
    return soundBuffers[name];
}

sf::Texture &asset_manager::getTexture(const std::string &name)
{
    if (textures.find(name) == textures.end())
        std::cerr << "[WARN] (asset_manager::getTexture) No \"" + name + "\" texture found\n";
    return textures[name];
}

sf::IntRect asset_manager::getSpriteBounds(const std::string &textureName, const std::string &subtextureName)
{
    if (spriteBounds.find(textureName) == spriteBounds.end())
        std::cerr << "[WARN] (asset_manager::getSpriteBounds) No \"" + textureName + "\" texture found\n";
    else if (spriteBounds.at(textureName).find(subtextureName) == spriteBounds.at(textureName).end())
        std::cerr << "[WARN] (asset_manager::getSpriteBounds) No bounds for\"" + textureName + "." + subtextureName + "\" found\n";
    return spriteBounds[textureName][subtextureName];
}

std::vector<sf::IntRect> asset_manager::getAnimationBounds(const std::string &textureName, const std::string &animationName)
{
    if (animationBounds.find(textureName) == animationBounds.end())
    {
        animationBounds[textureName][animationName].push_back(sf::IntRect());
        std::cerr << "[WARN] (asset_manager::getAnimationBounds) No \"" + textureName + "\" texture found\n";
    }
    else if (animationBounds.at(textureName).find(animationName) == animationBounds.at(textureName).end())
    {
        animationBounds[textureName][animationName].push_back(sf::IntRect());
        std::cerr << "[WARN] (asset_manager::getAnimationBounds) No bounds for\"" + textureName + "." + animationName + "\" found\n";
    }
    return animationBounds[textureName][animationName];
}
