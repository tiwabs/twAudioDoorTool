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

    // Default constructor
    SoundPreset() : maxOcclusion(0.7f) {}

    // Constructor with parameters
    SoundPreset(const std::string& name, const std::string& sounds, 
                const std::string& tuningParams, float maxOcclusion)
        : name(name), sounds(sounds), tuningParams(tuningParams), maxOcclusion(maxOcclusion) {}
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
     * Force reload settings from file
     * @return true if settings were loaded successfully, false otherwise
     */
    bool reloadSettings() { return loadSettings(); }
    
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
     * Update a sound preset at a specific index
     * @param index Index of the preset to update
     * @param preset New preset data
     */
    void updateSoundPreset(size_t index, const SoundPreset& preset);
    
    /**
     * Remove a sound preset by name
     * @param name Name of the preset to remove
     */
    void removeSoundPreset(const std::string& name);

    /**
     * Check if a sound preset with the given name exists
     * @param name Name to check
     * @return true if a preset with this name exists, false otherwise
     */
    bool hasSoundPreset(const std::string& name) const;
    
    /**
     * Set the path to the settings file
     * @param path The new path to the settings file
     */
    void setSettingsPath(const std::string& path) { settingsPath = path; }

private:
    SettingsManager() = default;  // Private constructor for singleton
    std::string settingsPath = "assets/settings.json";  // Path to settings file
    std::vector<SoundPreset> soundPresets;  // Collection of available sound presets
}; 