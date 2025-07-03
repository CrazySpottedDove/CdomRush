#include "ViewModel/SpiritViewModel/resourceManager.h"
#include "Common/macros.h"
#include "ViewModel/SpiritViewModel/readLua.h"

void ResourceManager::LoadTexture(const std::string& file_name, TextureLevel level){
    if(texture_map.find(file_name) == texture_map.end()){
        INFO("Loading texture: " << file_name);
        


    }
}
void UnloadSpecificTexturesAndSpriteFrameDatas();
void LoadTexturesAndSpriteFrameDatas(const std::string& file_name,
                                     const TextureLevel level = TextureLevel::Common);
void LoadAnimationGroups();
void LoadLevelDatas(const std::string& level_name);
void LoadTowers(const std::string& level_name, std::unordered_map<ID, Tower*>& towers);
void LoadPaths(const std::string& file_name);
void LoadWaves(const std::string& file_name);