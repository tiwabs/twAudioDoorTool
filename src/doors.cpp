#include "doors.h"

Door::Door(const std::string& name, const std::string& sounds, 
           const std::string& tuningParams, float maxOcclusion)
    : name(name)
    , sounds(sounds)
    , tuningParams(tuningParams)
    , maxOcclusion(maxOcclusion)
{}

nlohmann::json Door::toJson() const {
    nlohmann::json j;
    j["name"] = name;
    j["sounds"] = sounds;
    j["tuningParams"] = tuningParams;
    j["maxOcclusion"] = maxOcclusion;
    return j;
}

Door Door::fromJson(const nlohmann::json& j) {
    Door door;
    door.name = j["name"].get<std::string>();
    door.sounds = j["sounds"].get<std::string>();
    door.tuningParams = j["tuningParams"].get<std::string>();
    door.maxOcclusion = j["maxOcclusion"].get<float>();
    return door;
}
