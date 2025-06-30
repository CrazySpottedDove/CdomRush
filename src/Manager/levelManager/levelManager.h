#pragma once

#include "Manager/resourceManager/animationManager.h"
#include "utils/macros.h"
#include <string>
#include <utility>
#include <vector>

// (关卡名称，关卡点的位置)
typedef std::pair<std::string, Position> Level;
struct LevelData{
    std::vector<std::string> required_textures;
    std::vector<std::string> required_sounds;
};

class LevelManager{
public:
    /**
     * @brief 加载默认的 level_map 资源
     *
     */
    LevelManager();
    std::vector<Level> levels;
    void SetCurrentLevelName(const std::string& level_name){
        current_level_name = level_name;
    }

    /**
     * @brief 加载 current_level_name 对应的关卡资源
     *
     * @param animation_manager
     */
    void LoadLevelResource(AnimationManager& animation_manager);
private:
    void        ReadLevelDataFile(LevelData& level_data);
    std::string current_level_name;
};