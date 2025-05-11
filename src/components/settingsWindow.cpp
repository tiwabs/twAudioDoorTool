#include "settingsWindow.h"
#include <cstring>

SettingsWindow::SettingsWindow() : isOpen(false), isEditing(false), editingIndex(0) {
    presetName[0] = '\0';
    sounds[0] = '\0';
    tuningParams[0] = '\0';
    maxOcclusion = 0.7f;
}

void SettingsWindow::render() {
    if (ImGui::BeginPopupModal("Settings", &isOpen, 
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        
        if (!isEditing) {
            renderPresetManager();
        } else {
            renderPresetEditForm();
        }

        ImGui::Spacing();
        ImGui::EndPopup();
    } else if (!isOpen) {
        // If modal is closed (via the X button), reset edit mode
        resetForm();
    }
}

void SettingsWindow::renderPresetManager() {
    ImGui::Text("Door Presets:");
    const auto& presets = SettingsManager::getInstance().getSoundPresets();
    static int selectedPreset = 0;
    
    // Adjust selectedPreset if needed after deletion
    if (!presets.empty() && selectedPreset >= presets.size()) {
        selectedPreset = presets.size() - 1;
    }
    
    if (ImGui::BeginCombo("##P", presets.empty() ? "No Presets Found" : presets[selectedPreset].name.c_str())) {
        for (size_t i = 0; i < presets.size(); i++) {
            bool is_selected = (selectedPreset == i);
            if (ImGui::Selectable(presets[i].name.c_str(), is_selected)) {
                selectedPreset = i;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Spacing();
    if (ImGui::Button("Add", ImVec2(80, 20))) {
        editingIndex = -1;  // Indicates this is a new preset
        presetName[0] = '\0';
        sounds[0] = '\0';
        tuningParams[0] = '\0';
        maxOcclusion = 0.7f;
        isEditing = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Edit", ImVec2(80, 20))) {
        if (!presets.empty() && selectedPreset < presets.size()) {
            isEditing = true;
            editingIndex = selectedPreset;
            const auto& preset = presets[selectedPreset];
            strcpy(presetName, preset.name.c_str());
            strcpy(sounds, preset.sounds.c_str());
            strcpy(tuningParams, preset.tuningParams.c_str());
            maxOcclusion = preset.maxOcclusion;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete", ImVec2(80, 20))) {
        if (!presets.empty() && selectedPreset < presets.size()) {
            SettingsManager::getInstance().removeSoundPreset(presets[selectedPreset].name);
            // Adjust selectedPreset after deletion
            if (selectedPreset > 0) {
                selectedPreset--;
            }
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Reload Settings", ImVec2(120, 20))) {
        if (SettingsManager::getInstance().reloadSettings()) {
            ImGui::OpenPopup("Settings Reloaded");
        } else {
            ImGui::OpenPopup("Settings Reload Failed");
        }
    }

    // Popup for successful reload
    if (ImGui::BeginPopupModal("Settings Reloaded", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Settings have been successfully reloaded!");
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // Popup for failed reload
    if (ImGui::BeginPopupModal("Settings Reload Failed", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Failed to reload settings. Check the console for details.");
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void SettingsWindow::renderPresetEditForm() {
    const char* title = (editingIndex == -1) ? "Add New Preset" : "Edit Preset";
    ImGui::Text("%s", title);

    ImGui::Spacing();
    ImGui::Text("Preset Name:");
    ImGui::InputText("PresetName", presetName, IM_ARRAYSIZE(presetName));

    // Display error messages if the name is empty or already exists
    bool nameExists = false;
    bool hasErrors = false;
    if (isEditing && editingIndex != -1) {
        const auto& presets = SettingsManager::getInstance().getSoundPresets();
        if (editingIndex < presets.size()) {
            nameExists = strcmp(presetName, presets[editingIndex].name.c_str()) != 0 &&
                        SettingsManager::getInstance().hasSoundPreset(presetName);
        }
    } else {
        nameExists = SettingsManager::getInstance().hasSoundPreset(presetName);
    }

    if (strlen(presetName) == 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "The preset name cannot be empty");
        hasErrors = true;
    } else if (nameExists) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "A preset with this name already exists");
        hasErrors = true;
    }

    ImGui::Spacing();
    ImGui::Text("Sounds:");
    ImGui::InputText("Sounds", sounds, IM_ARRAYSIZE(sounds));
    if (strlen(sounds) == 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "The sounds field cannot be empty");
        hasErrors = true;
    }

    ImGui::Spacing();
    ImGui::Text("Tuning Parameters:");
    ImGui::InputText("TuningParams", tuningParams, IM_ARRAYSIZE(tuningParams));
    if (strlen(tuningParams) == 0) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "The tuning parameters field cannot be empty");
        hasErrors = true;
    }

    ImGui::Spacing();
    ImGui::Text("Max Occlusion:");
    ImGui::SliderFloat("MaxOcclusion", &maxOcclusion, 0.0f, 1.0f, "%.2f");

    ImGui::Spacing();
    bool canSave = !hasErrors;
    if (!canSave) {
        ImGui::BeginDisabled();
    }
    
    if (ImGui::Button(isEditing && editingIndex != -1 ? "Save Changes" : "Add Preset", ImVec2(100, 20))) {
        SoundPreset newPreset(presetName, sounds, tuningParams, maxOcclusion);
        
        if (isEditing && editingIndex != -1) {
            SettingsManager::getInstance().updateSoundPreset(editingIndex, newPreset);
        } else {
            SettingsManager::getInstance().addSoundPreset(newPreset);
        }
        resetForm();
    }
    
    if (!canSave) {
        ImGui::EndDisabled();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(100, 20))) {
        resetForm();
    }
}

void SettingsWindow::resetForm() {
    presetName[0] = '\0';
    sounds[0] = '\0';
    tuningParams[0] = '\0';
    maxOcclusion = 0.7f;
    isEditing = false;  // Reset edit mode when canceling
    editingIndex = 0;
} 