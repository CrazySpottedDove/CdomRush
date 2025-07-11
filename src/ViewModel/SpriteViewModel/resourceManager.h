#pragma once

#include "Common/animation.h"
#include "Common/macros.h"
#include "Common/resourceLevel.h"
#include "Common/sound.h"
#include "Common/towerEssential.h"
#include "ViewModel/level.h"
#include "ViewModel/wave.h"
#include "sol/sol.hpp"
#include <SFML/Graphics/Font.hpp>
#include <vector>

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();
    void        LoadLevelResources(const std::string& level_name, std::string& level_prepare_music,
                                   std::string& level_fight_music, double& gold);
    TextureMap* GetTextureMap() { return &texture_map; }
    SpriteFrameDataMap*          GetSpriteFrameDataMap() { return &sprite_frame_data_map; }
    AnimationGroupMap*           GetAnimationGroupMap() { return &animation_group_map; }
    const Paths*                 GetPaths() const { return &paths; }
    std::vector<LevelData>*      GetLevels() { return &levels; }
    std::vector<Wave>*           GetWaves() { return &waves; }
    std::vector<TowerEssential>* GetTowerEssentials() { return &tower_essentials; }
    SoundGroupMap*               GetSoundGroupMap() { return &sound_group_map; }
    SoundBufferMap*              GetSoundBufferMap() { return &sound_buffer_map; }
    MusicSet*                    GetMusicSet() { return &music_set; }
    sf::Font* GetFont() { return &font; }
    void                         UnloadSpecificTexturesAndSpriteFrameDatas();
    void                         UnloadSpecificSoundGroups();

private:
    TextureMap                  texture_map;
    TextureLevelMap             texture_level_map;
    SpriteFrameDataMap          sprite_frame_data_map;
    PrefixLevelMap              prefix_level_map;
    AnimationGroupMap           animation_group_map;
    Paths                       paths;
    SoundGroupMap               sound_group_map;
    SoundBufferMap              sound_buffer_map;
    SoundGroupLevelMap          sound_group_level_map;
    sf::Font                    font;
    MusicSet                    music_set;
    std::vector<LevelData>      levels;
    std::vector<Wave>           waves;
    std::vector<TowerEssential> tower_essentials;
    void                        LoadTexture(const std::string& file_path, ResourceLevel level);

    void                        LoadTexturesAndSpriteFrameDatas(const std::string&  file_path,
                                                                const ResourceLevel level = ResourceLevel::Common);
    // 加载声音组和声音组中的声音资源
    void LoadSoundGroups(const std::string&  file_path,
                         const ResourceLevel level = ResourceLevel::Common);

    void LoadAnimationGroups();
    void LoadLevelAssets(const std::string& level_name, std::string& level_prepare_music,
                         std::string& level_fight_music, double& gold);
    void LoadTowerEssentials(const std::string& file_name);
    void LoadPaths(const std::string& file_path);
    void LoadWaves(const std::string& file_path);
    void ParseSpriteFrameData(const sol::table& frame_data_unparsed, SpriteFrameData& frame_data);
};