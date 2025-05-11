#pragma once

#include "imgui.h"
#include "../settings_manager.h"

class SettingsWindow {
public:
    SettingsWindow();
    void render();
    void open() { isOpen = true; }

private:
    void renderPresetManager();
    bool isOpen = false;
}; 