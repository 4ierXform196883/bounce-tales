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

void AssetManager::loadFromJSON(const fs::path &path)
{
    std::string filename = path.stem().generic_string();
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] (AssetManager::loadTextures) Couldn't open file\n";
        return;
    }
    json j;
    file >> j;
    // SpriteBounds
    for (auto &[key, value] : j.items())
    {
        if (value.is_array() && !value[0].is_string())
        {
            spriteBounds[filename][key] = sf::IntRect(value[0], value[1], value[2], value[3]);
        }
    }
    // AnimationBounds
    for (auto &[key, value] : j.items())
    {
        if (value.is_array() && value[0].is_string())
        {
            for (auto subkey : value)
            {
                if (spriteBounds.find(filename) == spriteBounds.end() || spriteBounds.at(filename).find(subkey) == spriteBounds.at(filename).end())
                    std::cerr << "[WARN] (AssetManager::loadTextures) No \"" + static_cast<std::string>(subkey) + "\" frame found for \"" + filename + "\"\n";
                animationBounds[filename][key].push_back(spriteBounds[filename][static_cast<std::string>(subkey)]);
            }
        }
    }
    file.close();
}

void AssetManager::loadTextures(const std::string &path)
{
    textures["NULL"] = sf::Texture();
    spriteBounds["NULL"]["NULL"] = sf::IntRect(0, 0, 100, 100);
    animationBounds["NULL"]["NULL"].push_back(sf::IntRect(0, 0, 100, 100));
    
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
            textures[filename].setSmooth(true);
        }
    }
}

void AssetManager::loadSounds(const std::string &path)
{
    for (auto &entry : fs::directory_iterator(path))
    {
        if (entry.path().extension() != ".ogg")
            continue;

        std::string filename = entry.path().stem().generic_string();
        soundBuffers[filename].loadFromFile(entry.path().generic_string());
    }
}

void AssetManager::unloadTextures()
{
    textures.clear();
}

void AssetManager::unloadSounds()
{
    soundBuffers.clear();
}

const sf::SoundBuffer &AssetManager::getSoundBuffer(const std::string &name) const
{
    return soundBuffers.at(name);
}

const sf::Texture &AssetManager::getTexture(const std::string &name) const
{
    if (textures.find(name) == textures.end())
    {
        std::cerr << "[WARN] (AssetManager::getTexture) No \"" + name + "\" texture found\n";
        return textures.at("NULL");
    }
    return textures.at(name);
}

const sf::IntRect &AssetManager::getSpriteBounds(const std::string &textureName, const std::string &subtextureName) const
{
    if (spriteBounds.find(textureName) == spriteBounds.end())
    {
        std::cerr << "[WARN] (AssetManager::getSpriteBounds) No \"" + textureName + "\" texture found\n";
        return spriteBounds.at("NULL").at("NULL");
    }
    else if (spriteBounds.at(textureName).find(subtextureName) == spriteBounds.at(textureName).end())
    {
        std::cerr << "[WARN] (AssetManager::getSpriteBounds) No bounds for \"" + textureName + "." + subtextureName + "\" found\n";
        return spriteBounds.at("NULL").at("NULL");
    }
    return spriteBounds.at(textureName).at(subtextureName);
}

const std::vector<sf::IntRect> &AssetManager::getAnimationBounds(const std::string &textureName, const std::string &animationName) const
{
    if (animationBounds.find(textureName) == animationBounds.end())
    {
        std::cerr << "[WARN] (AssetManager::getAnimationBounds) No \"" + textureName + "\" texture found\n";
        return animationBounds.at("NULL").at("NULL");
    }
    else if (animationBounds.at(textureName).find(animationName) == animationBounds.at(textureName).end())
    {
        std::cerr << "[WARN] (AssetManager::getAnimationBounds) No bounds for \"" + textureName + "." + animationName + "\" found\n";
        return animationBounds.at("NULL").at("NULL");
    }
    return animationBounds.at(textureName).at(animationName);
}

bool AssetManager::hasTexture(const std::string &name) const
{
    return textures.find(name) != textures.end();
}

bool AssetManager::hasSpriteBounds(const std::string &textureName, const std::string &subtextureName) const
{
    return spriteBounds.find(textureName) != spriteBounds.end();
}

bool AssetManager::hasAnimationBounds(const std::string &textureName, const std::string &animationName) const
{
    return animationBounds.find(textureName) != animationBounds.end();
}
