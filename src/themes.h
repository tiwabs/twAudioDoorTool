#pragma once

class Theme {
    public:
        static const int THEME_DEFAULT = 0;
        static const int THEME_DARK = 1;
        static const int THEME_BLUISH = 2;
        static const int THEME_CANDY = 3;
        static const int THEME_CHERRY = 4;
        static const int THEME_CYBER = 5;
        static const int THEME_JUNGLE = 6;
        static const int THEME_LAVANDA = 7;
        static const int THEME_TERMINAL = 8;
        static const int THEME_SUNNY = 9;
        static const int THEME_ASHES = 10;
        static const int THEME_ENEFETE = 11;

        static void LoadTheme(int themeIndex);
        static int GetCurrentThemeIndex() { return currentThemeIndex; };
        static const char* GetThemeName(int themeIndex);

    private:
        static int currentThemeIndex;
};
