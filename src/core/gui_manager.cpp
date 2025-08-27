#include "gui_manager.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "game.hpp"

void GuiManager::init()
{
    for (auto &entry : fs::directory_iterator(Game::getSettings()->levels_path))
    {
        if (entry.is_regular_file() && entry.path().extension().generic_string() == ".json" && entry.path().stem().generic_string() != "menu")
        {
            std::ifstream file(entry.path().generic_string());
            if (!file.is_open())
            {
                std::cerr << "[ERROR] (GuiManager::init) Couldn't open level file\n";
                return;
            }
            nlohmann::json data;
            file >> data;
            file.close();
            levelNames.push_back(data["display_name"]);
            levelFilenames.push_back(entry.path().stem().generic_string());
        }
    }
}

void GuiManager::setUI(GuiManager::UI ui)
{
    currentUI = ui;
    switch (ui)
    {
    case MENU:
        gui->loadWidgetsFromFile(menu_ui_path);
        connectMenuCallbacks();
        break;

    case LEVEL:
        gui->loadWidgetsFromFile(level_ui_path);
        connectLevelCallbacks();
        break;

    case EDITOR:
        gui->removeAllWidgets();
        // gui->loadWidgetsFromFile(editor_ui_path);
        connectEditorCallbacks();
        break;

    default:
        break;
    }
}

void GuiManager::connectMenuCallbacks()
{
    // Play button
    auto playCallback = [this]
    {
        Game::loadLevel(levelFilenames.at(currentLevel));
    };
    gui->get<tgui::Button>("button_play")->onPress(playCallback);

    // Levelname group
    groups.emplace("levelname", tgui::Group::create());
    groups.at("levelname")->loadWidgetsFromFile(levelname_ui_path);
    groups.at("levelname")->setVisible(false);
    gui->add(groups.at("levelname"));
    connectLevelnameGroupCallbacks();

    // Settings button
    groups.emplace("settings", tgui::Group::create());
    groups.at("settings")->loadWidgetsFromFile(settings_ui_path);
    groups.at("settings")->setVisible(false);
    gui->add(groups.at("settings"));
    auto settingsCallback = [this]
    {
        groups.at("settings")->setVisible(!groups.at("settings")->isVisible());
    };
    gui->get<tgui::Button>("button_settings")->onPress(settingsCallback);
    connectSettingsGroupCallbacks();

    // Edit button
    auto editCallback = [this]
    {
        if (levelFilenames.at(currentLevel) == "new_template")
        {
            groups.at("settings")->setVisible(false);
            groups.at("levelname")->setVisible(true);
            gui->get<tgui::Button>("button_level_prev")->setEnabled(false);
            gui->get<tgui::Button>("button_level_next")->setEnabled(false);
            gui->get<tgui::Button>("button_settings")->setEnabled(false);
            Game::paused = true;
            return;
        }
        Game::loadLevel(levelFilenames.at(currentLevel), true);
    };
    gui->get<tgui::Button>("button_edit")->onPress(editCallback);

    // Exit button
    auto exitCallback = [this]
    {
        Game::running = false;
    };
    gui->get<tgui::Button>("button_exit")->onPress(exitCallback);

    if (levelNames.size() == 0)
        return;
    gui->get<tgui::Label>("label_level")->setText(levelNames.at(currentLevel));

    // Prev Level button
    auto prevLevelCallback = [this]
    {
        currentLevel = currentLevel == 0 ? levelNames.size() - 1 : currentLevel - 1;
        gui->get<tgui::Label>("label_level")->setText(levelNames.at(currentLevel));
        gui->get<tgui::Label>("label_time")->setText(Game::getStats()->getAsString(levelFilenames.at(currentLevel), "best_time"));
        gui->get<tgui::Label>("label_eggs")->setText(Game::getStats()->getAsString(levelFilenames.at(currentLevel), "best_eggs"));
        gui->get<tgui::Button>("button_play")->setEnabled(levelFilenames.at(currentLevel) != "new_template");
    };
    gui->get<tgui::Button>("button_level_prev")->onPress(prevLevelCallback);

    // Next Level button
    auto nextLevelCallback = [this]
    {
        currentLevel = (currentLevel + 1) % levelNames.size();
        gui->get<tgui::Label>("label_level")->setText(levelNames.at(currentLevel));
        gui->get<tgui::Label>("label_time")->setText(Game::getStats()->getAsString(levelFilenames.at(currentLevel), "best_time"));
        gui->get<tgui::Label>("label_eggs")->setText(Game::getStats()->getAsString(levelFilenames.at(currentLevel), "best_eggs"));
        gui->get<tgui::Button>("button_play")->setEnabled(levelFilenames.at(currentLevel) != "new_template");
    };
    gui->get<tgui::Button>("button_level_next")->onPress(nextLevelCallback);

    // Stats
    gui->get<tgui::Label>("label_time")->setText(Game::getStats()->getAsString(levelFilenames.at(currentLevel), "best_time"));
    gui->get<tgui::Label>("label_eggs")->setText(Game::getStats()->getAsString(levelFilenames.at(currentLevel), "best_eggs"));
}

void GuiManager::connectLevelCallbacks()
{
    // Pause group
    groups.emplace("pause", tgui::Group::create());
    groups.at("pause")->loadWidgetsFromFile(pause_ui_path);
    groups.at("pause")->setVisible(false);
    gui->add(groups.at("pause"));
    connectPauseGroupCallbacks();

    // Time label
    auto timeUpdateCallback = [this]
    {
        float curTime = Game::getClock().getElapsedTime().asSeconds() - Game::getStats()->currentStartTime;
        std::string minutes = std::to_string((int)curTime / 60);
        minutes = minutes.size() == 1 ? "0" + minutes : minutes;
        std::string seconds = std::to_string((int)curTime % 60);
        seconds = seconds.size() == 1 ? "0" + seconds : seconds;
        gui->get<tgui::Label>("label_time")->setText(minutes + ":" + seconds);
    };
    auto timeUpdateTimer = Timer::create(0.05, timeUpdateCallback);
    gui->get<tgui::Label>("label_time")->setUserData(timeUpdateTimer);

    // Eggs label
    auto eggUpdateCallback = [this]
    {
        std::string currentEggs = std::to_string(Game::getStats()->currentEggs);
        std::string totalEggs = std::to_string(Game::getStats()->get(levelFilenames.at(currentLevel), "total_eggs"));
        gui->get<tgui::Label>("label_eggs")->setText(currentEggs + "/" + totalEggs);
    };
    auto eggUpdateTimer = Timer::create(0.05, eggUpdateCallback);
    gui->get<tgui::Label>("label_eggs")->setUserData(eggUpdateTimer);
}

void GuiManager::connectEditorCallbacks()
{
    groups.emplace("pause", tgui::Group::create());
    groups.at("pause")->loadWidgetsFromFile(pause_ui_path);
    groups.at("pause")->setVisible(false);
    gui->add(groups.at("pause"));
    connectPauseGroupCallbacks();
    gui->get<tgui::Button>("button_respawn")->setEnabled(false);
}

void GuiManager::connectSettingsGroupCallbacks()
{
    if (currentUI != MENU)
        return;

    auto &settings = Game::getSettings();

    // Resolution
    tgui::String resolution = settings->getString("Screen", "width", "1280") + "x" + settings->getString("Screen", "height", "720");
    groups.at("settings")->get<tgui::ComboBox>("settings_resolution")->setSelectedItem(resolution);
    auto resolutionChangeCallback = [this](const tgui::String &selectedItem)
    {
        auto &settings = Game::getSettings();
        int width = selectedItem.substr(0, selectedItem.find("x")).toInt(1280);
        int height = selectedItem.substr(selectedItem.find("x") + 1).toInt(720);
        bool fullscreen = settings->getBool("Screen", "fullscreen", false);
        Game::reinitWindow(sf::Vector2i(width, height), fullscreen);
        settings->set("Screen", "width", width);
        settings->set("Screen", "height", height);
    };
    groups.at("settings")->get<tgui::ComboBox>("settings_resolution")->onItemSelect(resolutionChangeCallback);

    // Fullscreen
    bool fullscreen = settings->getBool("Screen", "fullscreen", false);
    groups.at("settings")->get<tgui::CheckBox>("settings_fullscreen")->setChecked(fullscreen);
    auto fullscreenChangeCallback = [this](bool isChecked)
    {
        auto &settings = Game::getSettings();
        int width = settings->getInt("Screen", "width", 1280);
        int height = settings->getInt("Screen", "height", 720);
        Game::reinitWindow(sf::Vector2i(width, height), isChecked);
        settings->set("Screen", "fullscreen", isChecked);
    };
    groups.at("settings")->get<tgui::CheckBox>("settings_fullscreen")->onChange(fullscreenChangeCallback);

    // Music volume
    double musicVolume = settings->getDouble("Volume", "music", 50.0);
    groups.at("settings")->get<tgui::Slider>("settings_music")->setValue(musicVolume);
    auto musicChangeCallback = [this](float newValue)
    {
        auto &settings = Game::getSettings();
        Game::getSoundManager()->setMusicVolume(newValue);
        settings->set("Volume", "music", newValue);
    };
    groups.at("settings")->get<tgui::Slider>("settings_music")->onValueChange(musicChangeCallback);

    // Sounds volume
    double soundsVolume = settings->getDouble("Volume", "sounds", 50.0);
    groups.at("settings")->get<tgui::Slider>("settings_sounds")->setValue(soundsVolume);
    auto soundsChangeCallback = [this](float newValue)
    {
        auto &settings = Game::getSettings();
        settings->set("Volume", "sounds", newValue);
    };
    groups.at("settings")->get<tgui::Slider>("settings_sounds")->onValueChange(soundsChangeCallback);
}

void GuiManager::connectPauseGroupCallbacks()
{
    // Resume button
    auto resumeCallback = [this]
    {
        groups.at("pause")->setVisible(false);
        Game::paused = false;
    };
    gui->get<tgui::Button>("button_resume")->onPress(resumeCallback);

    // Respawn button
    auto respawnCallback = [this]
    {
        Game::getObjectManager()->getPlayer()->onDeath();
        groups.at("pause")->setVisible(false);
        Game::paused = false;
    };
    gui->get<tgui::Button>("button_respawn")->onPress(respawnCallback);

    // Menu button
    auto menuCallback = [this]
    {
        Game::loadLevel("menu");
        groups.at("pause")->setVisible(false);
        Game::paused = false;
    };
    gui->get<tgui::Button>("button_menu")->onPress(menuCallback);

    // Exit button
    auto exitCallback = [this]
    {
        Game::running = false;
    };
    gui->get<tgui::Button>("button_exit")->onPress(exitCallback);
}

void GuiManager::connectLevelnameGroupCallbacks()
{
    if (currentUI != MENU)
        return;

    // Confirm button
    auto confirmCallback = [this]
    {
        std::string levelName = groups.at("levelname")->get<tgui::EditBox>("editbox_level")->getText().toStdString();
        std::string levelFilename = levelName;
        std::transform(levelFilename.begin(), levelFilename.end(), levelFilename.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        std::replace(levelFilename.begin(), levelFilename.end(), ' ', '_');
        std::ofstream file("levels/" + levelFilename + ".json");
        nlohmann::json data;
        data["display_name"] = levelName;
        if (!file.is_open())
        {
            std::cerr << "[ERROR] (GuiManager::confirmCallback) Couldn't open file for writing\n";
            return;
        }
        file << std::setw(4) << data << std::endl;
        file.close();
        Game::paused = false;
        std::cout << levelFilename;
        Game::loadLevel(levelFilename, true);
    };
    gui->get<tgui::Button>("button_confirm")->onPress(confirmCallback);

    // Cancel button
    auto cancelCallback = [this]
    {
        groups.at("levelname")->setVisible(false);
        gui->get<tgui::Button>("button_level_prev")->setEnabled(true);
        gui->get<tgui::Button>("button_level_next")->setEnabled(true);
        gui->get<tgui::Button>("button_settings")->setEnabled(true);
        groups.at("levelname")->get<tgui::EditBox>("editbox_level")->setText("");
        Game::paused = false;
    };
    gui->get<tgui::Button>("button_cancel")->onPress(cancelCallback);
}
