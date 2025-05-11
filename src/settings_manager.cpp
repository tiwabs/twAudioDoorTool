#include "settings_manager.h"
#include <fstream>
#include <iostream>
#include <filesystem>

#ifdef __APPLE__
#include <mach-o/dyld.h> // For _NSGetExecutablePath on macOS
#elif defined(_WIN32)
#include <windows.h>
#endif

/**
 * Get the absolute path to the executable
 * @return Path to the executable
 */
std::string getExecutablePath() {
#ifdef __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        return std::string(path);
    }
#elif defined(_WIN32)
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) != 0) {
        return std::string(path);
    }
#endif
    return "";
}

/**
 * Get the directory containing the executable
 * @return Path to the directory containing the executable
 */
std::string getExecutableDirectory() {
    std::string exePath = getExecutablePath();
    return std::filesystem::path(exePath).parent_path().string();
}

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
        // Obtient le répertoire de l'exécutable
        std::string exeDir = getExecutableDirectory();
        std::string settingsFilePath = exeDir + "/assets/settings.json";
        
        file.open(settingsFilePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open settings file at: " << settingsFilePath << std::endl;
            std::cerr << "Current working directory: " << std::filesystem::current_path().string() << std::endl;
            std::cerr << "Executable directory: " << exeDir << std::endl;
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
            p["Sounds"] = preset.sounds;
            p["TuningParams"] = preset.tuningParams;
            p["MaxOcclusion"] = preset.maxOcclusion;
            presetsArray.push_back(p);
        }
        j["availableDoorSound"] = presetsArray;

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