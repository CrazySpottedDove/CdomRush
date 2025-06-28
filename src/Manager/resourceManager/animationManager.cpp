#include "textureManager.h"
#include "animationManager.h"
#include "utils/macros.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <ostream>
#include <sol/state.hpp>
#include <sol/types.hpp>
#include <string>
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
void AnimationManager::LoadSpriteFramesFromLua(const sol::table&  sprite_frames_table,
                                            const TextureLevel level)
{
    for (const auto& pair : sprite_frames_table) {
        const std::string prefix       = pair.first.as<std::string>();
        const sol::table  frame_tables = pair.second.as<sol::table>();
        for (const auto& frame_table : frame_tables) {
            const sol::table frame_data_unparsed = frame_table.second.as<sol::table>();
            SpriteFrameData  frame_data;
            ParseSpriteFrameData(frame_data_unparsed, frame_data);
            texture_manager.LoadTexture(IMAGES_PATH + frame_data.textureName, level);
            common_animation_db.SpriteFrameMap[prefix].push_back(frame_data);
        }
    }
}

/**
 * @brief 加载 lua 文件资源
 *
 * @param path
 */
void AnimationManager::LoadResources(const std::string& path, const TextureLevel level)
{
    try {
        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::string);
        const sol::object result              = lua.script_file(path);
        const sol::table  sprite_frames_table = result.as<sol::table>();
        LoadSpriteFramesFromLua(sprite_frames_table, level);
    }
    catch (const sol::error& e) {
        std::cerr << "Error loading Lua file '" << path << "': " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
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
        for (const auto& entry : fs::directory_iterator("assets/images/common")) {
            if (entry.is_regular_file()) {
                const auto& path = entry.path();

                // 只处理 .lua 文件
                if (path.extension() == ".lua") {
                    std::cout << "  Loading: " << path.filename() << std::endl;

                    try {
                        LoadResources(path.string());
                        loaded_count++;
                    }
                    catch (const std::exception& e) {
                        std::cerr << "  Failed to load " << path.filename() << ": " << e.what()
                                  << std::endl;
                    }
                }
            }
        }

        std::cout << "Successfully loaded " << loaded_count << std::endl;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

/**
 * @brief 卸载非常态资源
 */
void AnimationManager::UnloadSpecificResources()
{
    specific_animation_db.SpriteFrameMap.clear();
    texture_manager.UnloadSpecificTextures();
    std::cout << "Specific resources unloaded." << std::endl;
}

