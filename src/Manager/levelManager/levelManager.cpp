#include "Manager/levelManager/levelManager.h"
#include "ViewModel/GameViewModel/Function/calc/motion.h"
#include "Manager/resourceManager/textureManager.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/towers/towers.h"
#include "Common/macros.h"
#include "utils/readLua.h"
#include <sol/table.hpp>
#include <string>
#include <unordered_map>

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

void LevelManager::ReadLevelDataFile(LevelAsset& level_data)
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

void LevelManager::LoadTowerPositions(Store& store)
{
    static const std::unordered_map<std::string, TowerType> tower_type_map = {
        {"tower_holder_grass", TowerType::None},
        {"tower_archer_1", TowerType::Archer1},
        {"tower_archer_2", TowerType::Archer2},
        {"tower_archer_3", TowerType::Archer3},
        {"tower_engineer_1", TowerType::Engineer1},
        {"tower_engineer_2", TowerType::Engineer2},
        {"tower_engineer_3", TowerType::Engineer3},
        {"tower_mage_1", TowerType::Mage1},
        {"tower_mage_2", TowerType::Mage2},
        {"tower_mage_3", TowerType::Mage3}};
    const std::string file_path           = std::string(TOWER_PATH) + current_level_name + ".lua";
    const sol::table  tower_positions_map = ReadLua(file_path);
    for (const auto& [key, value] : tower_positions_map) {
        const sol::table tower_position_table = value.as<sol::table>();
        const TowerType type = tower_type_map.at(tower_position_table["type"].get<std::string>());
        Tower* tower = store.template_manager.CreateTower(type);
        tower->position.x = tower_position_table["pos"]["x"].get<float>();
        tower->position.y = tower_position_table["pos"]["y"].get<float>();
        calc::map_position(tower->position);
        tower->rally_point.x = tower_position_table["rally_point"]["x"].get<float>();
        tower->rally_point.y = tower_position_table["rally_point"]["y"].get<float>();
        calc::map_position(tower->rally_point);
        store.QueueTower(tower);
    }
}

void LevelManager::LoadLevelResource(Store& store)
{
    LevelAsset level_data;
    ReadLevelDataFile(level_data);
    // 加载纹理 （和音频）
    for (const std::string& texture_lua_path : level_data.required_textures) {
        const std::string true_lua_path = std::string(IMAGES_PATH) + texture_lua_path;
        store.animation_manager.LoadSpriteFrameResources(true_lua_path, TextureLevel::Specific);
    }
    // 加载路径
    store.path_manager.ReadPathsFromLua(std::string(PATH_PATH) + current_level_name + ".lua");

    // 加载波次
    store.wave_manager.Init(std::string(WAVE_PATH) + current_level_name + ".lua");

    // 加载塔位
    LoadTowerPositions(store);
}