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
            std::cout << "No settings file found, using defaults" << std::endl;
            return false;
        }

        nlohmann::json j;
        file >> j;

        if (j.contains("theme")) {
            selectedThemeIndex = j["theme"];
        }

        if (j.contains("availableDoorSound")) {
            soundPresets.clear();
            for (const auto& preset : j["availableDoorSound"]) {
                SoundPreset p;
                p.name = preset["name"];
                p.sounds = preset["Sounds"];
                p.tuningParams = preset["TuningParams"];
                p.maxOcclusion = preset["MaxOcclusion"];
                soundPresets.push_back(p);
            }
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading settings: " << e.what() << std::endl;
        return false;
    }
}

bool SettingsManager::saveSettings() {
    try {
        nlohmann::json j;
        j["theme"] = selectedThemeIndex;

        nlohmann::json presetsArray = nlohmann::json::array();
        for (const auto& preset : soundPresets) {
            nlohmann::json p;
            p["name"] = preset.name;
            p["sounds"] = preset.sounds;
            p["tuningParams"] = preset.tuningParams;
            p["maxOcclusion"] = preset.maxOcclusion;
            presetsArray.push_back(p);
        }
        j["soundPresets"] = presetsArray;

        std::ofstream file(settingsPath);
        file << j.dump(4);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving settings: " << e.what() << std::endl;
        return false;
    }
}

void SettingsManager::setSelectedThemeIndex(int index) {
    selectedThemeIndex = index;
    saveSettings();
}

void SettingsManager::addSoundPreset(const SoundPreset& preset) {
    // Remove existing preset with same name if it exists
    auto it = std::find_if(soundPresets.begin(), soundPresets.end(),
        [&](const SoundPreset& p) { return p.name == preset.name; });
    
    if (it != soundPresets.end()) {
        soundPresets.erase(it);
    }
    
    soundPresets.push_back(preset);
    saveSettings();
}

void SettingsManager::removeSoundPreset(const std::string& name) {
    auto it = std::find_if(soundPresets.begin(), soundPresets.end(),
        [&](const SoundPreset& p) { return p.name == name; });
    
    if (it != soundPresets.end()) {
        soundPresets.erase(it);
        saveSettings();
    }
} 