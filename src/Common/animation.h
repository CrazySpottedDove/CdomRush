#pragma once
#include <Common/action.h>
#include <Common/state.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class TextureLevel
{
    Common,
    Specific
};

// TextureName -> Texture
typedef std::unordered_map<std::string, sf::Texture> TextureMap;

// TextureLevel -> TextureName
typedef std::unordered_map<TextureLevel, std::unordered_set<std::string>> TextureLevelMap;

// TextureLevel -> Prefix
typedef std::unordered_map<TextureLevel, std::unordered_set<std::string>> PrefixLevelMap;

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
    AnimationGroup() = default;
    AnimationGroup(const std::size_t from, const std::size_t to)
        : from(from)
        , to(to)
    {}
};

// prefix->(State->AnimationGroup)
typedef std::unordered_map<std::string, std::unordered_map<State, AnimationGroup>>
    AnimationGroupMap;

// prefix->(std::vector<SpriteFrameData>)
typedef std::unordered_map<std::string, std::vector<SpriteFrameData>> SpriteFrameDataMap;

struct Animation
{
    Animation() = default;   // 默认构造函数
    Animation(const State state, const std::string& prefix, Position offset_ = sf::Vector2f(0,0), bool hidden_ = false)
        : current_state(state)
        , prefix(prefix)
        , offset(offset_)
        ,hidden(hidden_)
    {}
    State               current_state;   // 当前状态
    std::string         prefix;          // 动画的前缀名
    std::vector<Action> actions;
    Position            offset     = sf::Vector2f(0,0);
    State               last_state = State::None;   // 上一个状态
    double              rotation   = 0.0;           // 旋转角度
    bool                pending    = true;          // 动画正在进行
    bool                flip       = false;
    float               anchor_x   = 0.5f;
    float               anchor_y   = 0.5f;
    float               scale_x    = 1.0f;    // 水平缩放
    float               scale_y    = 1.0f;    // 垂直
    bool                clicked    = false;   // 是否处于被点击状态 (决定 actions UI 是否展开)
    bool                hidden     = false;
    std::size_t         frame_id   = 1;       // 动画进行到第几帧
};