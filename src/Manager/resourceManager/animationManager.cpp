#include "animationManager.h"
#include "Model/components/state.h"
#include "textureManager.h"
#include "utils/macros.h"
#include "utils/readLua.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <ostream>
#include <sol/state.hpp>
#include <sol/types.hpp>
#include <string>
#include <unordered_map>

/**
 * @brief 解析 lua 表成帧纹理
 *
 * @param frame_data_unparsed [in]
 * @param frame_data [out]
 */
void AnimationManager::ParseSpriteFrameData(const sol::table& frame_data_unparsed,
                                            SpriteFrameData&  frame_data)
{
    frame_data.textureName        = frame_data_unparsed["a_name"].get<std::string>();
    const sol::table   f_quad     = frame_data_unparsed["f_quad"];
    const sol::table   size_table = frame_data_unparsed["size"];
    const sol::table   trim_table = frame_data_unparsed["trim"];
    const sf::Vector2i pos        = {f_quad[1].get<int>(), f_quad[2].get<int>()};
    const sf::Vector2i size       = {f_quad[3].get<int>(), f_quad[4].get<int>()};
    frame_data.frameRect          = sf::IntRect(pos, size);
    frame_data.displaySize        = {size_table[1].get<int>(), size_table[2].get<int>()};
    frame_data.trim_left          = trim_table[1].get<int>();
    frame_data.trim_top           = trim_table[2].get<int>();
    frame_data.trim_right         = trim_table[3].get<int>();
    frame_data.trim_bottom        = trim_table[4].get<int>();
}

/**
 * @brief 从 Lua 表中加载精灵帧数据
 *
 * @param sprite_frames_table
 */
void AnimationManager::LoadSpriteFrameDatasFromLua(const sol::table&  sprite_frames_table,
                                                   const TextureLevel level)
{
    for (const auto& pair : sprite_frames_table) {
        const std::string prefix       = pair.first.as<std::string>();
        const sol::table& frame_tables = pair.second.as<sol::table>();
        for (const auto& frame_table : frame_tables) {
            const sol::table frame_data_unparsed = frame_table.second.as<sol::table>();
            SpriteFrameData  frame_data;
            ParseSpriteFrameData(frame_data_unparsed, frame_data);
            std::cout << "Try to load texture:" << IMAGES_PATH + frame_data.textureName;
            texture_manager.LoadTexture(IMAGES_PATH + frame_data.textureName, level);
            switch (level) {
            case TextureLevel::Common:
            {
                common_sprite_frame_data_map[prefix].push_back(frame_data);
                break;
            }
            case TextureLevel::Specific:
            {
                specific_sprite_frame_data_map[prefix].push_back(frame_data);
                break;
            }
            }
        }
    }
}

void AnimationManager::LoadSpriteFrameResources(const std::string& path, const TextureLevel level)
{
    std::cout << "-1";
    const sol::table& sprite_frames_table = ReadLua(path);

    LoadSpriteFrameDatasFromLua(sprite_frames_table, level);
}

/**
 * @brief Construct a new Sprite Manager:: Sprite Manager object
 * @note 创建 SpriteManager 时会自动加载 assets/images/common 目录下的所有 .lua
 * 文件。这些文件定义了通用的精灵帧数据。
 */
AnimationManager::AnimationManager()
{
    namespace fs = std::filesystem;

    try {
        size_t loaded_count = 0;
        // 获得所有 common 级别的 sprite_frame_data
        for (const auto& entry : fs::directory_iterator("assets/images/common")) {
            if (entry.is_regular_file()) {
                const auto& path = entry.path();

                // 只处理 .lua 文件
                if (path.extension() == ".lua") {
                    std::cout << "  Loading: " << path.filename() << std::endl;

                    try {
                        std::cout << 0;
                        LoadSpriteFrameResources(path.string());
                        ++loaded_count;
                    }
                    catch (const std::exception& e) {
                        std::cerr << "  Failed to load " << path.filename() << ": " << e.what()
                                  << std::endl;
                    }
                }
            }
        }
        // LoadSpriteFrameResources("assets/images/common/common.lua");

        std::cout << "Successfully loaded " << loaded_count << std::endl;

        // 初始化 AnimationGroupMap
        LoadAnimationGroupsFromLua();
        std::cout << "Animation groups loaded." << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

void AnimationManager::UnloadSpecificResources()
{
    specific_sprite_frame_data_map.clear();
    texture_manager.UnloadSpecificTextures();
    std::cout << "Specific resources unloaded." << std::endl;
}

void AnimationManager::LoadAnimationGroupsFromLua()
{
    static std::unordered_map<std::string, State> state_map = {
        {"attack", State::Attack},
        {"idle", State::Idle},
        {"walk", State::Walk},
        {"shoot", State::Shoot},
        {"death", State::Death},
        {"walkingLeftRight", State::WalkingLeftRight},
        {"walkingUp", State::WalkingUp},
        {"walkingDown", State::WalkingDown},
        {"running", State::Running},
        {"idleUp", State::IdleUp},
        {"idleDown", State::IdleDown},
        {"flying", State::Flying},
        {"hit", State::Hit},
        {"shootingUp", State::ShootingUp},
        {"shootingDown", State::ShootingDown}};
    const sol::table& animation_groups_table = ReadLua("assets/images/animation_groups.lua");

    for (const auto& pair : animation_groups_table) {
        const std::string prefix      = pair.first.as<std::string>();
        const sol::table  state_group = pair.second.as<sol::table>();

        for (const auto& state_pair : state_group) {
            const State      state             = state_map[state_pair.first.as<std::string>()];
            const sol::table frames_table      = state_pair.second.as<sol::table>();
            animation_group_map[prefix][state] = AnimationGroup(
                frames_table["from"].get<std::size_t>(), frames_table["to"].get<std::size_t>());
        }
    }
}

/**
 * @brief 根据 prefix 和帧序数，返回对应的 SpriteFrameData 和纹理
 *
 * @param prefix
 * @param frame_index
 * @return const SpriteFrameData&
 */
std::pair<const SpriteFrameData&, const sf::Texture&> AnimationManager::RequireFrameData(
    const std::string& prefix, const std::size_t frame_index) const
{
    auto it = common_sprite_frame_data_map.find(prefix);
    if (it != common_sprite_frame_data_map.end()) {
        const auto& frame_data = it->second[frame_index - 1];
        return {frame_data, texture_manager.getTexture(IMAGES_PATH + frame_data.textureName)};
    }
    else if (specific_sprite_frame_data_map.find(prefix) != specific_sprite_frame_data_map.end()) {
        const auto& frame_data = specific_sprite_frame_data_map.at(prefix)[frame_index - 1];
        return {frame_data, texture_manager.getTexture(IMAGES_PATH+ frame_data.textureName)};
    }
    else {
        throw std::runtime_error("SpriteFrameData not found for prefix: " + prefix);
    }
}

/**
 * @brief 根据 prefix 和状态，返回对应的 AnimationGroup
 *
 * @param prefix
 * @param state
 * @return const AnimationGroup&
 */
const AnimationGroup& AnimationManager::RequireAnimationGroup(const std::string& prefix,
                                                              const State        state) const
{
    auto it = animation_group_map.find(prefix);
    if (it == animation_group_map.end()) {
        throw std::runtime_error("AnimationGroup not found for prefix: " + prefix);
    }
    auto state_it = it->second.find(state);
    if (state_it == it->second.end()) {
        throw std::runtime_error(
            "AnimationGroup not found for state: " + std::to_string(static_cast<int>(state)) +
            " for prefix: " + prefix);
    }
    return state_it->second;
}