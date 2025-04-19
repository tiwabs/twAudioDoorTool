#pragma once

/**
 * Class managing application themes
 * Provides functionality to load and switch between different GUI themes
 */
class Theme {
    public:
        // Theme index constants
        static const int THEME_DEFAULT = 0;   // Default theme
        static const int THEME_DARK = 1;      // Dark theme
        static const int THEME_BLUISH = 2;    // Bluish theme
        static const int THEME_CANDY = 3;     // Candy theme
        static const int THEME_CHERRY = 4;    // Cherry theme
        static const int THEME_CYBER = 5;     // Cyber theme
        static const int THEME_JUNGLE = 6;    // Jungle theme
        static const int THEME_LAVANDA = 7;   // Lavanda theme
        static const int THEME_TERMINAL = 8;  // Terminal theme
        static const int THEME_SUNNY = 9;     // Sunny theme
        static const int THEME_ASHES = 10;    // Ashes theme
        static const int THEME_ENEFETE = 11;  // Enefete theme

        /**
         * Load a theme by its index
         * @param themeIndex Index of the theme to load
         */
        static void LoadTheme(int themeIndex);

        /**
         * Get the currently active theme index
         * @return Index of the current theme
         */
        static int GetCurrentThemeIndex() { return currentThemeIndex; };

        /**
         * Get the name of a theme by its index
         * @param themeIndex Index of the theme
         * @return Name of the theme as a string
         */
        static const char* GetThemeName(int themeIndex);

    private:
        static int currentThemeIndex;  // Currently active theme index
};
