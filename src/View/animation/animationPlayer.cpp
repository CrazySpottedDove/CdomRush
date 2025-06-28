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
    std::cout << "AnimationPlayer initialized (frame-based)" << std::endl;
}

/**
 * @brief 开始播放指定状态的动画
 */
void AnimationPlayer::PlayAnimation(Animation& animation, State state, bool loop)
{
    // 检查是否需要重置动画状态
    bool should_reset = false;
    
    if (!animation.pending) {
        // 如果动画没有在播放，需要重置
        should_reset = true;
        std::cout << "Starting new animation for prefix: " << animation.prefix << std::endl;
    } else if (current_state_ != state) {
        // 如果切换到不同的状态，需要重置
        should_reset = true;
        std::cout << "Switching animation state from " << static_cast<int>(current_state_) 
                  << " to " << static_cast<int>(state) << std::endl;
    }
    
    if (should_reset) {
        animation.pending = true;
        animation.frame_id = 0;  // 重置到第一帧
        current_state_ = state;
        is_paused_ = false;
    }
    
    // 更新播放设置
    loop_enabled_ = loop;
    
    std::cout << "Animation settings: loop=" << loop << ", current_frame=" << animation.frame_id << std::endl;
}

/**
 * @brief 手动前进到下一帧
 */
bool AnimationPlayer::NextFrame(Animation& animation)
{
    // 如果动画没有在播放或者已暂停，不能前进
    if (!animation.pending || is_paused_) {
        return false;
    }
    
    try {
        // 获取动画总帧数
        std::size_t total_frames = GetAnimationFrameCount(animation.prefix, current_state_);
        
        if (total_frames == 0) {
            std::cerr << "Warning: Animation has 0 frames for prefix: " << animation.prefix << std::endl;
            animation.pending = false;
            return false;
        }
        
        // 前进到下一帧
        animation.frame_id++;
        
        // 处理动画结束逻辑
        if (animation.frame_id >= total_frames) {
            if (loop_enabled_) {
                // 循环播放：回到第一帧
                animation.frame_id = 0;
                std::cout << "Animation looped for prefix: " << animation.prefix << std::endl;
                return true;
            } else {
                // 非循环播放：停在最后一帧并停止
                animation.frame_id = total_frames - 1;
                animation.pending = false;
                std::cout << "Animation finished for prefix: " << animation.prefix << std::endl;
                return false;
            }
        }
        
        std::cout << "Advanced to frame " << animation.frame_id << " for prefix: " << animation.prefix << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error advancing animation frame: " << e.what() << std::endl;
        animation.pending = false;
        return false;
    }
}

/**
 * @brief 自动更新动画（这里可以添加自动前进的条件）
 */
void AnimationPlayer::Update(Animation& animation)
{
    // 基础版本：每次调用Update都自动前进一帧
    // 你可以根据需要修改这个逻辑，比如：
    // - 每隔几次调用才前进一帧
    // - 根据某些游戏条件决定是否前进
    // - 等等...
    
    NextFrame(animation);
}

/**
 * @brief 渲染当前帧到指定位置
 */
void AnimationPlayer::Render(sf::RenderWindow& window, const Animation& animation, 
                            const sf::Vector2f& position, const sf::Vector2f& scale)
{
    // 如果动画没有在播放，不渲染
    if (!animation.pending) {
        return;
    }
    
    try {
        // 计算实际的帧索引（需要加上动画组的起始偏移）
        std::size_t actual_frame_index = CalculateActualFrameIndex(animation, current_state_);
        
        // 从AnimationManager获取帧数据和纹理
        const auto& [frame_data, texture] = animation_manager_.RequireFrameData(animation.prefix, actual_frame_index);
        
        // 创建精灵并设置属性
        sf::Sprite sprite(texture);
        sprite.setTextureRect(frame_data.frameRect);
        sprite.setScale(scale);
        
        // 计算最终位置（考虑裁剪偏移）
        sf::Vector2f final_position = position;
        final_position.x += static_cast<float>(frame_data.trim_left) * scale.x;
        final_position.y += static_cast<float>(frame_data.trim_top) * scale.y;
        sprite.setPosition(final_position);
        
        // 渲染到窗口
        window.draw(sprite);
        
    } catch (const std::exception& e) {
        std::cerr << "Error rendering animation frame: " << e.what() << std::endl;
        // 渲染失败不需要停止动画，下一帧可能正常
    }
}

/**
 * @brief 停止当前动画
 */
void AnimationPlayer::StopAnimation(Animation& animation)
{
    animation.pending = false;
    animation.frame_id = 0;
    is_paused_ = false;
    
    std::cout << "Animation stopped for prefix: " << animation.prefix << std::endl;
}

/**
 * @brief 重置动画到第一帧
 */
void AnimationPlayer::ResetAnimation(Animation& animation)
{
    animation.frame_id = 0;
    is_paused_ = false;
    
    std::cout << "Animation reset to frame 0 for prefix: " << animation.prefix << std::endl;
}

/**
 * @brief 检查动画是否播放完成
 */
bool AnimationPlayer::IsAnimationFinished(const Animation& animation) const
{
    // 如果是循环动画或者还在播放，就没有完成
    if (loop_enabled_ || animation.pending) {
        return false;
    }
    
    try {
        std::size_t total_frames = GetAnimationFrameCount(animation.prefix, current_state_);
        return animation.frame_id >= total_frames - 1;
    } catch (const std::exception& e) {
        std::cerr << "Error checking animation finish status: " << e.what() << std::endl;
        return true; // 出错时认为动画已完成
    }
}

/**
 * @brief 暂停/恢复动画
 */
void AnimationPlayer::SetPaused(Animation& animation, bool paused)
{
    is_paused_ = paused;
    std::cout << "Animation " << (paused ? "paused" : "resumed") 
              << " for prefix: " << animation.prefix << std::endl;
}

/**
 * @brief 跳转到指定帧
 */
bool AnimationPlayer::JumpToFrame(Animation& animation, std::size_t frame_id)
{
    try {
        if (IsValidFrameId(animation.prefix, current_state_, frame_id)) {
            animation.frame_id = frame_id;
            std::cout << "Jumped to frame " << frame_id << " for prefix: " << animation.prefix << std::endl;
            return true;
        } else {
            std::cerr << "Invalid frame ID " << frame_id << " for prefix: " << animation.prefix << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error jumping to frame: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief 计算动画的实际帧索引（加上动画组的起始偏移）
 */
std::size_t AnimationPlayer::CalculateActualFrameIndex(const Animation& animation, State state) const
{
    try {
        // 需要AnimationManager提供GetAnimationGroup接口
        // 假设你的队友会添加这个方法到AnimationManager中
        const auto& animation_group = animation_manager_.GetAnimationGroup(animation.prefix, state);
        
        // 计算实际的帧索引：动画组起始位置 + 当前帧偏移
        std::size_t actual_index = animation_group.from + animation.frame_id;
        
        // 确保索引在有效范围内
        if (actual_index > animation_group.to) {
            std::cerr << "Warning: Frame index " << actual_index 
                      << " exceeds animation group range [" << animation_group.from 
                      << ", " << animation_group.to << "]" << std::endl;
            return animation_group.to; // 返回最后一帧
        }
        
        return actual_index;
    }
    catch (const std::exception& e) {
        std::cerr << "Error calculating frame index: " << e.what() << std::endl;
        return 0; // 出错时返回第一帧
    }
}

/**
 * @brief 获取指定状态的动画总帧数
 */
std::size_t AnimationPlayer::GetAnimationFrameCount(const std::string& prefix, State state) const
{
    try {
        // 需要AnimationManager提供GetAnimationGroup接口
        const auto& animation_group = animation_manager_.GetAnimationGroup(prefix, state);
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
