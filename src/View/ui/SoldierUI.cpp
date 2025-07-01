#include "SoldierUI.h"
#include <iostream>
#include "utils/macros.h"

/**
 * @brief 构造函数
 */
SoldierUI::SoldierUI(Soldier* soldier, AnimationPlayer& animation_player)
    : soldier_(soldier)
    , animation_player_(animation_player)
    , animation_context_()      // 初始化独立的动画上下文
    //, last_state_(State::Idle)
    //, last_heading_(Heading::Right)
    //, initialized_(false)
{
    if (soldier_ == nullptr) {
        std::cerr << "Warning: SoldierUI created with null soldier pointer" << std::endl;
        return;
    }
    
    DEBUG_CODE(std::cout << "SoldierUI created for soldier at (" << soldier_->position.x << "," << soldier_->position.y 
              << ") with prefix: " << soldier_->animation.prefix 
              << ", heading: " << static_cast<int>(soldier_->heading) << std::endl;)
    
}

/**
 * @brief 渲染Soldier
 */
void SoldierUI::Render(sf::RenderWindow& window, const Position& position, Animation& animation,const sf::Vector2f& scale)
{
    if (soldier_ == nullptr) {
        return;
    }

    // 如果Soldier已死亡且动画播放完成，不再渲染
    if (animation.state == State::Death && animation_player_.IsAnimationFinished(animation, animation_context_)) {
        return;
    }

    // 更新动画状态
    animation_player_.UpdateAnimation(animation,animation_context_);

    // 渲染Soldier
    animation_player_.RenderAnimation(window, position, animation,animation_context_, scale);
}

/**
 * @brief 初始化Soldier动画
 */
// void SoldierUI::InitializeAnimation()
// {
//     if (soldier_ == nullptr) {
//         return;
//     }

//     // 记录初始状态
//     last_state_ = soldier_->animation.state;
//     last_heading_ = soldier_->heading;

//     // 根据Soldier状态确定是否循环
//     bool should_loop = (soldier_->animation.state != State::Death && soldier_->animation.state != State::Attack);

//     // 开始播放初始动画（传递animation_context_）
//     animation_player_.PlayAnimation(*soldier_, animation_context_, should_loop);

//     initialized_ = true;

//     DEBUG_CODE(std::cout << "SoldierUI: Initialized animation for " << soldier_->animation.prefix
//               << " with state " << static_cast<int>(soldier_->animation.state)
//               << " and heading " << static_cast<int>(soldier_->heading) << std::endl;)
// }

/**
 * @brief 更新动画状态
 */
// void SoldierUI::UpdateAnimationState()
// {
//     if (soldier_ == nullptr || !initialized_) {
//         return;
//     }

//     // 检查是否需要更新动画
//     if (NeedsAnimationUpdate()) {
//         DEBUG_CODE(std::cout << "SoldierUI: State/Heading changed for " << soldier_->animation.prefix
//                   << " from (" << static_cast<int>(last_state_) << "," << static_cast<int>(last_heading_)
//                   << ") to (" << static_cast<int>(soldier_->animation.state) << "," << static_cast<int>(soldier_->heading) << ")" << std::endl;)

//         // 更新记录的状态
//         last_state_ = soldier_->animation.state;
//         last_heading_ = soldier_->heading;

//         // 根据新状态确定是否循环播放
//         bool should_loop = true;
//         if (soldier_->animation.state == State::Death || soldier_->animation.state == State::Attack) {
//             should_loop = false;
//         }

//         // 播放新动画（传递animation_context_）
//         animation_player_.PlayAnimation(*soldier_, animation_context_, should_loop);
//     }

//     // 自动更新动画帧（传递animation_context_）
//     animation_player_.Update(*soldier_, animation_context_);
// }

/**
 * @brief 检查是否需要更新动画
 */
// bool SoldierUI::NeedsAnimationUpdate() const
// {
//     if (soldier_ == nullptr) {
//         return false;
//     }

//     return (last_state_ != soldier_->animation.state) || (last_heading_ != soldier_->heading);
// }