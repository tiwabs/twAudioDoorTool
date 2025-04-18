#include "settings_manager.h"
#include <fstream>
#include <iostream>

SettingsManager& SettingsManager::getInstance() {
    static SettingsManager instance;
    return instance;
}

bool SettingsManager::loadSettings() {
    try {
        std::ifstream file(settingsPath);
        if (!file.is_open()) {
            std::cout << "Could not open settings file: " << settingsPath << std::endl;
            return false;
        }

        nlohmann::json j;
        file >> j;
        
        selectedThemeIndex = j["selectedThemeIndex"].get<int>();
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error loading settings: " << e.what() << std::endl;
        return false;
    }
}

bool SettingsManager::saveSettings() {
    try {
        std::ofstream file(settingsPath);
        if (!file.is_open()) {
            std::cout << "Could not open settings file for writing: " << settingsPath << std::endl;
            return false;
        }

        nlohmann::json j;
        j["selectedThemeIndex"] = selectedThemeIndex;
        
        file << j.dump(2);
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error saving settings: " << e.what() << std::endl;
        return false;
    }
}

void SettingsManager::setSelectedThemeIndex(int index) {
    selectedThemeIndex = index;
    saveSettings();
} 