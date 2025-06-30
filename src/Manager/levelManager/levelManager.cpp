#include "Manager/levelManager/levelManager.h"
#include "Manager/resourceManager/textureManager.h"
#include "Manager/store/store.h"
#include "utils/macros.h"
#include "utils/readLua.h"
#include <sol/table.hpp>
#include <string>

LevelManager::LevelManager()
{
    const sol::table level_map = ReadLua(LEVEL_MAP_FILE);
    for (const auto& [key, value] : level_map) {
        const sol::table level_data = value.as<sol::table>();
        levels.emplace_back(level_data["name"].get<std::string>(),
                            Position{level_data["position"]["x"].get<float>(),
                                     level_data["position"]["y"].get<float>()});
    }
}

void LevelManager::ReadLevelDataFile(LevelData& level_data)
{
    const std::string file_path      = std::string(LEVEL_DATA_PATH) + current_level_name + ".lua";
    const sol::table  level_data_map = ReadLua(file_path);
    const sol::table  required_textures = level_data_map["required_textures"];
    const sol::table  required_sounds   = level_data_map["required_sounds"];
    for (const auto& [key, value] : required_textures) {
        level_data.required_textures.push_back(value.as<std::string>());
    }
    for (const auto& [key, value] : required_sounds) {
        level_data.required_sounds.push_back(value.as<std::string>());
    }
}

void LevelManager::LoadLevelResource(Store& store)
{
    LevelData level_data;
    ReadLevelDataFile(level_data);
    for (const std::string& texture_lua_path : level_data.required_textures) {
        const std::string true_lua_path = std::string(IMAGES_PATH) + texture_lua_path;
        store.animation_manager.LoadSpriteFrameResources(true_lua_path, TextureLevel::Specific);
    }
    store.path_manager.ReadPathsFromLua(std::string(PATH_PATH) + current_level_name + ".lua");
}