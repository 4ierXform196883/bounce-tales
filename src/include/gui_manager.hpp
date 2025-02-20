#pragma once

#include <memory>
#include <TGUI/TGUI.hpp>
#include <TGUI/AllWidgets.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

// IMPLEMENT
class GuiManager
{
    friend class Game;
    enum UI
    {
        MENU,
        SETTINGS,
        LEVEL,
        EDITOR
    };

public:
    const std::string menu_ui_path = "assets/ui/menu.form";
    const std::string settings_ui_path = "assets/ui/settings.form";
    const std::string level_ui_path = "assets/ui/level.form";
    const std::string editor_ui_path = "assets/ui/editor.form";
    const std::string pause_ui_path = "assets/ui/pause.form";

private:
    void setUI(GuiManager::UI ui);
    void connectMenuCallbacks();
    void connectSettingsCallbacks();
    void connectLevelCallbacks();
    void connectEditorCallbacks();

    std::unique_ptr<tgui::Gui> gui;
};