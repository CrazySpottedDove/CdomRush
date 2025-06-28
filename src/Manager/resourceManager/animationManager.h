#pragma once
#include "sol/state.hpp"
#include "textureManager.h"
#include <SFML/Graphics.hpp>
#include <cstddef>
#include <unordered_map>
#include <vector>

// 一个 map 将 prefix 和 它对应的各帧纹理(string->SpriteFrameData) 对应起来

// 一个 map 将 prefix 和 它对应的动画组(string->AnimationGroup) 对应起来

// 用枚举类表示动画组的类型

// 各帧纹理被整合到不同的 lua 文件中，动态加载

// 关于动画组的信息也被储存在别的 lua 文件中，动态加载，便于更新

/**
 * @brief 定义了一个帧的全部信息
 * textureName: 纹理名称
 * frameRect:   纹理中的矩形区域
 * displaySize: 显示的尺寸
 * trim:        剪裁信息，{left, top, right, bottom}
 */
struct SpriteFrameData
{
    std::string  textureName;   // "screen_map-2.png"
    sf::IntRect  frameRect;     // 对应 f_quad
    sf::Vector2i displaySize;   // 对应 size
    int          trim_left;
    int          trim_top;
    int          trim_right;
    int          trim_bottom;
};

struct AnimationGroup
{
    std::size_t from;   // 起始帧索引
    std::size_t to;     // 结束帧索引
};

enum class AnimationType
{
    Idle,
    Walk,
    Attack,
    Shoot,
    Death,
    WalkingLeftRight,
    WalkingUp,
    WalkingDown,
    Running,
    IdleUp,
    IdleDown,
    Flying,
    Hit,
    ShootingUp,
    ShootingDown
};

/**
 * @brief 用于处理单位的动画的数据库
 *
 */
class AnimationDB
{
public:
    std::unordered_map<std::string, std::vector<SpriteFrameData>> SpriteFrameMap;
    std::unordered_map<std::string, std::unordered_map<AnimationType, AnimationGroup>>
        AnimationGroupMap;
    /**
     * @brief Get the Animation Group object
     *
     * @param prefix
     * @param type
     * @param animation_group
     * @return const AnimationGroup&
     */
    const AnimationGroup& GetAnimationGroup(const std::string& prefix, const AnimationType type,
                                            AnimationGroup& animation_group) const
    {
        return AnimationGroupMap.at(prefix).at(type);
    }
};

class SpriteManager
{
public:
    SpriteManager();
    ~SpriteManager() = default;
    void LoadResources(const std::string& path, const TextureLevel level = TextureLevel::Common);
    void UnloadSpecificResources();

private:
    TextureManager texture_manager;
    AnimationDB    common_animation_db;
    AnimationDB    specific_animation_db;
    void           LoadSpriteFramesFromLua(const sol::table&  sprite_frames_table,
                                           const TextureLevel level = TextureLevel::Common);
    void           ParseSpriteFrameData(const sol::table& frame_table, SpriteFrameData& frame_data);
};
