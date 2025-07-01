#include "FlagUI.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include "utils/macros.h"

/**
 * @brief 构造函数
 */
FlagUI::FlagUI(int level_id,
               const sf::Vector2f& position,
               AnimationPlayer& animation_player,
               const AnimationManager& animation_manager,
               LevelStatus initial_status)
    : level_id_(level_id)
    , position_(position)
    , level_status_(initial_status)
    , is_hovered_(false)
    , initialized_(false)
    , animation_player_(animation_player)
    , animation_manager_(animation_manager)
    , animation_context_()
    , click_callback_(nullptr)
{
    // 初始化虚拟实体数据 - 用于AnimationPlayer
    virtual_entity_.position = position_;
    virtual_entity_.animation.prefix = "map_flag";  // 统一使用map_flag前缀
    virtual_entity_.animation.state = State::Idle;  // 始终使用Idle状态
    virtual_entity_.animation.rotation = 0.0;
    virtual_entity_.animation.pending = false;
    virtual_entity_.animation.frame_id = 0;

    DEBUG_CODE(std::cout << "FlagUI: Created flag for level " << level_id_
              << " at position (" << position_.x << "," << position_.y << ")"
              << " with status: " << static_cast<int>(level_status_) << std::endl;)
}

/**
 * @brief 渲染旗子（自渲染实现，支持高亮）
 */
void FlagUI::Render(sf::RenderWindow& window, const sf::Vector2f& scale)
{
    // 初始化动画（仅在第一次调用时）
    if (!initialized_) {
        InitializeAnimation();
    }

    // 更新动画
    UpdateAnimation();

    try {
        // 获取当前帧的sprite
        sf::Sprite sprite = GetCurrentFrameSprite(scale);

        // 根据悬停状态设置颜色
        if (is_hovered_ && IsClickable()) {
            // 悬停高亮：淡黄色叠加
            sprite.setColor(sf::Color(255, 255, 180, 255));
        } else {
            // 正常渲染：白色（不改变原始颜色）
            sprite.setColor(sf::Color::White);
        }

        // 渲染到窗口
        window.draw(sprite);

    } catch (const std::exception& e) {
        std::cerr << "FlagUI: Error rendering flag " << level_id_ << ": " << e.what() << std::endl;
    }
}

/**
 * @brief 获取当前动画帧的sprite（自渲染核心方法）
 */
sf::Sprite FlagUI::GetCurrentFrameSprite(const sf::Vector2f& scale) const
{
    // 计算实际帧索引（参考AnimationPlayer的逻辑）
    const auto& animation_group = animation_manager_.RequireAnimationGroup(
        virtual_entity_.animation.prefix, animation_context_.current_state);

    std::size_t actual_frame_index = animation_group.from + virtual_entity_.animation.frame_id;

    // 确保帧索引在有效范围内
    if (actual_frame_index > animation_group.to) {
        actual_frame_index = animation_group.to;
    }

    // 获取帧数据和纹理
    const auto& [frame_data, texture] = animation_manager_.RequireFrameData(
        virtual_entity_.animation.prefix, actual_frame_index);

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

    // 计算最终渲染位置（参考AnimationPlayer的逻辑）
    sf::Vector2f final_position = virtual_entity_.position;
    final_position.x += static_cast<float>(frame_data.trim_left) * scale.x;
    final_position.y += static_cast<float>(frame_data.trim_top) * scale.y;

    // 加上图像中心的偏移（因为virtual_entity_.position是左上角坐标）
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
            if (IsMouseOver(mouse_world_pos) && IsClickable()) {
                DEBUG_CODE(std::cout << "FlagUI: Level " << level_id_ << " clicked!" << std::endl;)

                // 调用点击回调
                if (click_callback_) {
                    click_callback_(level_id_);
                }
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

        DEBUG_CODE(std::cout << "FlagUI: Level " << level_id_ << " status changed from "
                  << static_cast<int>(old_status) << " to " << static_cast<int>(status) << std::endl;)

        if (level_status_ == LevelStatus::Locked) {
            is_hovered_ = false;  // 锁定状态不显示悬停效果
        }
    }
}

/**
 * @brief 检查是否可点击
 */
bool FlagUI::IsClickable() const
{
    // 只要不是Locked状态就可以点击
    return level_status_ != LevelStatus::Locked;
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
 * @brief 初始化旗子动画
 */
void FlagUI::InitializeAnimation()
{
    // // 开始播放Idle动画（循环播放，模拟旗子飘扬）
    // animation_player_.PlayAnimation(virtual_entity_, animation_context_, State::Idle, true);

    // initialized_ = true;

    // DEBUG_CODE(std::cout << "FlagUI: Initialized animation for level " << level_id_
    //           << " with prefix " << virtual_entity_.animation.prefix
    //           << " (Idle state, looping)" << std::endl;)
}

/**
 * @brief 更新动画
 */
void FlagUI::UpdateAnimation()
{
    // if (!initialized_) {
    //     return;
    // }

    // // 简化：直接更新动画帧，无需状态切换
    // animation_player_.Update(virtual_entity_, animation_context_);
}

/**
 * @brief 获取旗子的点击区域
 */
sf::FloatRect FlagUI::GetClickBounds() const
{
    // 假设旗子的点击区域是一个64x64的正方形，以旗子位置为中心
    // const float flag_size = 64.0f;
    // return sf::FloatRect(
    //     position_.x - flag_size / 2.0f,
    //     position_.y - flag_size / 2.0f,
    //     flag_size,
    //     flag_size
    // );
}

/**
 * @brief 处理鼠标悬停逻辑
 */
void FlagUI::UpdateHoverState(const sf::Vector2f& mouse_pos)
{
    bool was_hovered = is_hovered_;

    // 只有可点击的旗子才响应悬停
    is_hovered_ = IsMouseOver(mouse_pos) && IsClickable();

    // 悬停状态改变时输出日志
    if (was_hovered != is_hovered_) {
        if (is_hovered_ && !was_hovered) {
            DEBUG_CODE(std::cout << "FlagUI: Level " << level_id_ << " hovered" << std::endl;)
        } else if (!is_hovered_ && was_hovered) {
            DEBUG_CODE(std::cout << "FlagUI: Level " << level_id_ << " unhovered" << std::endl;)
        }
    }
}