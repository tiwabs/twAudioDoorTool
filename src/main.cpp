#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "themes.h"
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
    
    if (GuiWindowBox(popupRect, "Settings"))
    {
        return true;
    }

    // Style selector with dropdown
    GuiLabel((Rectangle){ popupRect.x + 20.0f, popupRect.y + 30.0f, 100.0f, 20.0f }, "Theme:");
    
    // Create a dropdown for theme selection
    static int dropdownActive = Theme::GetCurrentThemeIndex();
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
    if (Theme::GetCurrentThemeIndex() != dropdownActive) {
        Theme::LoadTheme(dropdownActive);
    }

    return false;
};

bool doorPopup() {
    GuiUnlock();  // Débloquer pour le popup
    
    // Calculate center position for the popup
    const float popupWidth = 300.0f;
    const float popupHeight = 200.0f;
    Rectangle popupRect = getCenteredPopupRect(popupWidth, popupHeight);
    
    if (GuiWindowBox(popupRect, "Door"))
    {
        return true;
    }
    return false;
};

int main() {
    const int screenWidth = 500;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "GTA V Audio Door Tool");
    SetTargetFPS(60);

    // Load initial style
    std::cout << "Loading initial style: " << Theme::GetThemeName(Theme::GetCurrentThemeIndex()) << std::endl;
    Theme::LoadTheme(Theme::GetCurrentThemeIndex());

    bool showSettingsPopuop = false;
    bool showDoorPopup = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        // Use the theme's background color instead of a fixed gray color
        Color bgColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
        ClearBackground(bgColor);

        // Lock the interface if a popup is shown
        if (showSettingsPopuop || showDoorPopup) {
            GuiLock();
        }
        
        // Top buttons
        if (GuiButton((Rectangle){ 20.0f, 20.0f, 100.0f, 20.0f }, "Import file")) {
            std::cout << "Import file" << std::endl;
            //TODO: Make a file browser to select the input file
        }

        if (GuiButton((Rectangle){ 380.0f, 20.0f, 100.0f, 20.0f }, "Settings")) {
            std::cout << "Settings" << std::endl;
            if (!showSettingsPopuop && !showDoorPopup)
            {
                showSettingsPopuop = true;
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
            if (!showDoorPopup && !showSettingsPopuop)
            {
                showDoorPopup = true;
            }
        }
        
        GuiLabel((Rectangle){ 200.0f, 580.0f, 200.0f, 20.0f }, "made by tiwabs");

        // Popups should be drawn last
        if (showSettingsPopuop || showDoorPopup) {
            // Draw semi-transparent overlay using the theme's background color
            Color overlayColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
            overlayColor.a = 128; // Set alpha to 128 (semi-transparent)
            DrawRectangle(0, 0, screenWidth, screenHeight, overlayColor);
        }

        if (showSettingsPopuop) {
            if (settingsPopup()) {
                showSettingsPopuop = false;
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