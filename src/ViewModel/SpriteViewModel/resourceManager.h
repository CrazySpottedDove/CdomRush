#pragma once

#include "Common/animation.h"
#include "Common/level.h"
#include "Common/macros.h"
#include "Common/towerEssential.h"
#include "Common/wave.h"
#include "sol/sol.hpp"
#include <vector>

class ResourceManager
{
public:
    ResourceManager();
    void                LoadLevelResources(const std::string& level_name);
    TextureMap*         GetTextureMap() { return &texture_map; }
    SpriteFrameDataMap* GetSpriteFrameDataMap() { return &sprite_frame_data_map; }
    AnimationGroupMap*  GetAnimationGroupMap() { return &animation_group_map; }
    Paths*              GetPaths() { return &paths; }
    std::vector<LevelData>* GetLevels() { return &levels; }
    std::vector<Wave>*  GetWaves() { return &waves; }
    std::vector<TowerEssential>* GetTowerEssentials() { return &tower_essentials; }
private:
    TextureMap                  texture_map;
    TextureLevelMap             texture_level_map;
    SpriteFrameDataMap          sprite_frame_data_map;
    PrefixLevelMap              prefix_level_map;
    AnimationGroupMap           animation_group_map;
    Paths                       paths;
    std::vector<LevelData>      levels;
    std::vector<Wave>           waves;
    std::vector<TowerEssential> tower_essentials;
    void                        LoadTexture(const std::string& file_name, TextureLevel level);
    void                        UnloadSpecificTexturesAndSpriteFrameDatas();
    void                        LoadTexturesAndSpriteFrameDatas(const std::string& file_name,
                                                                const TextureLevel level = TextureLevel::Common);
    void                        LoadAnimationGroups();
    void                        LoadLevelAssets(const std::string& level_name);
    void                        LoadTowerEssentials(const std::string& file_name);
    void                        LoadPaths(const std::string& file_name);
    void                        LoadWaves(const std::string& file_name);
    void ParseSpriteFrameData(const sol::table& frame_data_unparsed, SpriteFrameData& frame_data);
};