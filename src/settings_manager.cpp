#include "settings_manager.h"
#include <fstream>
#include <iostream>
#include <filesystem>

/**
 * Get the singleton instance of SettingsManager
 * Creates the instance if it doesn't exist
 */
SettingsManager& SettingsManager::getInstance() {
    static SettingsManager instance;
    static bool settingsLoaded = false;
    
    if (!settingsLoaded) {
        settingsLoaded = instance.loadSettings();
    }
    
    return instance;
}

/**
 * Try to open the settings file at the given path
 * @return true if the file was opened successfully, false otherwise
 */
bool tryOpenSettingsFile(const std::string& path, std::ifstream& file) {
    file.open(path);
    if (file.is_open()) {
        std::cout << "Successfully opened settings file at: " << path << std::endl;
        return true;
    }
    return false;
}

/**
 * Load settings from the JSON file
 * If the file doesn't exist or there's an error, use default settings
 */
bool SettingsManager::loadSettings() {
    try {
        std::ifstream file;
        std::vector<std::string> possiblePaths = {
            settingsPath,                                    // assets/settings.json
            "assets/settings.json",                          // assets/settings.json (explicit)
            "../assets/settings.json",                       // ../assets/settings.json
            std::string("../../") + settingsPath,           // ../../assets/settings.json
            std::string("../../../") + settingsPath         // ../../../assets/settings.json
        };

        bool fileOpened = false;
        for (const auto& path : possiblePaths) {
            if (tryOpenSettingsFile(path, file)) {
                fileOpened = true;
                settingsPath = path;  // Update the path to the working one
                break;
            }
        }

        if (!fileOpened) {
            std::cerr << "Error: Could not open settings file. Tried the following paths:" << std::endl;
            for (const auto& path : possiblePaths) {
                std::cerr << "  - " << path << std::endl;
            }
            std::cerr << "Current working directory: " << std::filesystem::current_path().string() << std::endl;
            return false;
        }

        // Parse JSON data
        nlohmann::json j;
        try {
            file >> j;
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "Error parsing settings file: " << e.what() << std::endl;
            return false;
        }

        // Load sound presets if present
        if (j.contains("availableDoorSound")) {
            soundPresets.clear();
            for (const auto& preset : j["availableDoorSound"]) {
                try {
                    SoundPreset p;
                    p.name = preset["name"];
                    p.sounds = preset["Sounds"];
                    p.tuningParams = preset["TuningParams"];
                    p.maxOcclusion = preset["MaxOcclusion"];
                    soundPresets.push_back(p);
                } catch (const std::exception& e) {
                    std::cerr << "Error loading preset: " << e.what() << std::endl;
                    continue;
                }
            }
            std::cout << "Successfully loaded " << soundPresets.size() << " sound presets" << std::endl;
        } else {
            std::cout << "No sound presets found in settings file" << std::endl;
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error loading settings: " << e.what() << std::endl;
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

/**
 * Update a sound preset at a specific index
 * If the index is invalid, no action is taken
 */
void SettingsManager::updateSoundPreset(size_t index, const SoundPreset& preset) {
    if (index < soundPresets.size()) {
        soundPresets[index] = preset;
        saveSettings();
    }
}

/**
 * Check if a sound preset with the given name exists
 */
bool SettingsManager::hasSoundPreset(const std::string& name) const {
    return std::find_if(soundPresets.begin(), soundPresets.end(),
        [&](const SoundPreset& p) { return p.name == name; }) != soundPresets.end();
} 