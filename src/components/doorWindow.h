#pragma once

#include "imgui.h"
#include "../settings_manager.h"
#include "../doors.h"
#include <functional>
#include <string>
#include <vector>

class DoorWindow {
public:
    DoorWindow();
    void render();
    void open() { isOpen = true; }
    void openForEdit(const Door& door, size_t index);
    void setOnDoorAdded(std::function<void(const Door&)> callback) { onDoorAdded = callback; }
    void setOnDoorEdited(std::function<void(const Door&, size_t)> callback) { onDoorEdited = callback; }
    void setOnCheckDoorExists(std::function<bool(const char*, int)> callback) { onCheckDoorExists = callback; }
    bool isModalOpen() const { return isOpen; }
    std::function<std::vector<Door>()> onGetDoors;
    void generateXmlFile(const std::string& filePath);

private:
    void resetForm();
    bool isOpen = false;
    bool isEditing = false;
    size_t editingIndex = 0;
    std::function<void(const Door&)> onDoorAdded;
    std::function<void(const Door&, size_t)> onDoorEdited;
    std::function<bool(const char*, int)> onCheckDoorExists;
    
    // Form variables
    char doorName[256];
    char sounds[1024];
    char tuningParams[1024];
    float maxOcclusion;
    size_t selectedPreset;

    std::string calculateJoaatHash(const char* str);
}; 