#pragma once

#include <string>
#include <map>

using IniSection = std::map<std::string, std::string>;
using IniFile = std::map<std::string, IniSection>;

class Settings
{
public:
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

    std::string getString(const std::string& section, const std::string& key, const std::string& defaultValue = "") const;
    double getDouble(const std::string& section, const std::string& key, double defaultValue = 0.0) const;
    int getInt(const std::string& section, const std::string& key, int defaultValue = 0) const;
    bool getBool(const std::string& section, const std::string& key, bool defaultValue = false) const;

private:
    IniFile iniFile;
};