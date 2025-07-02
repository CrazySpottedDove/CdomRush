#include "FlagUI.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include "utils/macros.h"

/**
 * @brief 构造函数
 */
FlagUI::FlagUI(std::string level_name,
               const sf::Vector2f& position,
               AnimationPlayer& animation_player,
               const AnimationManager& animation_manager,
               LevelStatus initial_status)
    : FxUI(fx_, animation_player_)
    , level_name_(level_name)
    , position_(position)
    , level_status_(initial_status)
    , is_hovered_(false)
    , initialized_(false)
    , animation_player_(animation_player)
    , animation_manager_(animation_manager)
    , animation_context_()
{
    DEBUG_CODE(std::cout << "FlagUI: Created flag for level " << level_name_
              << " at position (" << position_.x << "," << position_.y << ")"
              << " with status: " << static_cast<int>(level_status_) << std::endl;)
}

/**
 * @brief 渲染旗子（自渲染实现，支持高亮）
 */
void FlagUI::Render(sf::RenderWindow& window, const Position& position, Animation& animation, const sf::Vector2f& scale)
{
    // 初始化动画（仅在第一次调用时）
    if (!initialized_) {
        animation_player_.PlayAnimationState(animation, animation_context_,  true);
        initialized_ = true;
    }

    // 更新动画
    animation_player_.UpdateAnimation(animation, animation_context_);

    try {
        // 获取当前帧的sprite
        sf::Sprite sprite = GetCurrentFrameSprite(position,animation,scale);

        // 根据悬停状态设置颜色
        if (is_hovered_) {
            // 悬停高亮：淡黄色叠加
            sprite.setColor(sf::Color(255, 255, 180, 255));
        } else {
            // 正常渲染：白色（不改变原始颜色）
            sprite.setColor(sf::Color::White);
        }
        // 渲染到窗口
        window.draw(sprite);

    } catch (const std::exception& e) {
        std::cerr << "FlagUI: Error rendering flag " << level_name_ << ": " << e.what() << std::endl;
    }
}

/**
 * @brief 获取当前动画帧的sprite（为了自渲染）
 */
sf::Sprite FlagUI::GetCurrentFrameSprite(const sf::Vector2f& position,
                                      Animation& animation, 
                                      //const AnimationContext& context, 
                                      const sf::Vector2f& scale) const
{
    // 计算实际帧索引（参考AnimationPlayer的逻辑）
    const auto& animation_group = animation_manager_.RequireAnimationGroup(
        animation.prefix, animation.state);

    std::size_t actual_frame_index = animation_group.from + animation.frame_id;

    // 确保帧索引在有效范围内
    if (actual_frame_index > animation_group.to) {
        actual_frame_index = animation_group.to;
    }

    // 获取帧数据和纹理
    const auto& [frame_data, texture] = animation_manager_.RequireFrameData(
        animation.prefix, actual_frame_index);

    // 创建sprite并设置属性
    sf::Sprite sprite(texture);

    // 设置纹理裁剪区域
    sprite.setTextureRect(frame_data.frameRect);

    // 设置原点到图像中心
    sprite.setOrigin(sf::Vector2f(
        static_cast<float>(frame_data.frameRect.size.x) / 2.0f,
        static_cast<float>(frame_data.frameRect.size.y) / 2.0f
    ));

    // 设置缩放
    sprite.setScale(scale);

    // 计算最终渲染位置
    sf::Vector2f final_position = position;
    final_position.x += static_cast<float>(frame_data.trim_left) * scale.x;
    final_position.y += static_cast<float>(frame_data.trim_top) * scale.y;

    // 加上图像中心的偏移
    final_position.x += static_cast<float>(frame_data.frameRect.size.x) / 2.0f * scale.x;
    final_position.y += static_cast<float>(frame_data.frameRect.size.y) / 2.0f * scale.y;

    sprite.setPosition(final_position);

    return sprite;
}

/**
 * @brief 处理鼠标事件
 */
bool FlagUI::HandleMouseEvent(const sf::Event& event, const sf::Vector2f& mouse_world_pos)
{
    if(event.is<sf::Event::MouseMoved>()){
        UpdateHoverState(mouse_world_pos);
    }else if(event.is<sf::Event::MouseButtonPressed>()){
        auto event_button = event.getIf<sf::Event::MouseButtonPressed>();
        if (event_button->button == sf::Mouse::Button::Left) {
            if (IsMouseOver(mouse_world_pos) ) {
                DEBUG_CODE(std::cout << "FlagUI: Level " << level_name_ << " clicked!" << std::endl;)

                // 把事件传回给store
                
                return true;   // 事件被处理
            }
        }
    }

    return false;  // 事件未被处理
}

/**
 * @brief 设置关卡状态
 */
void FlagUI::SetLevelStatus(LevelStatus status)
{
    if (level_status_ != status) {
        LevelStatus old_status = level_status_;
        level_status_ = status;

        DEBUG_CODE(std::cout << "FlagUI: Level " << level_name_ << " status changed from "
                  << static_cast<int>(old_status) << " to " << static_cast<int>(status) << std::endl;)

    }
}


/**
 * @brief 检查点击区域
 */
bool FlagUI::IsMouseOver(const sf::Vector2f& mouse_pos) const
{
    sf::FloatRect bounds = GetClickBounds();
    return bounds.contains(mouse_pos);
}


/**
 * @brief 获取旗子的点击区域
 */
sf::FloatRect FlagUI::GetClickBounds() const
{
    //假设旗子的点击区域是一个64x64的正方形，以旗子位置为中心
    const float flag_size = 64.0f;
    return sf::FloatRect(
        position_.x - flag_size / 2.0f,
        position_.y - flag_size / 2.0f,
        flag_size,
        flag_size
    );
}

/**
 * @brief 处理鼠标悬停逻辑
 */
void FlagUI::UpdateHoverState(const sf::Vector2f& mouse_pos)
{
    bool was_hovered = is_hovered_;

    // available的旗子响应悬停
    is_hovered_ = IsMouseOver(mouse_pos) ;

    // 悬停状态改变时输出日志
    if (was_hovered != is_hovered_) {
        if (is_hovered_ && !was_hovered) {
            DEBUG_CODE(std::cout << "FlagUI: Level " << level_name_ << " hovered" << std::endl;)
        } else if (!is_hovered_ && was_hovered) {
            DEBUG_CODE(std::cout << "FlagUI: Level " << level_name_ << " unhovered" << std::endl;)
        }
    }
}