#pragma once

#include "Common/animation.h"
#include "Common/level.h"
#include "Common/macros.h"
#include "Common/wave.h"
#include "ViewModel/GameViewModel/towers/towers.h"
#include <unordered_map>
#include <vector>
class ResourceManager
{
public:
private:
    TextureMap             texture_map;
    TextureLevelMap        texture_level_map;
    SpriteFrameDataMap     common_sprite_frame_data_map;
    SpriteFrameDataMap     specific_sprite_frame_data_map;
    AnimationGroupMap      animation_group_map;
    Paths                  paths;
    std::vector<LevelData> levels;
    std::vector<Wave> waves;
    void                   LoadTexture(const std::string& file_name, TextureLevel level);
    void                   UnloadSpecificTexturesAndSpriteFrameDatas();
    void                   LoadTexturesAndSpriteFrameDatas(const std::string& file_name,
                                                           const TextureLevel level = TextureLevel::Common);
    void                   LoadAnimationGroups();
    void                   LoadLevelDatas(const std::string& level_name);
    void LoadTowers(const std::string& level_name, std::unordered_map<ID, Tower*>& towers);
    void LoadPaths(const std::string& file_name);
    void LoadWaves(const std::string& file_name);
};