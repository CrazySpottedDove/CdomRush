#include "SoldierUI.h"
#include <iostream>

/**
 * @brief 构造函数
 */
SoldierUI::SoldierUI(Soldier* soldier, AnimationPlayer& animation_player)
    : soldier_(soldier)
    , animation_player_(animation_player)
    , animation_context_()  // 初始化动画上下文
    , last_state_(State::Idle)
    , last_heading_(Heading::Right)
    , initialized_(false)
{
    if (soldier_ == nullptr) {
        std::cerr << "Warning: SoldierUI created with null soldier pointer" << std::endl;
        return;
    }
    
    std::cout << "SoldierUI created for soldier at (" << soldier_->position.x << "," << soldier_->position.y 
              << ") with prefix: " << soldier_->animation.prefix << std::endl;
}

/**
 * @brief 渲染Soldier
 */
void SoldierUI::Render(sf::RenderWindow& window, const sf::Vector2f& scale)
{
    if (soldier_ == nullptr) {
        return;
    }
    
    // 如果Soldier已死亡且动画播放完成，不再渲染
    if (soldier_->state == State::Death && animation_player_.IsAnimationFinished(*soldier_, animation_context_)) {
        return;
    }
    
    // 初始化动画（仅在第一次调用时）
    if (!initialized_) {
        InitializeAnimation();
    }
    
    // 更新动画状态
    UpdateAnimationState();
    
    // 渲染Soldier（使用Unit接口，支持状态驱动和翻转）
    animation_player_.Render(window, *soldier_, animation_context_, scale);
}

/**
 * @brief 初始化Soldier动画
 */
void SoldierUI::InitializeAnimation()
{
    if (soldier_ == nullptr) {
        return;
    }
    
    // 记录初始状态
    last_state_ = soldier_->state;
    last_heading_ = soldier_->heading;
    
    // 根据Soldier状态确定是否循环
    bool should_loop = (soldier_->state != State::Death && soldier_->state != State::Attack);
    
    // 开始播放初始动画（传递animation_context_）
    animation_player_.PlayAnimation(*soldier_, animation_context_, should_loop);
    
    initialized_ = true;
    
    std::cout << "SoldierUI: Initialized animation for " << soldier_->animation.prefix 
              << " with state " << static_cast<int>(soldier_->state) 
              << " and heading " << static_cast<int>(soldier_->heading) << std::endl;
}

/**
 * @brief 更新动画状态
 */
void SoldierUI::UpdateAnimationState()
{
    if (soldier_ == nullptr || !initialized_) {
        return;
    }
    
    // 检查是否需要更新动画
    if (NeedsAnimationUpdate()) {
        std::cout << "SoldierUI: State/Heading changed for " << soldier_->animation.prefix 
                  << " from (" << static_cast<int>(last_state_) << "," << static_cast<int>(last_heading_) 
                  << ") to (" << static_cast<int>(soldier_->state) << "," << static_cast<int>(soldier_->heading) << ")" << std::endl;
        
        // 更新记录的状态
        last_state_ = soldier_->state;
        last_heading_ = soldier_->heading;
        
        // 根据新状态确定是否循环播放
        bool should_loop = true;
        if (soldier_->state == State::Death || soldier_->state == State::Attack) {
            should_loop = false; 
        }
        
        // 播放新动画（传递animation_context_）
        animation_player_.PlayAnimation(*soldier_, animation_context_, should_loop);
    }
    
    // 自动更新动画帧（传递animation_context_）
    animation_player_.Update(*soldier_, animation_context_);
}

/**
 * @brief 检查是否需要更新动画
 */
bool SoldierUI::NeedsAnimationUpdate() const
{
    if (soldier_ == nullptr) {
        return false;
    }
    
    return (last_state_ != soldier_->state) || (last_heading_ != soldier_->heading);
} 