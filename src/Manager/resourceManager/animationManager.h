#pragma once
#include "sol/state.hpp"
#include "textureManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Model/components/state.h"

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
    AnimationGroup(const std::size_t from, const std::size_t to): from(from), to(to) {}
};

// prefix->(状态->动画组)
typedef std::unordered_map<std::string, std::unordered_map<State, AnimationGroup>> AnimationGroupMap;

// prefix->纹理文件名
typedef std::unordered_map<std::string, std::vector<SpriteFrameData>> SpriteFrameDataMap;


class AnimationManager
{
public:
    AnimationManager();
    ~AnimationManager() = default;

    /**
     * @brief 加载某一个图集映射文件中定义的资源
     * @note 在进入游戏战斗前调用，加载 Specific 级别的资源
     * @note 在初始化时调用，加载 Common 级别的资源
     *
     * @param path
     * @param level
     */
    void LoadSpriteFrameResources(const std::string& path, const TextureLevel level = TextureLevel::Common);

    /**
     * @brief 释放 specific_sprite_frame_data_map 中的资源与 texture_manager 中对应的纹理资源
     *
     */
    void UnloadSpecificResources();

    /**
     * @brief 根据 prefix 和帧序数，返回对应的 SpriteFrameData 和纹理
     *
     * @param prefix
     * @param frame_index
     * @return const SpriteFrameData&
     */
    std::pair<const SpriteFrameData&, const sf::Texture&>
    RequireFrameData(const std::string& prefix, const std::size_t frame_index) const;

    /**
     * @brief 根据 prefix 和状态，返回对应的 AnimationGroup
     *
     * @param prefix
     * @param state
     * @return const AnimationGroup&
     */
    const AnimationGroup& RequireAnimationGroup(const std::string& prefix, const State state) const;

private:
    TextureManager texture_manager;
    SpriteFrameDataMap    common_sprite_frame_data_map;
    SpriteFrameDataMap    specific_sprite_frame_data_map;
    AnimationGroupMap animation_group_map;

    /**
     * @brief 从 lua 文件中加载精灵帧数据
     *
     * @param sprite_frames_table
     * @param level
     */
    void           LoadSpriteFrameDatasFromLua(const sol::table&  sprite_frames_table,
                                           const TextureLevel level = TextureLevel::Common);
    void           ParseSpriteFrameData(const sol::table& frame_table, SpriteFrameData& frame_data);

    /**
     * @brief 从 lua 文件中加载所有的动画组数据
     * @note 动画组数据较少，直接存储在内存中
     * @note lua 文件目录因此默认确定
     */
    void LoadAnimationGroupsFromLua();
};
