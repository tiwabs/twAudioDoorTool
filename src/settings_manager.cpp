#include "settings_manager.h"
#include <fstream>
#include <iostream>

/**
 * Get the singleton instance of SettingsManager
 * Creates the instance if it doesn't exist
 */
SettingsManager& SettingsManager::getInstance() {
    static SettingsManager instance;
    instance.loadSettings();  // Load settings when instance is created
    return instance;
}

/**
 * Load settings from the JSON file
 * If the file doesn't exist or there's an error, use default settings
 */
bool SettingsManager::loadSettings() {
    try {
        // Open settings file
        std::ifstream file(settingsPath);
        if (!file.is_open()) {
            std::cout << "No settings file found, using defaults" << std::endl;
            return false;
        }

        // Parse JSON data
        nlohmann::json j;
        file >> j;

        // Load sound presets if present
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

/**
 * Save current settings to JSON file
 * Includes all sound presets
 */
bool SettingsManager::saveSettings() {
    try {
        nlohmann::json j;

        // Convert sound presets to JSON array
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

        // Write to file with pretty formatting
        std::ofstream file(settingsPath);
        file << j.dump(4);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving settings: " << e.what() << std::endl;
        return false;
    }
}

/**
 * Add or update a sound preset
 * If a preset with the same name exists, it will be replaced
 */
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

/**
 * Remove a sound preset by name
 * If the preset doesn't exist, no action is taken
 */
void SettingsManager::removeSoundPreset(const std::string& name) {
    auto it = std::find_if(soundPresets.begin(), soundPresets.end(),
        [&](const SoundPreset& p) { return p.name == name; });
    
    if (it != soundPresets.end()) {
        soundPresets.erase(it);
        saveSettings();
    }
} 