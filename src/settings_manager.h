#pragma once

#include <string>
#include <nlohmann/json.hpp>

class SettingsManager {
public:
    static SettingsManager& getInstance();
    
    bool loadSettings();
    bool saveSettings();
    int getSelectedThemeIndex() const { return selectedThemeIndex; }
    void setSelectedThemeIndex(int index);
    
private:
    SettingsManager() = default;
    std::string settingsPath = "assets/settings.json";
    int selectedThemeIndex = 1; // Default to dark theme
}; 