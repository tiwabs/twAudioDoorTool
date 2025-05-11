#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class Door {
public:
    Door() = default;
    Door(const std::string& name, const std::string& sounds, 
         const std::string& tuningParams, float maxOcclusion);

    // Getters
    const std::string& getName() const { return name; }
    const std::string& getSounds() const { return sounds; }
    const std::string& getTuningParams() const { return tuningParams; }
    float getMaxOcclusion() const { return maxOcclusion; }

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setSounds(const std::string& newSounds) { sounds = newSounds; }
    void setTuningParams(const std::string& newParams) { tuningParams = newParams; }
    void setMaxOcclusion(float newOcclusion) { maxOcclusion = newOcclusion; }

    // JSON serialization
    nlohmann::json toJson() const;
    static Door fromJson(const nlohmann::json& j);

private:
    std::string name;
    std::string sounds;
    std::string tuningParams;
    float maxOcclusion = 0.7f;
};
