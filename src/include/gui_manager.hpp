#pragma once

#include <memory>
#include <TGUI/TGUI.hpp>
#include <TGUI/AllWidgets.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

// IMPLEMENT
class GuiManager
{
    friend class Game;

public:
    enum UI
    {
        MENU,
        LEVEL,
        EDITOR
    };

    const std::string menu_ui_path = "assets/ui/menu.txt";
    const std::string settings_ui_path = "assets/ui/settings.txt";
    const std::string level_ui_path = "assets/ui/level.txt";
    const std::string editor_ui_path = "assets/ui/editor.txt";
    const std::string pause_ui_path = "assets/ui/pause.txt";

private:
    void init();

    void setUI(GuiManager::UI ui);
    void connectMenuCallbacks();
    void connectLevelCallbacks();
    void connectEditorCallbacks();
    void connectSettingsGroupCallbacks();
    void connectPauseGroupCallbacks();

    std::unique_ptr<tgui::Gui> gui;
    size_t currentLevel = 0;
    std::vector<std::string> levelNames;
    std::vector<std::string> levelPaths;
    std::map<std::string, tgui::Group::Ptr> groups;
    GuiManager::UI currentUI;
};