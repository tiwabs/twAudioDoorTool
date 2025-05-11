#pragma once

#include "imgui.h"
#include "../settings_manager.h"
#include "settingsWindow.h"
#include "doorWindow.h"
#include "../doors.h"
#include <vector>
#include <string>

class MainWindow {
public:
    MainWindow();
    void render();

private:
    SettingsWindow settingsWindow;
    DoorWindow doorWindow;
    std::vector<Door> doors;
    void handleDoorAdded(const Door& door);
    void handleDoorEdited(const Door& door, size_t index);
    void deleteDoor(size_t index);
    bool checkDoorExists(const char* name, int currentIndex);
    void importXmlFile(const std::string& filePath);
}; 