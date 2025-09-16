#include "stats.hpp"

#include <fstream>
#include <iostream>
#include "game.hpp"

void Stats::set(const std::string &level, const std::string &stat, int value)
{
  data[level][stat] = value;
}

int Stats::get(const std::string &level, const std::string &stat) const
{
  if (data.contains(level) && data.at(level).contains(stat))
    return data.at(level).at(stat);
  return 0;
}

std::string Stats::getAsString(const std::string &level, const std::string &stat) const
{
  int value = this->get(level, stat);
  if (stat == "best_time")
  {
    std::string minutes = std::to_string(value / 60);
    minutes = minutes.size() == 1 ? "0" + minutes : minutes;
    std::string seconds = std::to_string(value % 60);
    seconds = seconds.size() == 1 ? "0" + seconds : seconds;
    return minutes + ":" + seconds;
  }

  else if (stat == "best_eggs" || stat == "total_eggs")
  {
    std::string bestEggs = std::to_string(this->get(level, "best_eggs"));
    std::string totalEggs = std::to_string(this->get(level, "total_eggs"));
    return bestEggs + "/" + totalEggs;
  }
  return "";
}

void Stats::loadTotalEggs()
{
  for (auto &entry : fs::directory_iterator(Game::getSettings()->levels_path))
  {
    if (!entry.is_regular_file() || entry.path().extension().generic_string() != ".json")
      continue;

    std::ifstream file(entry.path().generic_string());
    if (!file.is_open())
    {
      std::cerr << "[ERROR] (Stats::load) Couldn't open level file\n";
      continue;
    }
    nlohmann::json levelData;
    file >> levelData;
    file.close();
    if (!levelData.contains("triggers") && !levelData.contains("enemies"))
      continue;
    int totalCount = 0;
    if (levelData.contains("triggers"))
      totalCount += std::count_if(levelData.at("triggers").begin(), levelData.at("triggers").end(), [](const nlohmann::json &item)
                                  { return item.contains("type") && item["type"] == "egg"; });
    if (levelData.contains("enemies"))
      totalCount += levelData.at("enemies").size();
    data[entry.path().stem().generic_string()]["total_eggs"] = totalCount;
  }
}

void Stats::load(const std::string &path)
{
  loadTotalEggs();
  std::ifstream file(path);
  if (!file.is_open())
  {
    std::cerr << "[ERROR] (Stats::load) Couldn't open file\n";
    return;
  }
  file >> data;
  file.close();
}

void Stats::save(const std::string &path)
{
  std::ofstream file(path);
  if (!file.is_open())
  {
    std::cerr << "[ERROR] (ObjectManager::saveStats) Couldn't open file\n";
    return;
  }
  file << data;
  file.close();
}

void Stats::reset()
{
  currentEggs = 0;
  currentStartTime = Game::getClock().getElapsedTime().asSeconds();
  currentLevelName = "menu";
}
