#include <iostream>
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

bool settingsPopup() {
    GuiUnlock();  // Débloquer pour le popup
    if (GuiWindowBox((Rectangle){20, 20, 100, 20}, "Settings"))
    {
        return true;
    }
    return false;
};

bool doorPopup() {
    GuiUnlock();  // Débloquer pour le popup
    if (GuiWindowBox((Rectangle){20, 20, 100, 20}, "Door"))
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

    bool showSettingsPopuop = false;
    bool showDoorPopup = false;

    //TODO: Implement styles from raygui selectable from the settings window

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        // Lock the interface if a popup is shown
        if (showSettingsPopuop || showDoorPopup) {
            GuiLock();
        }
        
        // Top buttons
        if (GuiButton((Rectangle){ 20, 20, 100, 20 }, "Import file")) {
            std::cout << "Import file" << std::endl;
            //TODO: Make a file browser to select the input file
        }

        if (GuiButton((Rectangle){ 380, 20, 100, 20 }, "Settings")) {
            std::cout << "Settings" << std::endl;
            if (!showSettingsPopuop && !showDoorPopup)
            {
                showSettingsPopuop = true;
            }
        }

        // Middle
        GuiGroupBox((Rectangle){ 20, 60, 460, 480 }, "Doors");
        
        //TODO: Make a scroll area for the doors

        // Bottom buttons
        if (GuiButton((Rectangle){ 20, 560, 100, 20 }, "Generate file")) {
            std::cout << "Generate file" << std::endl;
            //TODO: Make a file browser to select the output location and file name
        }

        if (GuiButton((Rectangle){ 380, 560, 100, 20 }, "Add new door")) {
            std::cout << "Add new door" << std::endl;
            if (!showDoorPopup && !showSettingsPopuop)
            {
                showDoorPopup = true;
            }
        }
        
        GuiLabel((Rectangle){ 200, 580, 200, 20 }, "made by tiwabs");

        // Popups should be drawn last
        if (showSettingsPopuop || showDoorPopup) {
            // Draw semi-transparent overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 128 });
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