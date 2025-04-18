#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct SoundPreset {
    std::string name;
    std::string sounds;
    std::string tuningParams;
    float maxOcclusion;
};

class SettingsManager {
public:
    static SettingsManager& getInstance();
    
    bool loadSettings();
    bool saveSettings();
    int getSelectedThemeIndex() const { return selectedThemeIndex; }
    void setSelectedThemeIndex(int index);
    
    // Sound preset management
    const std::vector<SoundPreset>& getSoundPresets() const { return soundPresets; }
    void addSoundPreset(const SoundPreset& preset);
    void removeSoundPreset(const std::string& name);
    
private:
    SettingsManager() = default;
    std::string settingsPath = "assets/settings.json";
    int selectedThemeIndex = 1; // Default to dark theme
    std::vector<SoundPreset> soundPresets;
}; 