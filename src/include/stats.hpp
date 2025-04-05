#pragma once

#include <string>
#include <nlohmann/json.hpp>

class Stats
{
    friend class Game;

public:
    void set(const std::string &level, const std::string &stat, int value);
    int get(const std::string &level, const std::string &stat) const;
    std::string getAsString(const std::string &level, const std::string &stat) const;
    
    float currentStartTime;
    int currentTotalEggs, currentEggs;
    std::string currentLevelName;

private:
    void load(const std::string &path);
    void save(const std::string &path);
    void reset();

    nlohmann::json data;
};