#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

/**
 * Structure representing a sound preset for doors
 * Contains all necessary parameters for door sound configuration
 */
struct SoundPreset {
    std::string name;           // Name of the preset
    std::string sounds;         // Sound file paths or identifiers
    std::string tuningParams;   // Sound tuning parameters
    float maxOcclusion;         // Maximum occlusion value for the sound
};

/**
 * Singleton class managing application settings
 * Handles loading/saving settings and managing sound presets
 */
class SettingsManager {
public:
    /**
     * Get the singleton instance of SettingsManager
     * @return Reference to the SettingsManager instance
     */
    static SettingsManager& getInstance();
    
    /**
     * Load settings from the settings file
     * @return true if settings were loaded successfully, false otherwise
     */
    bool loadSettings();
    
    /**
     * Save current settings to the settings file
     * @return true if settings were saved successfully, false otherwise
     */
    bool saveSettings();
    
    /**
     * Get the currently selected theme index
     * @return Index of the selected theme
     */
    int getSelectedThemeIndex() const { return selectedThemeIndex; }
    
    /**
     * Set the theme index and save the change
     * @param index New theme index to set
     */
    void setSelectedThemeIndex(int index);
    
    /**
     * Get all available sound presets
     * @return Vector of all sound presets
     */
    const std::vector<SoundPreset>& getSoundPresets() const { return soundPresets; }
    
    /**
     * Add or update a sound preset
     * @param preset The sound preset to add or update
     */
    void addSoundPreset(const SoundPreset& preset);
    
    /**
     * Remove a sound preset by name
     * @param name Name of the preset to remove
     */
    void removeSoundPreset(const std::string& name);
    
private:
    SettingsManager() = default;  // Private constructor for singleton
    std::string settingsPath = "assets/settings.json";  // Path to settings file
    int selectedThemeIndex = 1;   // Default to dark theme
    std::vector<SoundPreset> soundPresets;  // Collection of available sound presets
}; 