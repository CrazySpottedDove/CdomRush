#include "EnemyUI.h"
#include <iostream>
#include "utils/macros.h"

/**
 * @brief 构造函数
 */
EnemyUI::EnemyUI(Enemy* enemy, AnimationPlayer& animation_player)
    : enemy_(enemy)
    , animation_player_(animation_player)
    , animation_context_()      // 初始化独立的动画上下文
    //, last_state_(State::Idle)
    //, last_heading_(Heading::Right)
    //, initialized_(false)
{
    if (enemy_ == nullptr) {
        std::cerr << "Warning: EnemyUI created with null enemy pointer" << std::endl;
        return;
    }
    
    DEBUG_CODE(std::cout << "EnemyUI created for enemy at (" << enemy_->position.x << "," << enemy_->position.y 
              << ") with prefix: " << enemy_->animation.prefix 
              << ", heading: " << static_cast<int>(enemy_->heading) << std::endl;)
    
}

/**
 * @brief 渲染Enemy
 */
void EnemyUI::Render(sf::RenderWindow& window,const Position& position,Animation& animation, const sf::Vector2f& scale)
{
    if (enemy_ == nullptr) {
        return;
    }

    // 如果Enemy已死亡且动画播放完成，不再渲染
    if (animation.state == State::Death && animation_player_.IsAnimationFinished(animation, animation_context_)) {
        return;
    }

    // 更新动画状态
    animation_player_.UpdateAnimation(animation,animation_context_);

    // 渲染Enemy
    animation_player_.RenderAnimation(window, position, animation,animation_context_, scale);
}

/**
 * @brief 初始化Enemy动画
 */
// void EnemyUI::InitializeAnimation()
// {
//     if (enemy_ == nullptr) {
//         return;
//     }

//     // 记录初始状态
//     last_state_ = enemy_->animation.state;
//     last_heading_ = enemy_->heading;

//     // 根据Enemy状态确定是否循环
//     bool should_loop = (enemy_->animation.state != State::Death && enemy_->animation.state != State::Attack);

//     // 开始播放初始动画（传递animation_context_）
//     animation_player_.PlayAnimation(*enemy_, animation_context_, should_loop);

//     initialized_ = true;

//     DEBUG_CODE(std::cout << "EnemyUI: Initialized animation for " << enemy_->animation.prefix
//               << " with state " << static_cast<int>(enemy_->animation.state)
//               << " and heading " << static_cast<int>(enemy_->heading) << std::endl;)
// }

/**
 * @brief 更新动画状态
 */
// void EnemyUI::UpdateAnimationState()
// {
//     if (enemy_ == nullptr || !initialized_) {
//         return;
//     }

//     // 检查是否需要更新动画
//     if (NeedsAnimationUpdate()) {
//         DEBUG_CODE(std::cout << "EnemyUI: State/Heading changed for " << enemy_->animation.prefix
//                   << " from (" << static_cast<int>(last_state_) << "," << static_cast<int>(last_heading_)
//                   << ") to (" << static_cast<int>(enemy_->animation.state) << "," << static_cast<int>(enemy_->heading) << ")" << std::endl;)

//         // 更新记录的状态
//         last_state_ = enemy_->animation.state;
//         last_heading_ = enemy_->heading;

//         // 根据新状态确定是否循环播放
//         bool should_loop = true;
//         if (enemy_->animation.state == State::Death || enemy_->animation.state == State::Attack) {
//             should_loop = false;
//         }

//         // 播放新动画
//         animation_player_.PlayAnimation(*enemy_, animation_context_, should_loop);
//     }

//     // 自动更新动画帧
//     animation_player_.Update(*enemy_, animation_context_);
// }

/**
 * @brief 检查是否需要更新动画
//  */
// bool EnemyUI::NeedsAnimationUpdate() const
// {
//     if (enemy_ == nullptr) {
//         return false;
//     }

//     return (last_state_ != enemy_->animation.state) || (last_heading_ != enemy_->heading);
// }