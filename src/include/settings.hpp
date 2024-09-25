#pragma once

#include <string>

namespace settings
{
    const std::string title = "Bounce Tales";
    const std::string settings_path = "settings.ini";
    const std::string textures_path = "assets/textures";
    const std::string sounds_path = "assets/sounds";
    const std::string music_path = "assets/music";
    
    void load(const std::string& path);
    void save(const std::string& path);

    void set(const std::string& section, const std::string& key, const std::string& value);
    void set(const std::string& section, const std::string& key, double value);
    void set(const std::string& section, const std::string& key, int value);
    void set(const std::string& section, const std::string& key, bool value);

    std::string getString(const std::string& section, const std::string& key, const std::string& defaultValue = "");
    double getDouble(const std::string& section, const std::string& key, double defaultValue = 0.0);
    int getInt(const std::string& section, const std::string& key, int defaultValue = 0);
    bool getBool(const std::string& section, const std::string& key, bool defaultValue = false);
}