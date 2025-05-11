#pragma once

#include "imgui.h"
#include "../settings_manager.h"
#include <string>
#include <vector>

class SettingsWindow {
public:
    SettingsWindow();
    void render();
    void open() { isOpen = true; }

private:
    void renderPresetManager();
    void renderPresetEditForm();
    void resetForm();

    bool isOpen = false;
    bool isEditing = false;
    size_t editingIndex = 0;
    char presetName[256] = "";
    char sounds[256] = "";
    char tuningParams[256] = "";
    float maxOcclusion = 0.0f;
}; 