#include "settingsWindow.h"

SettingsWindow::SettingsWindow() {}

void SettingsWindow::render() {
    if (ImGui::BeginPopupModal("Settings", &isOpen, 
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {
        renderPresetManager();

        ImGui::Spacing();

        ImGui::EndPopup();
    }
}

void SettingsWindow::renderPresetManager() {
    ImGui::Text("Door Presets:");
    const auto& presets = SettingsManager::getInstance().getSoundPresets();
    static int selectedPreset = 0;
    
    if (ImGui::BeginCombo("##Presets", presets.empty() ? "No presets" : presets[selectedPreset].name.c_str())) {
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
        // TODO: Implement add preset
    }
    ImGui::SameLine();
    if (ImGui::Button("Edit", ImVec2(80, 20))) {
        // TODO: Implement edit preset
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete", ImVec2(80, 20))) {
        if (!presets.empty() && selectedPreset < presets.size()) {
            SettingsManager::getInstance().removeSoundPreset(presets[selectedPreset].name);
        }
    }
} 