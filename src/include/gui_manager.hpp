#pragma once

#include <memory>
#include <TGUI/TGUI.hpp>
#include <TGUI/AllWidgets.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

class GuiManager
{
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

    void setUI(GuiManager::UI ui); // IMPLEMENT

private:
    void connectMenuCallbacks();
    void connectSettingsCallbacks();
    void connectLevelCallbacks();
    void connectEditorCallbacks();

    std::unique_ptr<tgui::Gui> gui;
};