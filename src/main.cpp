#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "themes.h"
#include "settings_manager.h"
#include "raylib.h"
#include "raygui.h"

// Function to calculate the centered position for a popup
Rectangle getCenteredPopupRect(float width, float height) {
    const float screenWidth = static_cast<float>(GetScreenWidth());
    const float screenHeight = static_cast<float>(GetScreenHeight());
    const float x = (screenWidth - width) / 2.0f;
    const float y = (screenHeight - height) / 2.0f;
    return (Rectangle){ x, y, width, height };
}

bool settingsPopup() {
    GuiUnlock();  // Débloquer pour le popup
    
    // Calculate center position for the popup
    const float popupWidth = 300.0f;
    const float popupHeight = 400.0f;
    Rectangle popupRect = getCenteredPopupRect(popupWidth, popupHeight);
    
    if (GuiWindowBox(popupRect, "Settings")) {
        return true;
    }

    // Style selector with dropdown
    GuiLabel((Rectangle){ popupRect.x + 20.0f, popupRect.y + 30.0f, 100.0f, 20.0f }, "Theme:");
    
    // Create a dropdown for theme selection
    static int dropdownActive = SettingsManager::getInstance().getSelectedThemeIndex();
    static bool dropdownEditMode = false;
    Rectangle dropdownRect = { popupRect.x + 20.0f, popupRect.y + 60.0f, 260.0f, 30.0f };
    
    // Convert styles array to a single string with items separated by semicolons
    static char dropdownText[1024] = "";
    if (dropdownText[0] == '\0') {
        strcpy(dropdownText, Theme::GetThemeName(0));
        for (int i = 1; i < 12; i++) {
            strcat(dropdownText, ";");
            strcat(dropdownText, Theme::GetThemeName(i));
        }
    }
    
    if (GuiDropdownBox(dropdownRect, dropdownText, &dropdownActive, dropdownEditMode)) {
        dropdownEditMode = !dropdownEditMode;
    }
    
    // Apply the selected theme if it has changed
    if (SettingsManager::getInstance().getSelectedThemeIndex() != dropdownActive) {
        SettingsManager::getInstance().setSelectedThemeIndex(dropdownActive);
        Theme::LoadTheme(dropdownActive);
    }

    return false;
};

// TODO: Make this popup work with add / edit doors, for now it works with add doors only
bool doorPopup() {
    GuiUnlock();  // Débloquer pour le popup

    static char doorName[256] = "";
    static char sounds[256] = "";
    static char tuningParams[256] = "";
    static float maxOcclusion = 0.7f;
    static int selectedPreset = 0;
    static bool doorNameEditMode = false;
    
    // Calculate center position for the popup
    const float popupWidth = 400.0f;
    const float popupHeight = 200.0f;
    Rectangle popupRect = getCenteredPopupRect(popupWidth, popupHeight);
    
    // TODO: change the title of the popup depending on the mode (add or edit)
    if (GuiWindowBox(popupRect, "Add New Door")) {
        return true;
    }

    // Door properties
    GuiLabel((Rectangle){ popupRect.x + 20.0f, popupRect.y + 30.0f, 100.0f, 20.0f }, "Door Name:");
    if (GuiTextBox((Rectangle){ popupRect.x + 20.0f, popupRect.y + 55.0f, 360.0f, 25.0f }, doorName, 256, doorNameEditMode)) {
        doorNameEditMode = !doorNameEditMode;
    }

    // Sound preset selection
    GuiLabel((Rectangle){ popupRect.x + 20.0f, popupRect.y + 90.0f, 100.0f, 20.0f }, "Sound Preset:");
    
    // Create dropdown text for sound presets
    static char presetDropdownText[4096] = "";
    if (presetDropdownText[0] == '\0') {
        const auto& presets = SettingsManager::getInstance().getSoundPresets();
        if (!presets.empty()) {
            strcpy(presetDropdownText, presets[0].name.c_str());
            for (size_t i = 1; i < presets.size(); i++) {
                strcat(presetDropdownText, ";");
                strcat(presetDropdownText, presets[i].name.c_str());
            }
        }
    }
    
    static bool presetDropdownEditMode = false;
    Rectangle presetDropdownRect = { popupRect.x + 20.0f, popupRect.y + 115.0f, 360.0f, 30.0f };
    if (GuiDropdownBox(presetDropdownRect, presetDropdownText, &selectedPreset, presetDropdownEditMode)) {
        presetDropdownEditMode = !presetDropdownEditMode;
        if (!presetDropdownEditMode) {
            const auto& presets = SettingsManager::getInstance().getSoundPresets();
            if (selectedPreset < presets.size()) {
                const auto& preset = presets[selectedPreset];
                strcpy(sounds, preset.sounds.c_str());
                strcpy(tuningParams, preset.tuningParams.c_str());
                maxOcclusion = preset.maxOcclusion;
            }
        }
    }

    if (GuiButton((Rectangle){ popupRect.x + 20.0f, popupRect.y + 160.0f, 100.0f, 20.0f }, "Cancel")) {
        doorName[0] = '\0';
        sounds[0] = '\0';
        tuningParams[0] = '\0';
        maxOcclusion = 0.7f;
        selectedPreset = 0;

        return true;
    }

    // TODO: change the button depending on the mode (add or edit)
    // Add button
    if (GuiButton((Rectangle){ popupRect.x + 280.0f, popupRect.y + 160.0f, 100.0f, 20.0f }, "Add Door")) {
        // TODO: Add door logic

        // TODO: clean the values
        return true;
    }

    // if (GuiButton((Rectangle){ popupRect.x + 280.0f, popupRect.y + 160.0f, 100.0f, 30.0f }, "Edit Door")) {
    //     // TODO: Edit door logic

    //     // TODO: clean the values
    //     return true;
    // }

    return false;
};

int main() {
    const int screenWidth = 500;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "GTA V Audio Door Tool");
    SetTargetFPS(60);

    // Load settings and initial theme
    SettingsManager::getInstance().loadSettings();
    int initialTheme = SettingsManager::getInstance().getSelectedThemeIndex();
    std::cout << "Loading initial style: " << Theme::GetThemeName(initialTheme) << std::endl;
    Theme::LoadTheme(initialTheme);

    bool showSettingsPopup = false;
    bool showDoorPopup = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        // Use the theme's background color instead of a fixed gray color
        Color bgColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
        ClearBackground(bgColor);

        // Lock the interface if a popup is shown
        if (showSettingsPopup || showDoorPopup) {
            GuiLock();
        }
        
        // Top buttons
        if (GuiButton((Rectangle){ 20.0f, 20.0f, 100.0f, 20.0f }, "Import file")) {
            std::cout << "Import file" << std::endl;
            //TODO: Make a file browser to select the input file
        }

        if (GuiButton((Rectangle){ 380.0f, 20.0f, 100.0f, 20.0f }, "Settings")) {
            std::cout << "Settings" << std::endl;
            if (!showSettingsPopup && !showDoorPopup)
            {
                showSettingsPopup = true;
            }
        }

        // Middle
        GuiGroupBox((Rectangle){ 20.0f, 60.0f, 460.0f, 480.0f }, "Doors");
        
        //TODO: Make a scroll area for the doors

        // Bottom buttons
        if (GuiButton((Rectangle){ 20.0f, 560.0f, 100.0f, 20.0f }, "Generate file")) {
            std::cout << "Generate file" << std::endl;
            //TODO: Make a file browser to select the output location and file name
        }

        if (GuiButton((Rectangle){ 380.0f, 560.0f, 100.0f, 20.0f }, "Add new door")) {
            std::cout << "Add new door" << std::endl;
            if (!showDoorPopup && !showSettingsPopup)
            {
                showDoorPopup = true;
            }
        }
        
        GuiLabel((Rectangle){ 200.0f, 580.0f, 200.0f, 20.0f }, "made by tiwabs");

        // Popups should be drawn last
        if (showSettingsPopup || showDoorPopup) {
            // Draw semi-transparent overlay using the theme's background color to make it look like disabled
            Color overlayColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
            overlayColor.a = 128;
            DrawRectangle(0, 0, screenWidth, screenHeight, overlayColor);
        }

        // TODO: Move this with the other popup stuff
        if (showSettingsPopup) {
            if (settingsPopup()) {
                showSettingsPopup = false;
                GuiUnlock();
            }
        }

        if (showDoorPopup) {
            if (doorPopup()) {
                showDoorPopup = false;
                GuiUnlock();
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}