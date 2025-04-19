#include "themes.h"
#include "raygui.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <iostream>

// Include all theme style definitions from raygui
#include "../build/_deps/raygui-src/styles/ashes/ashes.h"
#include "../build/_deps/raygui-src/styles/bluish/bluish.h"
#include "../build/_deps/raygui-src/styles/candy/candy.h"
#include "../build/_deps/raygui-src/styles/cherry/cherry.h"
#include "../build/_deps/raygui-src/styles/cyber/cyber.h"
#include "../build/_deps/raygui-src/styles/dark/dark.h"
#include "../build/_deps/raygui-src/styles/enefete/enefete.h"
#include "../build/_deps/raygui-src/styles/jungle/jungle.h"
#include "../build/_deps/raygui-src/styles/lavanda/lavanda.h"
#include "../build/_deps/raygui-src/styles/sunny/sunny.h"
#include "../build/_deps/raygui-src/styles/terminal/terminal.h"

// Initialize the current theme to dark theme by default
int Theme::currentThemeIndex = Theme::THEME_DARK;

/**
 * Load and apply a theme by its index
 * @param themeIndex Index of the theme to load
 */
void Theme::LoadTheme(int themeIndex) {
    currentThemeIndex = themeIndex;

    // Apply the selected theme using raygui's theme loading functions
    switch (themeIndex) {
        case THEME_DEFAULT:
            GuiLoadStyleDefault();
            break;
        case THEME_DARK:
            GuiLoadStyleDark();
            break;
        case THEME_BLUISH:
            GuiLoadStyleBluish();
            break;
        case THEME_CANDY:
            GuiLoadStyleCandy();
            break;
        case THEME_CHERRY:
            GuiLoadStyleCherry();
            break;
        case THEME_CYBER:
            GuiLoadStyleCyber();
            break;
        case THEME_JUNGLE:
            GuiLoadStyleJungle();
            break;
        case THEME_LAVANDA:
            GuiLoadStyleLavanda();
            break;
        case THEME_TERMINAL:
            GuiLoadStyleTerminal();
            break;
        case THEME_SUNNY:
            GuiLoadStyleSunny();
            break;
        case THEME_ASHES:
            GuiLoadStyleAshes();
            break;
        case THEME_ENEFETE:
            GuiLoadStyleEnefete();
            break;
        default:
            std::cout << "Theme index out of range" << std::endl;
            GuiLoadStyleDefault();
            break;
    }
}

/**
 * Get the name of a theme by its index
 * @param themeIndex Index of the theme
 * @return Name of the theme as a string, or "Unknown" if index is invalid
 */
const char* Theme::GetThemeName(int themeIndex) {
    switch (themeIndex) {
        case THEME_DEFAULT: return "Default";
        case THEME_DARK: return "Dark";
        case THEME_BLUISH: return "Bluish";
        case THEME_CANDY: return "Candy";
        case THEME_CHERRY: return "Cherry";
        case THEME_CYBER: return "Cyber";
        case THEME_JUNGLE: return "Jungle";
        case THEME_LAVANDA: return "Lavanda";
        case THEME_TERMINAL: return "Terminal";
        case THEME_SUNNY: return "Sunny";
        case THEME_ASHES: return "Ashes";
        case THEME_ENEFETE: return "Enefete";
        default: return "Unknown";
    }
}