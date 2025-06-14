#include "doorWindow.h"
#include <cstring>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <pugixml.hpp>

DoorWindow::DoorWindow() : isOpen(false), isEditing(false), editingIndex(0), maxOcclusion(0.7f), selectedPreset(0) {
    doorName[0] = '\0';
    sounds[0] = '\0';
    tuningParams[0] = '\0';
    
    // Initialize with the first preset if available
    const auto& presets = SettingsManager::getInstance().getSoundPresets();
    if (!presets.empty()) {
        strcpy(sounds, presets[0].sounds.c_str());
        strcpy(tuningParams, presets[0].tuningParams.c_str());
        maxOcclusion = presets[0].maxOcclusion;
    }
}

void DoorWindow::openForEdit(const Door& door, size_t index) {
    isOpen = true;
    isEditing = true;
    editingIndex = index;
    
    // Fill form with door data
    strcpy(doorName, door.getName().c_str());
    strcpy(sounds, door.getSounds().c_str());
    strcpy(tuningParams, door.getTuningParams().c_str());
    maxOcclusion = door.getMaxOcclusion();
}

void DoorWindow::generateXmlFile(const std::string& filePath) {
    pugi::xml_document doc;
    
    // Create XML declaration
    pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";
    
    // Create root node
    pugi::xml_node root = doc.append_child("Dat151");
    
    // Add Version node
    pugi::xml_node version = root.append_child("Version");
    version.append_attribute("value") = "9458585";
    
    // Add Items node
    pugi::xml_node items = root.append_child("Items");
    
    // Get all doors from the callback
    if (onGetDoors) {
        auto doors = onGetDoors();
        
        // First pass: Generate all DoorAudioSettings
        for (const auto& door : doors) {
            std::string doorName = door.getName();
            std::string doorPrefix = "d_" + doorName;

            // DoorAudioSettings
            pugi::xml_node das = items.append_child("Item");
            das.append_attribute("type") = "DoorAudioSettings";
            das.append_attribute("ntOffset") = "0";
            das.append_child("Name").text() = doorPrefix.c_str();
            das.append_child("Sounds").text() = door.getSounds().c_str();
            das.append_child("TuningParams").text() = door.getTuningParams().c_str();
            pugi::xml_node maxOcclusion = das.append_child("MaxOcclusion");
            maxOcclusion.append_attribute("value") = door.getMaxOcclusion();
        }
        
        // Second pass: Generate all DoorAudioSettingsLink
        for (const auto& door : doors) {
            std::string hash = calculateJoaatHash(door.getName().c_str());
            std::string doorName = door.getName();
            std::string doorPrefix = "d_" + doorName;
            std::string daslName = "dasl_" + hash;
            
            // DoorAudioSettingsLink
            pugi::xml_node dasl = items.append_child("Item");
            dasl.append_attribute("type") = "DoorAudioSettingsLink";
            dasl.append_attribute("ntOffset") = "0";
            dasl.append_child("Name").text() = daslName.c_str();
            dasl.append_child("Door").text() = doorPrefix.c_str();
        }
    }
    
    // Save the document
    doc.save_file(filePath.c_str());
}

void DoorWindow::render() {
    if (!isOpen) return;

    const char* title = isEditing ? "Edit Door" : "Add New Door";
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(title, &isOpen, 
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse)) {

        ImGui::Text("Door Name:");
        ImGui::InputText("DoorName", doorName, IM_ARRAYSIZE(doorName));

        // Display error messages if the name is empty or already exists
        bool nameExists = false;
        if (onCheckDoorExists) {
            nameExists = onCheckDoorExists(doorName, isEditing ? editingIndex : -1);
        }

        if (strlen(doorName) == 0) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "The door name cannot be empty");
        } else if (nameExists) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "A door with this name already exists");
        }

        ImGui::Spacing();
        ImGui::Text("Sound Preset:");
        const auto& presets = SettingsManager::getInstance().getSoundPresets();
        
        // Automatic initialization if the field is empty and there are presets
        if (presets.size() > 0 && sounds[0] == '\0') {
            const auto& preset = presets[selectedPreset];
            strcpy(sounds, preset.sounds.c_str());
            strcpy(tuningParams, preset.tuningParams.c_str());
            maxOcclusion = preset.maxOcclusion;
        }
        
        if (ImGui::BeginCombo("Presets", presets.empty() ? "No presets" : presets[selectedPreset].name.c_str())) {
            for (size_t i = 0; i < presets.size(); i++) {
                bool is_selected = (selectedPreset == i);
                if (ImGui::Selectable(presets[i].name.c_str(), is_selected)) {
                    selectedPreset = i;
                    const auto& preset = presets[i];
                    strcpy(sounds, preset.sounds.c_str());
                    strcpy(tuningParams, preset.tuningParams.c_str());
                    maxOcclusion = preset.maxOcclusion;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Spacing();
        bool canSave = strlen(doorName) > 0 && !nameExists;
        if (!canSave) {
            ImGui::BeginDisabled();
        }
        
        if (ImGui::Button(isEditing ? "Save Changes" : "Add Door", ImVec2(100, 20))) {
            Door newDoor(doorName, sounds, tuningParams, maxOcclusion);
            
            if (isEditing) {
                if (onDoorEdited) {
                    onDoorEdited(newDoor, editingIndex);
                }
            } else {
                if (onDoorAdded) {
                    onDoorAdded(newDoor);
                }
            }
            resetForm();
        }
        
        if (!canSave) {
            ImGui::EndDisabled();
        }

        ImGui::End();
    }
    
    // If window is closed (isOpen became false)
    if (!isOpen) {
        isEditing = false;  // Reset edit mode
    }
}

void DoorWindow::resetForm() {
    doorName[0] = '\0';
    sounds[0] = '\0';
    tuningParams[0] = '\0';
    maxOcclusion = 0.7f;
    selectedPreset = 0;
    isOpen = false;
    isEditing = false;

    // If there are presets, initialize the fields with the first preset
    const auto& presets = SettingsManager::getInstance().getSoundPresets();
    if (!presets.empty()) {
        strcpy(sounds, presets[0].sounds.c_str());
        strcpy(tuningParams, presets[0].tuningParams.c_str());
        maxOcclusion = presets[0].maxOcclusion;
    }
}

std::string DoorWindow::calculateJoaatHash(const char* str) {
    uint32_t hash = 0;
    std::string keyLowered;
    
    for (const char* p = str; *p; ++p) {
        keyLowered += std::tolower(*p);
    }
    
    for (size_t i = 0; i < keyLowered.length(); i++) {
        hash += static_cast<uint32_t>(keyLowered[i]);
        hash += (hash << 10);
        hash ^= (static_cast<uint32_t>(hash) >> 6);
    }
    
    hash += (hash << 3);
    hash ^= (static_cast<uint32_t>(hash) >> 11);
    hash += (hash << 15);
    
    std::stringstream ss;
    ss << std::hex << hash;
    std::string hashedStr = ss.str();
    
    while (hashedStr.length() < 8) {
        hashedStr = "0" + hashedStr;
    }
    
    return hashedStr;
} 