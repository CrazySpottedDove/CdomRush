#include "animationPlayer.h"
#include <iostream>
#include <SFML/Graphics/Sprite.hpp>

/**
 * @brief 构造函数 - 初始化动画播放器
 */
AnimationPlayer::AnimationPlayer(const AnimationManager& animation_manager)
    : animation_manager_(animation_manager)
    , current_state_(State::Idle)
    , loop_enabled_(true)
    , is_paused_(false)
{
    std::cout << "AnimationPlayer initialized (supports Entity + Unit with flip)" << std::endl;
}

// ===============================
// Entity接口实现 - 基础动画功能
// ===============================

/**
 * @brief 播放指定状态的动画（Entity版本 - 基础）
 */
void AnimationPlayer::PlayAnimation(Entity& entity, State state, bool loop)
{
    // 使用指定的state
    bool should_reset = false;
    
    if (!entity.animation.pending) {
        should_reset = true;
        std::cout << "Starting new animation for Entity prefix: " << entity.animation.prefix 
                  << " (state: " << static_cast<int>(state) << ")" << std::endl;
    } else if (current_state_ != state) {
        should_reset = true;
        std::cout << "Switching Entity animation from " << static_cast<int>(current_state_) 
                  << " to " << static_cast<int>(state) << std::endl;
    }
    
    if (should_reset) {
        entity.animation.pending = true;
        entity.animation.frame_id = 0;
        entity.animation.state = state; 
        current_state_ = state;
        is_paused_ = false;
    }
    
    loop_enabled_ = loop;
    std::cout << "Entity animation settings: loop=" << loop 
              << ", current_frame=" << entity.animation.frame_id << std::endl;
}

/**
 * @brief 手动前进到下一帧（Entity版本）
 */
bool AnimationPlayer::NextFrame(Entity& entity)
{
    if (!entity.animation.pending || is_paused_) {
        return false;
    }
    
    try {
        std::size_t total_frames = GetAnimationFrameCount(entity.animation.prefix, current_state_);
        
        if (total_frames == 0) {
            std::cerr << "Warning: Entity animation has 0 frames for prefix: " << entity.animation.prefix << std::endl;
            entity.animation.pending = false;
            return false;
        }
        
        entity.animation.frame_id++;
        
        if (entity.animation.frame_id >= total_frames) {
            if (loop_enabled_) {
                entity.animation.frame_id = 0;
                std::cout << "Entity animation looped for prefix: " << entity.animation.prefix << std::endl;
                return true;
            } else {
                entity.animation.frame_id = total_frames - 1;
                entity.animation.pending = false;
                std::cout << "Entity animation finished for prefix: " << entity.animation.prefix << std::endl;
                return false;
            }
        }
        
        std::cout << "Entity advanced to frame " << entity.animation.frame_id 
                  << " for prefix: " << entity.animation.prefix << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error advancing Entity animation frame: " << e.what() << std::endl;
        entity.animation.pending = false;
        return false;
    }
}

/**
 * @brief 自动更新动画（Entity版本）
 */
void AnimationPlayer::Update(Entity& entity)
{
    // Entity版本：简单的帧前进，不检测状态变化
    NextFrame(entity);
}

/**
 * @brief 渲染当前帧（Entity版本 - 不支持翻转）
 */
void AnimationPlayer::Render(sf::RenderWindow& window, const Entity& entity, 
                            const sf::Vector2f& scale)
{
    if (!entity.animation.pending) {
        return;
    }
    
    try {
        std::size_t actual_frame_index = CalculateActualFrameIndex(entity.animation, current_state_);
        
        const auto& [frame_data, texture] = animation_manager_.RequireFrameData(entity.animation.prefix, actual_frame_index);
        
        sf::Sprite sprite(texture);
        // 设置纹理裁剪区域（实际图像内容）
        sprite.setTextureRect(frame_data.frameRect);
        
        // 设置原点到图像中心
        sprite.setOrigin(
            static_cast<float>(frame_data.frameRect.width) / 2.0f,
            static_cast<float>(frame_data.frameRect.height) / 2.0f
        );
        
        sprite.setScale(scale);  
        
        // 计算最终渲染位置：entity位置 + trim偏移 + 图像中心偏移
        sf::Vector2f final_position = entity.position;
        final_position.x += static_cast<float>(frame_data.trim_left) * scale.x;
        final_position.y += static_cast<float>(frame_data.trim_top) * scale.y;
        
        // 加上图像中心的偏移
        final_position.x += static_cast<float>(frame_data.frameRect.width) / 2.0f * scale.x;
        final_position.y += static_cast<float>(frame_data.frameRect.height) / 2.0f * scale.y;
        sprite.setPosition(final_position);
        
        window.draw(sprite);
        
        // displaySize信息
        std::cout << "Entity render - displaySize: " << frame_data.displaySize.x << "x" << frame_data.displaySize.y
                  << ", frameRect: " << frame_data.frameRect.width << "x" << frame_data.frameRect.height
                  << ", trim: (" << frame_data.trim_left << "," << frame_data.trim_top << ")" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error rendering Entity animation frame: " << e.what() << std::endl;
    }
}

/**
 * @brief 停止当前动画（Entity版本）
 */
void AnimationPlayer::StopAnimation(Entity& entity)
{
    entity.animation.pending = false;
    entity.animation.frame_id = 0;
    is_paused_ = false;
    
    std::cout << "Entity animation stopped for prefix: " << entity.animation.prefix << std::endl;
}

/**
 * @brief 重置动画到第一帧（Entity版本）
 */
void AnimationPlayer::ResetAnimation(Entity& entity)
{
    entity.animation.frame_id = 0;
    is_paused_ = false;
    
    std::cout << "Entity animation reset to frame 0 for prefix: " << entity.animation.prefix << std::endl;
}

/**
 * @brief 检查动画是否播放完成（Entity版本）
 */
bool AnimationPlayer::IsAnimationFinished(const Entity& entity) const
{
    if (loop_enabled_ || entity.animation.pending) {
        return false;
    }
    
    try {
        std::size_t total_frames = GetAnimationFrameCount(entity.animation.prefix, current_state_);
        return entity.animation.frame_id >= total_frames - 1;
    } catch (const std::exception& e) {
        std::cerr << "Error checking Entity animation finish status: " << e.what() << std::endl;
        return true;
    }
}

/**
 * @brief 暂停/恢复动画（Entity版本）
 */
void AnimationPlayer::SetPaused(Entity& entity, bool paused)
{
    is_paused_ = paused;
    std::cout << "Entity animation " << (paused ? "paused" : "resumed") 
              << " for prefix: " << entity.animation.prefix << std::endl;
}

/**
 * @brief 跳转到指定帧（Entity版本）
 */
bool AnimationPlayer::JumpToFrame(Entity& entity, std::size_t frame_id)
{
    try {
        if (IsValidFrameId(entity.animation.prefix, current_state_, frame_id)) {
            entity.animation.frame_id = frame_id;
            std::cout << "Entity jumped to frame " << frame_id 
                      << " for prefix: " << entity.animation.prefix << std::endl;
            return true;
        } else {
            std::cerr << "Invalid frame ID " << frame_id 
                      << " for Entity prefix: " << entity.animation.prefix << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error jumping Entity to frame: " << e.what() << std::endl;
        return false;
    }
}

// ===============================
// Unit接口实现 - 完整功能（翻转）
// ===============================

/**
 * @brief 开始播放动画（Unit版本）
 */
void AnimationPlayer::PlayAnimation(Unit& unit, bool loop)
{
    State target_animation_state = unit.state;
    
    bool should_reset = false;
    
    if (!unit.animation.pending) {
        should_reset = true;
        std::cout << "Starting new animation for Unit prefix: " << unit.animation.prefix 
                  << " (Unit state: " << static_cast<int>(unit.state) 
                  << ", Heading: " << static_cast<int>(unit.heading) << ")" << std::endl;
    } else if (current_state_ != target_animation_state) {
        should_reset = true;
        std::cout << "Switching Unit animation from " << static_cast<int>(current_state_) 
                  << " to " << static_cast<int>(target_animation_state) 
                  << " (Unit state: " << static_cast<int>(unit.state) << ")" << std::endl;
    }
    
    if (should_reset) {
        unit.animation.pending = true;
        unit.animation.frame_id = 0;
        unit.animation.state = target_animation_state;  // 使用映射后的状态
        current_state_ = target_animation_state;
        is_paused_ = false;
    }
    
    loop_enabled_ = loop;
}

/**
 * @brief 手动前进到下一帧（Unit版本）
 */
bool AnimationPlayer::NextFrame(Unit& unit)
{
    if (!unit.animation.pending || is_paused_) {
        return false;
    }
    
    try {
        std::size_t total_frames = GetAnimationFrameCount(unit.animation.prefix, current_state_);
        
        if (total_frames == 0) {
            std::cerr << "Warning: Unit animation has 0 frames for prefix: " << unit.animation.prefix << std::endl;
            unit.animation.pending = false;
            return false;
        }
        
        unit.animation.frame_id++;
        
        if (unit.animation.frame_id >= total_frames) {
            if (loop_enabled_) {
                unit.animation.frame_id = 0;
                std::cout << "Unit animation looped for prefix: " << unit.animation.prefix << std::endl;
                return true;
            } else {
                unit.animation.frame_id = total_frames - 1;
                unit.animation.pending = false;
                std::cout << "Unit animation finished for prefix: " << unit.animation.prefix << std::endl;
                return false;
            }
        }
        
        std::cout << "Unit advanced to frame " << unit.animation.frame_id 
                  << " for prefix: " << unit.animation.prefix << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error advancing Unit animation frame: " << e.what() << std::endl;
        unit.animation.pending = false;
        return false;
    }
}

/**
 * @brief 自动更新动画（Unit版本
 */
void AnimationPlayer::Update(Unit& unit)
{
    // Unit版本：检查状态变化并自动切换动画
    State target_animation_state = unit.state;
    if (current_state_ != target_animation_state) {
        PlayAnimation(unit, loop_enabled_);  
        return;
    }
    
    NextFrame(unit);
}

/**
 * @brief 渲染当前帧（Unit版本）
 */
void AnimationPlayer::Render(sf::RenderWindow& window, const Unit& unit, 
                            const sf::Vector2f& scale)
{
    if (!unit.animation.pending) {
        return;
    }
    
    try {
        std::size_t actual_frame_index = CalculateActualFrameIndex(unit.animation, current_state_);
        
        const auto& [frame_data, texture] = animation_manager_.RequireFrameData(unit.animation.prefix, actual_frame_index);
        
        sf::Sprite sprite(texture);
        // 设置纹理裁剪区域（实际图像内容）
        sprite.setTextureRect(frame_data.frameRect);
        
        // 设置原点到图像中心
        sprite.setOrigin(
            static_cast<float>(frame_data.frameRect.width) / 2.0f,
            static_cast<float>(frame_data.frameRect.height) / 2.0f
        );
        
        bool needs_flip = ShouldFlipHorizontally(unit.state, unit.heading);
        sf::Vector2f final_scale = scale;
        if (needs_flip) {
            final_scale.x = -scale.x;  // 水平翻转以中心为支点
            std::cout << "Applying horizontal flip for Unit state: " 
                      << static_cast<int>(unit.state) 
                      << ", heading: " << static_cast<int>(unit.heading) << std::endl;
        }
        sprite.setScale(final_scale);
        
        // 计算最终渲染位置
        sf::Vector2f final_position = unit.position;
        
        // 期望位置 = unit.position + trim偏移 + 图像中心偏移
        final_position.x += static_cast<float>(frame_data.trim_left) * scale.x;
        final_position.y += static_cast<float>(frame_data.trim_top) * scale.y;
        
        // 加上图像中心的偏移
        final_position.x += static_cast<float>(frame_data.frameRect.width) / 2.0f * scale.x;
        final_position.y += static_cast<float>(frame_data.frameRect.height) / 2.0f * scale.y;
        
        sprite.setPosition(final_position);
        window.draw(sprite);
        
        // displaySize和翻转
        std::cout << "Unit render - displaySize: " << frame_data.displaySize.x << "x" << frame_data.displaySize.y
                  << ", frameRect: " << frame_data.frameRect.width << "x" << frame_data.frameRect.height
                  << ", trim: (" << frame_data.trim_left << "," << frame_data.trim_top 
                  << "," << frame_data.trim_right << "," << frame_data.trim_bottom << ")"
                  << ", flipped: " << needs_flip << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error rendering Unit animation frame: " << e.what() << std::endl;
    }
}

/**
 * @brief 停止当前动画（Unit版本）
 */
void AnimationPlayer::StopAnimation(Unit& unit)
{
    unit.animation.pending = false;
    unit.animation.frame_id = 0;
    is_paused_ = false;
    
    std::cout << "Unit animation stopped for prefix: " << unit.animation.prefix << std::endl;
}

/**
 * @brief 重置动画到第一帧（Unit版本）
 */
void AnimationPlayer::ResetAnimation(Unit& unit)
{
    unit.animation.frame_id = 0;
    is_paused_ = false;
    
    std::cout << "Unit animation reset to frame 0 for prefix: " << unit.animation.prefix << std::endl;
}

/**
 * @brief 检查动画是否播放完成（Unit版本）
 */
bool AnimationPlayer::IsAnimationFinished(const Unit& unit) const
{
    if (loop_enabled_ || unit.animation.pending) {
        return false;
    }
    
    try {
        std::size_t total_frames = GetAnimationFrameCount(unit.animation.prefix, current_state_);
        return unit.animation.frame_id >= total_frames - 1;
    } catch (const std::exception& e) {
        std::cerr << "Error checking Unit animation finish status: " << e.what() << std::endl;
        return true;
    }
}

/**
 * @brief 暂停/恢复动画（Unit版本）
 */
void AnimationPlayer::SetPaused(Unit& unit, bool paused)
{
    is_paused_ = paused;
    std::cout << "Unit animation " << (paused ? "paused" : "resumed") 
              << " for prefix: " << unit.animation.prefix << std::endl;
}

/**
 * @brief 跳转到指定帧（Unit版本）
 */
bool AnimationPlayer::JumpToFrame(Unit& unit, std::size_t frame_id)
{
    try {
        if (IsValidFrameId(unit.animation.prefix, current_state_, frame_id)) {
            unit.animation.frame_id = frame_id;
            std::cout << "Unit jumped to frame " << frame_id 
                      << " for prefix: " << unit.animation.prefix << std::endl;
            return true;
        } else {
            std::cerr << "Invalid frame ID " << frame_id 
                      << " for Unit prefix: " << unit.animation.prefix << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error jumping Unit to frame: " << e.what() << std::endl;
        return false;
    }
}

// ===============================
// Unit private方法
// ===============================

/**
 * @brief 检查是否需要水平翻转（仅Unit）
 */
bool AnimationPlayer::ShouldFlipHorizontally(State unit_state, Heading heading) const
{
    if (unit_state == State::WalkingLeftRight) {
        return heading == Heading::Left;  // 向左走时需要翻转
    }
    
    // 此处待添加更多翻转逻辑
    if (unit_state == State::Running && heading == Heading::Left) 
        return true;
    
    return false;
}

// ===============================
// 通用方法
// ===============================

/**
 * @brief 计算动画的实际帧索引（加上动画组的起始偏移）
 */
std::size_t AnimationPlayer::CalculateActualFrameIndex(const Animation& animation, State state) const
{
    try {
        const auto& animation_group = animation_manager_.RequireAnimationGroup(animation.prefix, state);
        
        std::size_t actual_index = animation_group.from + animation.frame_id;
        
        if (actual_index > animation_group.to) {
            std::cerr << "Warning: Frame index " << actual_index 
                      << " exceeds animation group range [" << animation_group.from 
                      << ", " << animation_group.to << "]" << std::endl;
            return animation_group.to;
        }
        
        return actual_index;
    }
    catch (const std::exception& e) {
        std::cerr << "Error calculating frame index: " << e.what() << std::endl;
        return 0;
    }
}

/**
 * @brief 获取指定状态的动画总帧数
 */
std::size_t AnimationPlayer::GetAnimationFrameCount(const std::string& prefix, State state) const
{
    try {
        const auto& animation_group = animation_manager_.RequireAnimationGroup(prefix, state);
        return animation_group.to - animation_group.from + 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error getting animation frame count: " << e.what() << std::endl;
        return 0;
    }
}

/**
 * @brief 检查指定的帧ID是否有效
 */
bool AnimationPlayer::IsValidFrameId(const std::string& prefix, State state, std::size_t frame_id) const
{
    std::size_t total_frames = GetAnimationFrameCount(prefix, state);
    return frame_id < total_frames;
}
