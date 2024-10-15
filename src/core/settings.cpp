#include "settings.hpp"
#include <map>

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

void Settings::load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ERROR] (Settings::load) Couldn't open file\n";
        return;
    }
    std::string currentSection;
    std::regex sectionRegex(R"(\[\s*(\w+)\s*\])");
    std::regex keyValueRegex(R"((\w+)\s*=\s*(.*))");
    std::regex commentRegex(R"(;.*)");
    std::string line;

    while (std::getline(file, line)) {
        line = std::regex_replace(line, commentRegex, "");

        std::smatch match;
        if (std::regex_match(line, match, sectionRegex)) {
            currentSection = match[1];
            iniFile[currentSection] = IniSection();
        }
        else if (std::regex_match(line, match, keyValueRegex)) {
            std::string key = match[1];
            std::string value = match[2];
            iniFile[currentSection][key] = value;
        }
    }

    file.close();
}

void Settings::save(const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ERROR] (Settings::save) Couldn't open file\n";
        return;
    }

    for (const auto& section : iniFile) {
        file << "[" << section.first << "]\n";
        for (const auto& kvp : section.second) {
            file << kvp.first << "=" << kvp.second << "\n";
        }
        file << "\n";
    }

    file.close();
}

void Settings::set(const std::string& section, const std::string& key, const std::string& value)
{
    iniFile[section][key] = value;
}

void Settings::set(const std::string &section, const std::string &key, double value)
{
    iniFile[section][key] = std::to_string(value);
}

void Settings::set(const std::string &section, const std::string &key, int value)
{
    iniFile[section][key] = std::to_string(value);
}

void Settings::set(const std::string &section, const std::string &key, bool value)
{
    iniFile[section][key] = std::to_string(value);
}

std::string Settings::getString(const std::string &section, const std::string &key, const std::string &defaultValue) const
{
    if (iniFile.count(section) && iniFile.at(section).count(key)) {
        return iniFile.at(section).at(key);
    }
    std::cerr << "[WARN] (Settings::getString) Key \"" + key + "\" not found\n";
    return defaultValue;
}

double Settings::getDouble(const std::string &section, const std::string &key, double defaultValue) const
{
    std::string valueStr = Settings::getString(section, key);
    try {
        return std::stod(valueStr);
    } catch (const std::invalid_argument& e) {
        std::cerr << "[WARN] (Settings::getDouble) \"" + key + "\" value is not double\n";
        return defaultValue;
    } catch (const std::out_of_range& e) {
        std::cerr << "[WARN] (Settings::getDouble) \"" + key + "\" value is too big\n";
        return defaultValue;
    }
}

int Settings::getInt(const std::string &section, const std::string &key, int defaultValue) const
{
    std::string valueStr = Settings::getString(section, key);
    try {
        return std::stoi(valueStr);
    } catch (const std::invalid_argument& e) {
        std::cerr << "[WARN] (Settings::getInt) \"" + key + "\" value is not int\n";
        return defaultValue;
    } catch (const std::out_of_range& e) {
        std::cerr << "[WARN] (Settings::getInt) \"" + key + "\" value is too big\n";
        return defaultValue;
    }
}

bool Settings::getBool(const std::string &section, const std::string &key, bool defaultValue) const
{
    std::string valueStr = getString(section, key);
    std::transform(valueStr.begin(), valueStr.end(), valueStr.begin(), ::tolower);
    if (valueStr == "true" || valueStr == "1") {
        return true;
    } else if (valueStr == "false" || valueStr == "0") {
        return false;
    } else {
        std::cerr << "[WARN] (Settings::getBool) \"" + key + "\" value is not bool\n";
        return defaultValue;
    }
}