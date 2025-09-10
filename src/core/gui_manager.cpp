#include "gui_manager.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#define round(x, n) (::round(x * pow(10.0f, n)) / pow(10.0f, n))

#include "game.hpp"
#include "editor.hpp"

std::string to_string3(float value)
{
  std::ostringstream out;
  out << std::fixed << std::setprecision(3) << value;
  return out.str();
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
  std::vector<std::string> result;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, delimiter)) {
    result.push_back(token);
  }

  return result;
}

std::string joinVector(const std::vector<std::string>& arr, const std::string& delimiter) {
    std::ostringstream oss;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i != 0) oss << delimiter;
        oss << arr[i];
    }
    return oss.str();
}

nlohmann::json editorConfigValues = nlohmann::json{
  {"pos", {{"type", "hide"}}},
    {"scale", {{"type", "vector"}}},
    {"origin", {{"type", "hide"}}},
    {"rotation", {{"type", "float"}}},
    {"mass", {{"type", "ufloat"}}},
    {"air_resistance", {{"type", "ufloat"}}},
    {"islands", {{"type", "uint"}}},
    {"clouds", {{"type", "uint"}}},
    {"additional_distance", {{"type", "ufloat"}}},
    {"start_pos", {{"type", "hide"}}},
    {"spawn_pos", {{"type", "hide"}}},
    {"control_force", {{"type", "ufloat"}}},
    {"type", {{"type", "hide"} /*, {"values", {"death_zone", "win_zone", "egg", "change_skin:light", "change_skin:normal", "change_skin:heavy"}}*/}},
    {"size", {{"type", "vector"}}},
    {"texture", {{"type", "hide"}}},
    {"subtexture", {{"type", "hide"}}},
    {"animation", {{"type", "hide"}}},
    {"tag", {{"type", "string"}}},
    {"verts", {{"type", "hide"}}},
    {"path", {{"type", "hide"}}},
    {"speed_mult", {{"type", "ufloat"}}},
    {"power", {{"type", "ufloat"}}},
    {"count", {{"type", "int"}}},
    {"elevation", {{"type", "ufloat"}}},
    {"doors", {{"type", "array"}}},
    {"is_on", {{"type", "bool"}}},
    {"direction", {{"type", "vector"}}},
    {"bezier_verts", {{"type", "hide"}}},
    {"follow", {{"type", "string"}}},
};

void GuiManager::setConfig(const std::string &type, nlohmann::json *config)
{
  if (type == "camera")
  {
    editorData.cameraConfig = config;
    connectCameraConfigGroupCallbacks();
  }
  else if (type == "background")
  {
    editorData.backgroundConfig = config;
    connectBackgroundConfigGroupCallbacks();
  }
  else if (type == "object")
  {
    editorData.objectConfig = config;
    connectObjectConfigGroupCallbacks();
  }
}

void GuiManager::setEditorInfo(const std::string &info)
{
  if (currentUI != UI::EDITOR)
    return;
  gui->get<tgui::Label>("label_info")->setText(info);
  auto resetTimerCallback = [this]
  {
    if (currentUI == UI::EDITOR)
      gui->get<tgui::Label>("label_info")->setText("");
  };
  editorData.infoResetTimer = Timer::create(3, resetTimerCallback, false);
}

void GuiManager::toggleEditorInstructionsVisibility()
{
  if (currentUI != UI::EDITOR)
    return;

  for (const auto &widget : gui->getWidgets())
  {
    if (widget->getWidgetName().find("label_instructions") != tgui::String::npos)
    {
      widget->setVisible(!widget->isVisible());
    }
  }
}

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
  groups.clear();
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
      gui->loadWidgetsFromFile(editor_ui_path);
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
  auto exitCallback = []
  {
    Game::running = false;
  };
  gui->get<tgui::Button>("button_exit")->onPress(exitCallback);

  if (levelNames.empty())
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
  groups.emplace("object", tgui::Group::create());
  gui->add(groups.at("object"));
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

void GuiManager::connectBackgroundConfigGroupCallbacks()
{
  // Background clouds
  gui->get<tgui::EditBox>("editbox_bg_clouds")->setText(std::to_string(static_cast<int>(this->editorData.backgroundConfig->at("clouds"))));
  auto cloudsCallback = [this]
  {
    if (!this->editorData.backgroundConfig)
      return;
    this->editorData.backgroundConfig->at("clouds") = gui->get<tgui::EditBox>("editbox_bg_clouds")->getText().toInt();
    dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateBackground();
  };
  gui->get<tgui::EditBox>("editbox_bg_clouds")->onTextChange(cloudsCallback);

  // Background islands
  gui->get<tgui::EditBox>("editbox_bg_islands")->setText(std::to_string(static_cast<int>(this->editorData.backgroundConfig->at("islands"))));
  auto islandsCallback = [this]
  {
    if (!this->editorData.backgroundConfig)
      return;
    this->editorData.backgroundConfig->at("islands") = gui->get<tgui::EditBox>("editbox_bg_islands")->getText().toInt();
    dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateBackground();
  };
  gui->get<tgui::EditBox>("editbox_bg_islands")->onTextChange(islandsCallback);

  // Background distance
  gui->get<tgui::EditBox>("editbox_bg_distance")->setText(std::to_string(static_cast<int>(this->editorData.backgroundConfig->at("additional_distance"))));
  auto distanceCallback = [this]
  {
    if (!this->editorData.backgroundConfig)
      return;
    this->editorData.backgroundConfig->at("additional_distance") = gui->get<tgui::EditBox>("editbox_bg_distance")->getText().toInt();
    dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateBackground();
  };
  gui->get<tgui::EditBox>("editbox_bg_distance")->onTextChange(distanceCallback);
}

void GuiManager::connectCameraConfigGroupCallbacks()
{
  // Camera follow
  gui->get<tgui::EditBox>("editbox_camera_follow")->setText(static_cast<std::string>(this->editorData.cameraConfig->at("follow")));
  auto followCallback = [this]
  {
    if (!this->editorData.cameraConfig)
      return;
    this->editorData.cameraConfig->at("follow") = gui->get<tgui::EditBox>("editbox_camera_follow")->getText();
    dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateCamera();
  };
  gui->get<tgui::EditBox>("editbox_camera_follow")->onTextChange(followCallback);

  if (!editorData.cameraConfig->contains("pos"))
    (*editorData.cameraConfig)["pos"] = {0.0f, 0.0f};

  // Camera Position X
  gui->get<tgui::EditBox>("editbox_camera_pos_x")->setText(std::to_string(static_cast<int>(this->editorData.cameraConfig->at("pos").at(0))));
  auto posXCallback = [this]
  {
    if (!this->editorData.cameraConfig)
      return;
    this->editorData.cameraConfig->at("pos").at(0) = gui->get<tgui::EditBox>("editbox_camera_pos_x")->getText().toInt();
    dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateCamera();
  };
  gui->get<tgui::EditBox>("editbox_camera_pos_x")->onTextChange(posXCallback);

  // Camera Position Y
  gui->get<tgui::EditBox>("editbox_camera_pos_y")->setText(std::to_string(static_cast<int>(this->editorData.cameraConfig->at("pos").at(1))));
  auto posYCallback = [this]
  {
    if (!this->editorData.cameraConfig)
      return;
    this->editorData.cameraConfig->at("pos").at(1) = gui->get<tgui::EditBox>("editbox_camera_pos_y")->getText().toInt();
    dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateCamera();
  };
  gui->get<tgui::EditBox>("editbox_camera_pos_y")->onTextChange(posYCallback);

  // Camera Size X
  gui->get<tgui::EditBox>("editbox_camera_size_x")->setText(std::to_string(static_cast<int>(this->editorData.cameraConfig->at("size").at(0))));
  auto sizeXCallback = [this]
  {
    if (!this->editorData.cameraConfig)
      return;
    this->editorData.cameraConfig->at("size").at(0) = gui->get<tgui::EditBox>("editbox_camera_size_x")->getText().toInt();
    dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateCamera();
  };
  gui->get<tgui::EditBox>("editbox_camera_size_x")->onTextChange(sizeXCallback);

  // Camera Size Y
  gui->get<tgui::EditBox>("editbox_camera_size_y")->setText(std::to_string(static_cast<int>(this->editorData.cameraConfig->at("size").at(1))));
  auto sizeYCallback = [this]
  {
    if (!this->editorData.cameraConfig)
      return;
    this->editorData.cameraConfig->at("size").at(1) = gui->get<tgui::EditBox>("editbox_camera_size_y")->getText().toInt();
    dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateCamera();
  };
  gui->get<tgui::EditBox>("editbox_camera_size_y")->onTextChange(sizeYCallback);
}

void GuiManager::connectObjectConfigGroupCallbacks()
{
  if (groups.find("object") == groups.end())
    return;
  groups.at("object")->removeAllWidgets();
  // Если nullptr - удаляем и всё
  if (!editorData.objectConfig)
    return;

  if (!editorData.objectConfig->contains("scale") && !editorData.objectConfig->contains("verts"))
    (*editorData.objectConfig)["scale"] = {1.0f, 1.0f};
  if (!editorData.objectConfig->contains("rotation"))
    (*editorData.objectConfig)["rotation"] = 0.0f;

  // Считаем количество полей
  auto predicate = [](const auto &json)
  {
    const std::string &key = json.key();
    if (editorConfigValues.contains(key))
    {
      const auto &typeField = editorConfigValues[key]["type"];
      return typeField != "hide";
    }
    return false;
  };
  size_t fields = std::count_if(editorData.objectConfig->items().begin(), editorData.objectConfig->items().end(), predicate);
  if (fields == 0)
    return;

  // Создаём задний фон
  auto bg = tgui::Label::copy(gui->get<tgui::Label>("label_bg_temp"));
  std::string layoutStr = std::to_string(4 + 4 * fields) + "%";
  bg->setSize(bg->getSize().x, layoutStr.c_str());
  layoutStr = std::to_string(100 - (3 + 4 + 4 * fields)) + "%";
  bg->setPosition(bg->getPosition().x, layoutStr.c_str());
  bg->setVisible(true);
  groups.at("object")->add(bg);

  // Добавляем поля
  size_t i = 0;
  for (const auto &it : editorData.objectConfig->items())
  {
    const auto &typeField = editorConfigValues[it.key()]["type"];
    if (typeField == "hide")
      continue;
    // 3% from bottom of the screen
    // 2% from top and bottom of background (so 4%)
    // 4% - label height
    // 3.5% editbox height (so +0.25% from label pos)
    std::string labelPosString = std::to_string(100 - (3 + 4 + 4 * fields) + 2 + 4 * i) + "%";
    std::string editboxPosString = std::to_string(100 - (3 + 4 + 4 * fields) + 2 + 4 * i) + ".25%";

    // Создаём Label
    auto label = tgui::Label::copy(gui->get<tgui::Label>("label_temp"));
    label->setWidgetName("label_object_" + it.key());
    label->setText(it.key());
    label->setPosition(label->getPosition().x, labelPosString.c_str());
    label->setVisible(true);
    groups.at("object")->add(label);
    // std::cout << it.key() << " " << labelPosString << " " << editboxPosString << " " << typeField << "\n";

    // Создаём editbox
    if (typeField == "vector")
    {
      const std::string& key = it.key();
      auto editboxX = tgui::EditBox::copy(gui->get<tgui::EditBox>("editbox_temp_vector_x"));
      auto editboxY = tgui::EditBox::copy(gui->get<tgui::EditBox>("editbox_temp_vector_y"));
      editboxX->setWidgetName("editbox_object_" + it.key() + "_x");
      editboxY->setWidgetName("editbox_object_" + it.key() + "_y");
      editboxX->setPosition(editboxX->getPosition().x, editboxPosString.c_str());
      editboxY->setPosition(editboxY->getPosition().x, editboxPosString.c_str());
      editboxX->setVisible(true);
      editboxY->setVisible(true);
      editboxX->setText(to_string3(static_cast<float>(editorData.objectConfig->at(key).at(0))));
      editboxY->setText(to_string3(static_cast<float>(editorData.objectConfig->at(key).at(1))));
      auto changeXCallback = [this, key, editboxX]
      {
        editorData.objectConfig->at(key).at(0) = editboxX->getText().toFloat();
        dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateSelectedObject();
        // std::cout << key << " changed to " << editorData.objectConfig->at(key).at(0) << "\n";
      };
      auto changeYCallback = [this, key, editboxY]
      {
        editorData.objectConfig->at(key).at(1) = editboxY->getText().toFloat();
        dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateSelectedObject();
      };
      editboxX->onTextChange(changeXCallback);
      editboxY->onTextChange(changeYCallback);
      groups.at("object")->add(editboxX);
      groups.at("object")->add(editboxY);
    }
    else if (typeField == "bool")
    {
      const std::string& key = it.key();
      auto checkbox = tgui::CheckBox::copy(gui->get<tgui::CheckBox>("checkbox_temp"));
      checkbox->setWidgetName("checkbox_object_" + it.key());
      checkbox->setPosition(checkbox->getPosition().x, editboxPosString.c_str());
      checkbox->setVisible(true);
      checkbox->setChecked(static_cast<bool>(editorData.objectConfig->at(key)));
      auto changeCallback = [this, key, checkbox, typeField]
      {
        editorData.objectConfig->at(key) = checkbox->isChecked();
        dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateSelectedObject();
      };
      checkbox->onChange(changeCallback);
      groups.at("object")->add(checkbox);
    }
    else
    {
      const std::string& key = it.key();
      std::string typeFieldStr = static_cast<std::string>(typeField);
      tgui::EditBox::Ptr editbox = tgui::EditBox::copy(gui->get<tgui::EditBox>("editbox_temp_" + typeFieldStr));
      editbox->setWidgetName("editbox_object_" + it.key());
      editbox->setPosition(editbox->getPosition().x, editboxPosString.c_str());
      editbox->setVisible(true);
      if (typeField == "float" || typeField == "ufloat")
        editbox->setText(to_string3(static_cast<float>(editorData.objectConfig->at(key))));
      else if (typeField == "int" || typeField == "uint")
        editbox->setText(std::to_string(static_cast<int>(editorData.objectConfig->at(key))));
      else if (typeField == "string")
        editbox->setText(static_cast<std::string>(editorData.objectConfig->at(key)));
      else if (typeField == "array" && editorData.objectConfig->at(key).is_array())
      {
        std::string text = joinVector(editorData.objectConfig->at(key).get<std::vector<std::string>>(), ";");
        editbox->setText(text);
      }
      auto changeCallback = [this, key, editbox, typeField]
      {
        if (typeField == "float" || typeField == "ufloat")
          editorData.objectConfig->at(key) = editbox->getText().toFloat();
        else if (typeField == "int" || typeField == "uint")
          editorData.objectConfig->at(key) = editbox->getText().toInt();
        else if (typeField == "string")
          editorData.objectConfig->at(key) = editbox->getText().toStdString();
        else if (typeField == "array")
        {
          std::vector<std::string> values = splitString(editbox->getText().toStdString(), ';');
          editorData.objectConfig->at(key) = values;
        }
        dynamic_cast<Editor *>(Game::getObjectManager().get())->recreateSelectedObject();
      };
      editbox->onTextChange(changeCallback);
      groups.at("object")->add(editbox);
    }

    ++i;
  }
}
