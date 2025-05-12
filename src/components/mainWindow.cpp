#include "mainWindow.h"
#include <iostream>
#include "../libs/ImGuiFileDialog/ImGuiFileDialog.h"
#include <pugixml.hpp>

MainWindow::MainWindow() {
    doorWindow.setOnDoorAdded([this](const Door& door) {
        handleDoorAdded(door);
    });
    doorWindow.setOnDoorEdited([this](const Door& door, size_t index) {
        handleDoorEdited(door, index);
    });
    doorWindow.setOnCheckDoorExists([this](const char* name, int currentIndex) {
        return checkDoorExists(name, currentIndex);
    });
    doorWindow.onGetDoors = [this]() {
        return doors;
    };
}

void MainWindow::handleDoorAdded(const Door& door) {
    doors.push_back(door);
}

void MainWindow::handleDoorEdited(const Door& door, size_t index) {
    if (index < doors.size()) {
        doors[index] = door;
    }
}

void MainWindow::deleteDoor(size_t index) {
    if (index < doors.size()) {
        doors.erase(doors.begin() + index);
    }
}

bool MainWindow::checkDoorExists(const char* name, int currentIndex) {
    for (size_t i = 0; i < doors.size(); i++) {
        // Ignorer la porte en cours d'édition
        if (i == currentIndex) continue;
        
        if (doors[i].getName() == name) {
            return true;
        }
    }
    return false;
}

void MainWindow::importXmlFile(const std::string& filePath) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if (!result) {
        std::cerr << "Error loading XML file: " << result.description() << std::endl;
        return;
    }

    auto dat151Node = doc.child("Dat151");
    if (!dat151Node) {
        std::cerr << "No 'Dat151' node found in XML file" << std::endl;
        return;
    }

    auto itemsNode = dat151Node.child("Items");
    if (!itemsNode) {
        std::cerr << "No 'Items' node found in XML file" << std::endl;
        return;
    }

    for (auto itemNode : itemsNode.children("Item")) {
        if (std::string(itemNode.attribute("type").as_string()) == "DoorAudioSettings") {
            Door door;
            std::string name = itemNode.child("Name").text().as_string();
            // Remove 'd_' prefix if it exists
            if (name.substr(0, 2) == "d_") {
                name = name.substr(2);
            }
            door.setName(name);
            door.setSounds(itemNode.child("Sounds").text().as_string());
            door.setTuningParams(itemNode.child("TuningParams").text().as_string());
            door.setMaxOcclusion(itemNode.child("MaxOcclusion").attribute("value").as_float());

            // Check if a door with this name already exists
            auto it = std::find_if(doors.begin(), doors.end(), [&](const Door& d) {
                return d.getName() == name;
            });
            if (it != doors.end()) {
                *it = door; // Replace existing door
            } else {
                doors.push_back(door); // Add new door
            }
        }
    }
}

void MainWindow::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(500, 600));
    ImGui::Begin("GTA V Audio Door Tool", nullptr, 
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    // Top navigation buttons
    if (ImGui::Button("Import file", ImVec2(100, 20))) {
        IGFD::FileDialogConfig config;
        config.countSelectionMax = 1;
        config.flags = ImGuiFileDialogFlags_Modal;
        config.path = ".";
        config.fileName = "";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseImportFile", "Choose File to Import", ".xml", config);
    }

    ImGui::SameLine(ImGui::GetWindowWidth() - 110);
    if (ImGui::Button("Settings", ImVec2(100, 20))) {
        settingsWindow.open();
        ImGui::OpenPopup("Settings");
    }

    // Scrollable area for door cards
    ImGui::BeginChild("Doors", ImVec2(482, 480), true);

    bool isModalOpen = doorWindow.isModalOpen();
    for (size_t i = 0; i < doors.size(); i++) {
        const auto& door = doors[i];
        ImGui::BeginChild(("DoorCard" + std::to_string(i)).c_str(), ImVec2(450, 90), true);

        ImGui::BeginGroup();
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%zu | %s", i + 1, door.getName().c_str());
        ImGui::Text("%s", buffer);
        ImGui::EndGroup();

        float bouton_width_total = 120;
        float padding = 10.0f;
        ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - bouton_width_total - padding);

        if (isModalOpen) ImGui::BeginDisabled();
        if (ImGui::Button("Edit", ImVec2(60, 20))) {
            doorWindow.openForEdit(door, i);
        }
        if (isModalOpen) ImGui::EndDisabled();

        ImGui::SameLine();
        if (ImGui::Button("Delete", ImVec2(60, 20))) {
            deleteDoor(i);
        }

        ImGui::Text("Sound: %s", door.getSounds().c_str());
        ImGui::Text("Tuning: %s", door.getTuningParams().c_str());
        ImGui::Text("Max Occlusion: %.2f", door.getMaxOcclusion());

        ImGui::EndChild();
    }

    ImGui::EndChild();

    // Bottom action buttons
    ImGui::Columns(3, nullptr, false);
    // Column 1: left button
    if (ImGui::Button("Generate file", ImVec2(100, 20))) {
        IGFD::FileDialogConfig config;
        config.countSelectionMax = 1;
        config.flags = ImGuiFileDialogFlags_Modal;
        config.path = ".";
        config.fileName = "door_game.dat151.rel.xml";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFile", "Choose File", ".xml", config);
    }
    ImGui::NextColumn();
    // Column 2: centered text
    float textWidth = ImGui::CalcTextSize("made by tiwabs").x;
    float col2Width = ImGui::GetColumnWidth();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col2Width - textWidth) * 0.5f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.0f);
    ImGui::Text("made by tiwabs");
    ImGui::NextColumn();
    // Column 3: right button
    if (isModalOpen) {
        ImGui::BeginDisabled();
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 50);
    if (ImGui::Button("Add new door", ImVec2(100, 20))) {
        doorWindow.open();
        ImGui::OpenPopup("Add New Door");
    }
    if (isModalOpen) {
        ImGui::EndDisabled();
    }
    ImGui::Columns(1);
    
    // Display file dialogs
    if (ImGuiFileDialog::Instance()->Display("ChooseFile")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            doorWindow.generateXmlFile(filePath);
        }
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseImportFile")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
            importXmlFile(filePath);
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // Render modals
    settingsWindow.render();
    doorWindow.render();

    ImGui::End();
} 