#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "themes.h"
#include "settings_manager.h"
#include "raylib.h"
#include "raygui.h"

/**
 * Calculates the centered position for a popup window
 * @param width - Width of the popup
 * @param height - Height of the popup
 * @return Rectangle with centered coordinates
 */
Rectangle getCenteredPopupRect(float width, float height) {
    const float screenWidth = static_cast<float>(GetScreenWidth());
    const float screenHeight = static_cast<float>(GetScreenHeight());
    const float x = (screenWidth - width) / 2.0f;
    const float y = (screenHeight - height) / 2.0f;
    return Rectangle{ x, y, width, height };
}

/**
 * Handles the settings popup window functionality
 * Allows users to change application theme
 * @return true if popup should close, false otherwise
 */
bool settingsPopup() {
    GuiUnlock();  // Unlock GUI for popup interaction
    
    // Calculate center position for the popup
    const float popupWidth = 300.0f;
    const float popupHeight = 400.0f;
    Rectangle popupRect = getCenteredPopupRect(popupWidth, popupHeight);
    
    // Create window box with close button
    if (GuiWindowBox(popupRect, "Settings")) {
        return true;
    }

    // Style selector with dropdown
    GuiLabel(Rectangle{ popupRect.x + 20.0f, popupRect.y + 30.0f, 100.0f, 20.0f }, "Theme:");
    
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
    
    // Handle dropdown interaction
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

/**
 * Handles the door popup window functionality
 * Currently only supports adding new doors (TODO: implement edit functionality)
 * @return true if popup should close, false otherwise
 */
bool doorPopup() {
    GuiUnlock();  // Unlock GUI for popup interaction

    // Static variables to store door properties
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
    
    // Create window box with close button
    if (GuiWindowBox(popupRect, "Add New Door")) {
        return true;
    }

    // Door name input field
    GuiLabel(Rectangle{ popupRect.x + 20.0f, popupRect.y + 30.0f, 100.0f, 20.0f }, "Door Name:");
    if (GuiTextBox(Rectangle{ popupRect.x + 20.0f, popupRect.y + 55.0f, 360.0f, 25.0f }, doorName, 256, doorNameEditMode)) {
        doorNameEditMode = !doorNameEditMode;
    }

    // Cancel button - resets all fields
    if (GuiButton(Rectangle{ popupRect.x + 20.0f, popupRect.y + 160.0f, 100.0f, 20.0f }, "Cancel")) {
        doorName[0] = '\0';
        sounds[0] = '\0';
        tuningParams[0] = '\0';
        maxOcclusion = 0.7f;
        selectedPreset = 0;
        return true;
    }

    // Add door button
    if (GuiButton(Rectangle{ popupRect.x + 280.0f, popupRect.y + 160.0f, 100.0f, 20.0f }, "Add Door")) {
        // TODO: Add door logic
        // TODO: clean the values
        return true;
    }

    // Sound preset selection
    GuiLabel(Rectangle{ popupRect.x + 20.0f, popupRect.y + 90.0f, 100.0f, 20.0f }, "Sound Preset:");
    
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
    
    // Handle preset selection
    Rectangle presetDropdownRect = { popupRect.x + 20.0f, popupRect.y + 115.0f, 360.0f, 30.0f };
    int newSelectedPreset = GuiComboBox(presetDropdownRect, presetDropdownText, selectedPreset);
    
    // Update fields when a new preset is selected
    if (newSelectedPreset != selectedPreset) {
        selectedPreset = newSelectedPreset;
        const auto& presets = SettingsManager::getInstance().getSoundPresets();
        if (selectedPreset < presets.size()) {
            const auto& preset = presets[selectedPreset];
            strcpy(sounds, preset.sounds.c_str());
            strcpy(tuningParams, preset.tuningParams.c_str());
            maxOcclusion = preset.maxOcclusion;
        }
    }

    return false;
};

/**
 * Main application entry point
 * Initializes the window and runs the main application loop
 */
int main() {
    // Initialize window with fixed dimensions
    const int screenWidth = 500;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "GTA V Audio Door Tool");
    SetTargetFPS(60);
    SetExitKey(0);

    // Load application settings and initial theme
    SettingsManager::getInstance().loadSettings();
    int initialTheme = SettingsManager::getInstance().getSelectedThemeIndex();
    std::cout << "Loading initial style: " << Theme::GetThemeName(initialTheme) << std::endl;
    Theme::LoadTheme(initialTheme);

    // Popup state flags
    bool showSettingsPopup = false;
    bool showDoorPopup = false;

    // Main application loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        
        // Set background color based on current theme
        Color bgColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
        ClearBackground(bgColor);

        // Lock GUI when popups are shown
        if (showSettingsPopup || showDoorPopup) {
            GuiLock();
        }
        
        // Top navigation buttons
        if (GuiButton(Rectangle{ 20.0f, 20.0f, 100.0f, 20.0f }, "Import file")) {
            std::cout << "Import file" << std::endl;
            //TODO: Make a file browser to select the input file
        }

        if (GuiButton(Rectangle{ 380.0f, 20.0f, 100.0f, 20.0f }, "Settings")) {
            std::cout << "Settings" << std::endl;
            if (!showSettingsPopup && !showDoorPopup)
            {
                showSettingsPopup = true;
            }
        }

        // Main content area
        GuiGroupBox(Rectangle{ 20.0f, 60.0f, 460.0f, 480.0f }, "Doors");
        
        //TODO: Make a scroll area for the doors

        // Bottom action buttons
        if (GuiButton(Rectangle{ 20.0f, 560.0f, 100.0f, 20.0f }, "Generate file")) {
            std::cout << "Generate file" << std::endl;
            //TODO: Make a file browser to select the output location and file name
        }

        if (GuiButton(Rectangle{ 380.0f, 560.0f, 100.0f, 20.0f }, "Add new door")) {
            std::cout << "Add new door" << std::endl;
            if (!showDoorPopup && !showSettingsPopup)
            {
                showDoorPopup = true;
            }
        }
        
        // Credits
        GuiLabel(Rectangle{ 200.0f, 580.0f, 200.0f, 20.0f }, "made by tiwabs");

        // Draw popup overlays
        if (showSettingsPopup || showDoorPopup) {
            // Create semi-transparent overlay
            Color overlayColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
            overlayColor.a = 128;
            DrawRectangle(0, 0, screenWidth, screenHeight, overlayColor);
        }

        // Handle popup windows
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

    // Cleanup
    CloseWindow();
    return 0;
}