#include "animationPlayer.h"
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics/Sprite.hpp>
#include "Model/components/animation.h"
#include "utils/macros.h"

/**
 * @brief 构造函数 - 初始化动画播放器
 */
AnimationPlayer::AnimationPlayer(const AnimationManager& animation_manager)
    : animation_manager_(animation_manager)
{
    DEBUG_CODE(std::cout << "AnimationPlayer initialized (stateless design with AnimationContext)" << std::endl;)
}

/**
 * @brief 统一的渲染接口
 */
void AnimationPlayer::RenderAnimation(sf::RenderWindow& window, const sf::Vector2f& position,
                                      Animation& animation, const AnimationContext& context, const sf::Vector2f& scale)
{
    if(!animation.pending) {
        return; // 如果动画未进行，直接返回
    }

    try {
    std::size_t actual_frame_index = CalculateActualFrameIndex(animation, animation.state);

    const auto& [frame_data, texture] = animation_manager_.RequireFrameData(animation.prefix, actual_frame_index);

    sf::Sprite sprite(texture);
    // 设置纹理裁剪区域（实际图像内容）
    sprite.setTextureRect(frame_data.frameRect);
    // 设置原点到图像中心
    sprite.setOrigin(sf::Vector2f(
        static_cast<float>(frame_data.displaySize.x) * animation.anchor_x - frame_data.trim_left,
        static_cast<float>(frame_data.displaySize.y) * (1 - animation.anchor_y) -
            frame_data.trim_top));

    //处理翻转（基于flip）
    sf::Vector2f final_scale = scale;
    if(animation.flip)
    {
        final_scale.x = -scale.x; // 翻转X轴
        DEBUG_CODE(std::cout << "Animation flipped for prefix: " << animation.prefix << std::endl;)
    }
    sprite.setScale(final_scale);

    //计算渲染位置
    // sf::Vector2f final_position = position;
    // final_position.x += static_cast<float>(frame_data.trim_left) * scale.x;
    // final_position.y += static_cast<float>(frame_data.trim_top) * scale.y;

    // 加上图像中心的偏移(因为原点设为了图像中心)
    // final_position.x += static_cast<float>(frame_data.frameRect.size.x) / 2.0f * scale.x;
    // final_position.y += static_cast<float>(frame_data.frameRect.size.y) / 2.0f * scale.y;

    // final_position.x -= 43.0f;
    // final_position.y -= 60.0f; // 调整位置偏移
    sprite.setPosition(position);

    window.draw(sprite);
    DEBUG_CODE(std::cout << "RenderAnimation - prefix: " << animation.prefix
                  << ", state: " << static_cast<int>(animation.state)
                  << ", frame: " << animation.frame_id
                  << ", flip: " << animation.flip << std::endl;)

    } catch (const std::exception& e) {
        std::cerr << "Error rendering animation: " << e.what() << std::endl;
    }
}

/**
 * @brief 统一的更新接口
 */
void AnimationPlayer::UpdateAnimation(Animation& animation, AnimationContext& context)
{
    //状态变化
    if(context.needs_state_change(animation)) {
        DEBUG_CODE(std::cout << "Updating animation state from " << static_cast<int>(animation.last_state)
                  << " to " << static_cast<int>(animation.state) << std::endl;)
        PlayAnimationState(animation, context,  context.loop_enabled);
        return;
    }

    //状态未变化，下一帧
    if(!animation.pending || context.is_paused) {
        return;
    }

    try{
        std::size_t total_frames = GetAnimationFrameCount(animation.prefix, animation.state);

        if (total_frames == 0) {
            std::cerr << "Warning: Animation has 0 frames for prefix: " << animation.prefix << std::endl;
            animation.pending = false;
            return;
        }

        animation.frame_id++;

        if (animation.frame_id >= total_frames) {
            if (context.loop_enabled) {
                animation.frame_id = 0; // 循环播放
                DEBUG_CODE(std::cout << "Animation looped for prefix: " << animation.prefix << std::endl;)
            } else {
                animation.frame_id = total_frames - 1; // 停止在最后一帧
                animation.pending = false; // 动画结束
                DEBUG_CODE(std::cout << "Animation finished for prefix: " << animation.prefix << std::endl;)
            }
        }

        DEBUG_CODE(std::cout << "Advanced to frame " << animation.frame_id
                      << " for prefix: " << animation.prefix << std::endl;)

    } catch (const std::exception& e) {
        std::cerr << "Error advancing animation frame: " << e.what() << std::endl;
        animation.pending = false; // 出错时停止动画
    }
}

/**
 * @brief 统一的播放接口
 */
void AnimationPlayer::PlayAnimationState(Animation& animation, AnimationContext& context,
                                         bool loop)
{
    bool should_reset = false;

    if(!animation.pending){
        should_reset = true;
        DEBUG_CODE(std::cout << "Starting new animation for prefix: " << animation.prefix
                  << " (state: " << static_cast<int>(animation.state) << ")" << std::endl;)
    } else if(context.needs_state_change(animation)) {
        should_reset = true;
        DEBUG_CODE(std::cout << "Switching animation from " << static_cast<int>(animation.last_state)
                  << " to " << static_cast<int>(animation.state) << std::endl;)
    }

    if(should_reset){
        animation.pending = true;
        animation.frame_id = 0;
        animation.last_state = animation.state;
        //animation.state = state;
        context.is_paused = false; // 重置为非暂停状态
    }
}

/**
 * @brief 渲染地图背景
 */
void AnimationPlayer::RenderMap(sf::RenderWindow& window,
                               const std::string& map_prefix,
                               const sf::Vector2f& position,
                               const sf::Vector2f& scale) const
{
    try {
        // 地图被当作静态动画：状态=Idle，获取动画组信息
        const auto& animation_group = animation_manager_.RequireAnimationGroup(map_prefix, State::Idle);

        // 使用动画组的起始帧（对于地图来说，from和to都是1）
        std::size_t actual_frame_index = animation_group.from;

        const auto& [frame_data, texture] = animation_manager_.RequireFrameData(map_prefix, actual_frame_index);

        sf::Sprite map_sprite(texture);
        // 设置纹理裁剪区域
        map_sprite.setTextureRect(frame_data.frameRect);

        // 设置位置和缩放
        map_sprite.setPosition(position);
        map_sprite.setScale(scale);

        // 渲染地图
        window.draw(map_sprite);

        // 调试信息（每60帧输出一次）
        DEBUG_CODE(
            static int debug_counter = 0;
            debug_counter++;
            if (debug_counter % 60 == 0) {
                std::cout << "Map rendered: " << map_prefix
                          << " at (" << position.x << "," << position.y << ")"
                          << " scale (" << scale.x << "," << scale.y << ")"
                          << " frame_index (" << actual_frame_index << ")"
                          << " size (" << frame_data.frameRect.size.x << "x" << frame_data.frameRect.size.y << ")" << std::endl;
            }
        )

    } catch (const std::exception& e) {
        std::cerr << "Error rendering map '" << map_prefix << "': " << e.what() << std::endl;
        std::cerr << "请确保地图资源已通过AnimationManager加载,并且存在idle动画组" << std::endl;
    }
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




// ===============================
// Entity接口实现 - 基础动画功能
// ===============================

// /**
//  * @brief 播放指定状态的动画（Entity版本 - 基础）
//  */
// void AnimationPlayer::PlayAnimation(Entity& entity, AnimationContext& context, State state, bool loop)
// {
//     // 使用指定的state
//     bool should_reset = false;

//     if (!entity.animation.pending) {
//         should_reset = true;
//         DEBUG_CODE(std::cout << "Starting new animation for Entity prefix: " << entity.animation.prefix
//                   << " (state: " << static_cast<int>(state) << ")" << std::endl;)
//     } else if (context.needs_state_change(state)) {
//         should_reset = true;
//         DEBUG_CODE(std::cout << "Switching Entity animation from " << static_cast<int>(context.current_state)
//                   << " to " << static_cast<int>(state) << std::endl;)
//     }

//     if (should_reset) {
//         entity.animation.pending = true;
//         entity.animation.frame_id = 0;
//         entity.animation.state = state;
//         context.update_state(state);
//         context.is_paused = false;
//     }

//     context.loop_enabled = loop;
//     DEBUG_CODE(std::cout << "Entity animation settings: loop=" << loop
//               << ", current_frame=" << entity.animation.frame_id << std::endl;)
// }

// /**
//  * @brief 手动前进到下一帧（Entity版本）
//  */
// bool AnimationPlayer::NextFrame(Entity& entity, AnimationContext& context)
// {
//     if (!entity.animation.pending || context.is_paused) {
//         return false;
//     }

//     try {
//         std::size_t total_frames = GetAnimationFrameCount(entity.animation.prefix, context.current_state);

//         if (total_frames == 0) {
//             std::cerr << "Warning: Entity animation has 0 frames for prefix: " << entity.animation.prefix << std::endl;
//             entity.animation.pending = false;
//             return false;
//         }

//         entity.animation.frame_id++;

//         if (entity.animation.frame_id >= total_frames) {
//             if (context.loop_enabled) {
//                 entity.animation.frame_id = 0;
//                 DEBUG_CODE(std::cout << "Entity animation looped for prefix: " << entity.animation.prefix << std::endl;)
//                 return true;
//             } else {
//                 entity.animation.frame_id = total_frames - 1;
//                 entity.animation.pending = false;
//                 DEBUG_CODE(std::cout << "Entity animation finished for prefix: " << entity.animation.prefix << std::endl;)
//                 return false;
//             }
//         }

//         DEBUG_CODE(std::cout << "Entity advanced to frame " << entity.animation.frame_id
//                   << " for prefix: " << entity.animation.prefix << std::endl;)
//         return true;

//     } catch (const std::exception& e) {
//         std::cerr << "Error advancing Entity animation frame: " << e.what() << std::endl;
//         entity.animation.pending = false;
//         return false;
//     }
// }

// /**
//  * @brief 自动更新动画（Entity版本）
//  */
// void AnimationPlayer::Update(Entity& entity, AnimationContext& context)
// {
//     // Entity版本：简单的帧前进，不检测状态变化
//     NextFrame(entity, context);
// }

// /**
//  * @brief 渲染当前帧（Entity版本 - 不支持翻转）
//  */
// void AnimationPlayer::Render(sf::RenderWindow& window, const Entity& entity, const AnimationContext& context,
//                             const sf::Vector2f& scale)
// {
//     if (!entity.animation.pending) {
//         return;
//     }

//     try {
//         std::size_t actual_frame_index = CalculateActualFrameIndex(entity.animation, context.current_state);

//         const auto& [frame_data, texture] = animation_manager_.RequireFrameData(entity.animation.prefix, actual_frame_index);

//         sf::Sprite sprite(texture);
//         // 设置纹理裁剪区域（实际图像内容）
//         sprite.setTextureRect(frame_data.frameRect);

//         // 设置原点到图像中心
//         sprite.setOrigin(sf::Vector2f(static_cast<float>(frame_data.frameRect.size.x) / 2.0f,
//                                       static_cast<float>(frame_data.frameRect.size.y) / 2.0f)
//         );

//         sprite.setScale(scale);

//         // 计算最终渲染位置：entity位置 + trim偏移 + 图像中心偏移
//         sf::Vector2f final_position = entity.position;
//         final_position.x += static_cast<float>(frame_data.trim_left) * scale.x;
//         final_position.y += static_cast<float>(frame_data.trim_top) * scale.y;

//         // 加上图像中心的偏移
//         final_position.x += static_cast<float>(frame_data.frameRect.size.x) / 2.0f * scale.x;
//         final_position.y += static_cast<float>(frame_data.frameRect.size.y) / 2.0f * scale.y;
//         sprite.setPosition(final_position);

//         window.draw(sprite);

//         // displaySize信息
//         DEBUG_CODE(std::cout << "Entity render - displaySize: " << frame_data.displaySize.x << "x" << frame_data.displaySize.y
//                   << ", frameRect: " << frame_data.frameRect.size.x << "x" << frame_data.frameRect.size.y
//                   << ", trim: (" << frame_data.trim_left << "," << frame_data.trim_top << ")" << std::endl;)

//     } catch (const std::exception& e) {
//         std::cerr << "Error rendering Entity animation frame: " << e.what() << std::endl;
//     }
// }

// /**
//  * @brief 停止当前动画（Entity版本）
//  */
// void AnimationPlayer::StopAnimation(Entity& entity, AnimationContext& context)
// {
//     entity.animation.pending = false;
//     entity.animation.frame_id = 0;
//     context.is_paused = false;

//     DEBUG_CODE(std::cout << "Entity animation stopped for prefix: " << entity.animation.prefix << std::endl;)
// }

// /**
//  * @brief 重置动画到第一帧（Entity版本）
//  */
// void AnimationPlayer::ResetAnimation(Entity& entity, AnimationContext& context)
// {
//     entity.animation.frame_id = 0;
//     context.is_paused = false;

//     DEBUG_CODE(std::cout << "Entity animation reset to frame 0 for prefix: " << entity.animation.prefix << std::endl;)
// }

/**
 * @brief 检查动画是否播放完成（Entity版本）
 */
bool AnimationPlayer::IsAnimationFinished(Animation& animation,
                                          const AnimationContext& context) const
{
    if (context.loop_enabled || animation.pending) {
        return false;
    }

    try {
        std::size_t total_frames =
            GetAnimationFrameCount(animation.prefix, animation.state);
        return animation.frame_id >= total_frames - 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error checking Entity animation finish status: " << e.what() << std::endl;
        return true;
    }
}

// /**
//  * @brief 检查动画是否播放完成（Entity版本）
//  */
// bool AnimationPlayer::IsAnimationFinished(const Entity& entity, const AnimationContext& context) const
// {
//     if (context.loop_enabled || entity.animation.pending) {
//         return false;
//     }

//     try {
//         std::size_t total_frames = GetAnimationFrameCount(entity.animation.prefix, context.current_state);
//         return entity.animation.frame_id >= total_frames - 1;
//     } catch (const std::exception& e) {
//         std::cerr << "Error checking Entity animation finish status: " << e.what() << std::endl;
//         return true;
//     }
// }

// /**
//  * @brief 暂停/恢复动画（Entity版本）
//  */
// void AnimationPlayer::SetPaused(Entity& entity, AnimationContext& context, bool paused)
// {
//     context.is_paused = paused;
//     DEBUG_CODE(std::cout << "Entity animation " << (paused ? "paused" : "resumed")
//               << " for prefix: " << entity.animation.prefix << std::endl;)
// }

// /**
//  * @brief 跳转到指定帧（Entity版本）
//  */
// bool AnimationPlayer::JumpToFrame(Entity& entity, AnimationContext& context, std::size_t frame_id)
// {
//     try {
//         if (IsValidFrameId(entity.animation.prefix, context.current_state, frame_id)) {
//                     entity.animation.frame_id = frame_id;
//         DEBUG_CODE(std::cout << "Entity jumped to frame " << frame_id
//                   << " for prefix: " << entity.animation.prefix << std::endl;)
//             return true;
//         } else {
//             std::cerr << "Invalid frame ID " << frame_id
//                       << " for Entity prefix: " << entity.animation.prefix << std::endl;
//             return false;
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Error jumping Entity to frame: " << e.what() << std::endl;
//         return false;
//     }
// }

// // ===============================
// // Unit接口实现 - 完整功能（翻转）
// // ===============================

// /**
//  * @brief 开始播放动画（Unit版本）
//  */
// void AnimationPlayer::PlayAnimation(Unit& unit, AnimationContext& context, bool loop)
// {
//     State target_animation_state = unit.animation.state;

//     bool should_reset = false;

//     if (!unit.animation.pending) {
//         should_reset = true;
//         DEBUG_CODE(std::cout << "Starting new animation for Unit prefix: " << unit.animation.prefix
//                   << " (Unit state: " << static_cast<int>(unit.animation.state)
//                   << ", Heading: " << static_cast<int>(unit.heading) << ")" << std::endl;)
//     } else if (context.needs_state_change(target_animation_state)) {
//         should_reset = true;
//         DEBUG_CODE(std::cout << "Switching Unit animation from " << static_cast<int>(context.current_state)
//                   << " to " << static_cast<int>(target_animation_state)
//                   << " (Unit state: " << static_cast<int>(unit.animation.state) << ")" << std::endl;)
//     }

//     if (should_reset) {
//         unit.animation.pending = true;
//         unit.animation.frame_id = 0;
//         unit.animation.state = target_animation_state;  // 使用映射后的状态
//         context.update_state(target_animation_state);
//         context.is_paused = false;
//     }

//     context.loop_enabled = loop;
// }

// /**
//  * @brief 手动前进到下一帧（Unit版本）
//  */
// bool AnimationPlayer::NextFrame(Unit& unit, AnimationContext& context)
// {
//     if (!unit.animation.pending || context.is_paused) {
//         return false;
//     }

//     try {
//         std::size_t total_frames = GetAnimationFrameCount(unit.animation.prefix, context.current_state);

//         if (total_frames == 0) {
//             std::cerr << "Warning: Unit animation has 0 frames for prefix: " << unit.animation.prefix << std::endl;
//             unit.animation.pending = false;
//             return false;
//         }

//         unit.animation.frame_id++;

//         if (unit.animation.frame_id >= total_frames) {
//             if (context.loop_enabled) {
//                 unit.animation.frame_id = 0;
//                 DEBUG_CODE(std::cout << "Unit animation looped for prefix: " << unit.animation.prefix << std::endl;)
//                 return true;
//             } else {
//                 unit.animation.frame_id = total_frames - 1;
//                 unit.animation.pending = false;
//                 DEBUG_CODE(std::cout << "Unit animation finished for prefix: " << unit.animation.prefix << std::endl;)
//                 return false;
//             }
//         }

//         DEBUG_CODE(std::cout << "Unit advanced to frame " << unit.animation.frame_id
//                   << " for prefix: " << unit.animation.prefix << std::endl;)
//         return true;

//     } catch (const std::exception& e) {
//         std::cerr << "Error advancing Unit animation frame: " << e.what() << std::endl;
//         unit.animation.pending = false;
//         return false;
//     }
// }

// /**
//  * @brief 自动更新动画（Unit版本）
//  */
// void AnimationPlayer::Update(Unit& unit, AnimationContext& context)
// {
//     // Unit版本：检查状态变化并自动切换动画
//     State target_animation_state = unit.animation.state;
//     if (context.needs_state_change(target_animation_state)) {
//         PlayAnimation(unit, context, context.loop_enabled);
//         return;
//     }

//     NextFrame(unit, context);
// }

// /**
//  * @brief 渲染当前帧（Unit版本）
//  */
// void AnimationPlayer::Render(sf::RenderWindow& window, const Unit& unit, const AnimationContext& context,
//                             const sf::Vector2f& scale)
// {
//     if (!unit.animation.pending) {
//         return;
//     }

//     try {
//         std::size_t actual_frame_index = CalculateActualFrameIndex(unit.animation, context.current_state);

//         const auto& [frame_data, texture] = animation_manager_.RequireFrameData(unit.animation.prefix, actual_frame_index);

//         sf::Sprite sprite(texture);
//         // 设置纹理裁剪区域（实际图像内容）
//         sprite.setTextureRect(frame_data.frameRect);

//         // 设置原点到图像中心
//         sprite.setOrigin(sf::Vector2f(static_cast<float>(frame_data.frameRect.size.x) / 2.0f,
//                                       static_cast<float>(frame_data.frameRect.size.y) / 2.0f)
//         );

//         bool needs_flip = ShouldFlipHorizontally(unit.animation.state, unit.heading);
//         sf::Vector2f final_scale = scale;
//         if (needs_flip) {
//             final_scale.x = -scale.x;  // 水平翻转以中心为支点
//             DEBUG_CODE(std::cout << "Applying horizontal flip for Unit state: "
//                       << static_cast<int>(unit.animation.state)
//                       << ", heading: " << static_cast<int>(unit.heading) << std::endl;)
//         }
//         sprite.setScale(final_scale);

//         // 计算最终渲染位置
//         sf::Vector2f final_position = unit.position;

//         // 期望位置 = unit.position + trim偏移 + 图像中心偏移
//         final_position.x += static_cast<float>(frame_data.trim_left) * scale.x;
//         final_position.y += static_cast<float>(frame_data.trim_top) * scale.y;

//         // 加上图像中心的偏移
//         final_position.x += static_cast<float>(frame_data.frameRect.size.x) / 2.0f * scale.x;
//         final_position.y += static_cast<float>(frame_data.frameRect.size.y) / 2.0f * scale.y;

//         sprite.setPosition(final_position);
//         window.draw(sprite);

//         // displaySize和翻转
//         DEBUG_CODE(std::cout << "Unit render - displaySize: " << frame_data.displaySize.x << "x" << frame_data.displaySize.y
//                   << ", frameRect: " << frame_data.frameRect.size.x << "x" << frame_data.frameRect.size.y
//                   << ", trim: (" << frame_data.trim_left << "," << frame_data.trim_top
//                   << "," << frame_data.trim_right << "," << frame_data.trim_bottom << ")"
//                   << ", flipped: " << needs_flip << std::endl;)

//     } catch (const std::exception& e) {
//         std::cerr << "Error rendering Unit animation frame: " << e.what() << std::endl;
//     }
// }

// /**
//  * @brief 停止当前动画（Unit版本）
//  */
// void AnimationPlayer::StopAnimation(Unit& unit, AnimationContext& context)
// {
//     unit.animation.pending = false;
//     unit.animation.frame_id = 0;
//     context.is_paused = false;

//     DEBUG_CODE(std::cout << "Unit animation stopped for prefix: " << unit.animation.prefix << std::endl;)
// }

// /**
//  * @brief 重置动画到第一帧（Unit版本）
//  */
// void AnimationPlayer::ResetAnimation(Unit& unit, AnimationContext& context)
// {
//     unit.animation.frame_id = 0;
//     context.is_paused = false;

//     DEBUG_CODE(std::cout << "Unit animation reset to frame 0 for prefix: " << unit.animation.prefix << std::endl;)
// }

// /**
//  * @brief 检查动画是否播放完成（Unit版本）
//  */
// bool AnimationPlayer::IsAnimationFinished(const Unit& unit, const AnimationContext& context) const
// {
//     if (context.loop_enabled || unit.animation.pending) {
//         return false;
//     }

//     try {
//         std::size_t total_frames = GetAnimationFrameCount(unit.animation.prefix, context.current_state);
//         return unit.animation.frame_id >= total_frames - 1;
//     } catch (const std::exception& e) {
//         std::cerr << "Error checking Unit animation finish status: " << e.what() << std::endl;
//         return true;
//     }
// }

// /**
//  * @brief 暂停/恢复动画（Unit版本）
//  */
// void AnimationPlayer::SetPaused(Unit& unit, AnimationContext& context, bool paused)
// {
//     context.is_paused = paused;
//     DEBUG_CODE(std::cout << "Unit animation " << (paused ? "paused" : "resumed")
//               << " for prefix: " << unit.animation.prefix << std::endl;)
// }

// /**
//  * @brief 跳转到指定帧（Unit版本）
//  */
// bool AnimationPlayer::JumpToFrame(Unit& unit, AnimationContext& context, std::size_t frame_id)
// {
//     try {
//         if (IsValidFrameId(unit.animation.prefix, context.current_state, frame_id)) {
//             unit.animation.frame_id = frame_id;
//             DEBUG_CODE(std::cout << "Unit jumped to frame " << frame_id
//                       << " for prefix: " << unit.animation.prefix << std::endl;)
//             return true;
//         } else {
//             std::cerr << "Invalid frame ID " << frame_id
//                       << " for Unit prefix: " << unit.animation.prefix << std::endl;
//             return false;
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Error jumping Unit to frame: " << e.what() << std::endl;
//         return false;
//     }
// }

// ===============================
// Unit private方法
// ===============================

// /**
//  * @brief 检查是否需要水平翻转（仅Unit）
//  */
// bool AnimationPlayer::ShouldFlipHorizontally(State unit_state, Heading heading) const
// {
//     if (unit_state == State::WalkingLeftRight) {
//         return heading == Heading::Left;  // 向左走时需要翻转
//     }

//     // 此处待添加更多翻转逻辑
//     if (unit_state == State::Running && heading == Heading::Left)
//         return true;

//     return false;
// }

