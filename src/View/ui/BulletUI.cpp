#include "BulletUI.h"
#include <iostream>

/**
 * @brief 构造函数
 */
BulletUI::BulletUI(Bullet* bullet, AnimationPlayer& animation_player)
    : bullet_(bullet)
    , animation_player_(animation_player)
    , last_state_(State::Idle)
    , initialized_(false)
{
    if (bullet_ == nullptr) {
        std::cerr << "Warning: BulletUI created with null bullet pointer" << std::endl;
        return;
    }
    
    std::cout << "BulletUI created for bullet at (" << bullet_->position.x << "," << bullet_->position.y 
              << ") with prefix: " << bullet_->animation.prefix << std::endl;
}

/**
 * @brief 渲染Bullet
 */
void BulletUI::Render(sf::RenderWindow& window, const sf::Vector2f& scale)
{
    if (bullet_ == nullptr) {
        return;
    }
    
    // 如果Bullet已击中且动画播放完成，不再渲染
    if (bullet_->animation.state == State::Hit && animation_player_.IsAnimationFinished(*bullet_)) {
        return;
    }
    
    // 初始化动画（仅在第一次调用时）
    if (!initialized_) {
        InitializeAnimation();
    }
    
    // 更新动画状态
    UpdateAnimationState();
    
    // 渲染Bullet（使用Entity接口，基础动画播放）
    animation_player_.Render(window, *bullet_, scale);
}

/**
 * @brief 初始化Bullet动画
 */
void BulletUI::InitializeAnimation()
{
    if (bullet_ == nullptr) {
        return;
    }
    
    // 记录初始状态
    last_state_ = bullet_->animation.state;
    
    // 开始播放初始动画（根据Bullet状态确定是否循环）
    bool should_loop = (bullet_->animation.state == State::Flying);  // 飞行时循环，击中时不循环
    animation_player_.PlayAnimation(*bullet_, bullet_->animation.state, should_loop);
    
    initialized_ = true;
    
    std::cout << "BulletUI: Initialized animation for " << bullet_->animation.prefix 
              << " with state " << static_cast<int>(bullet_->animation.state) << std::endl;
}

/**
 * @brief 更新动画状态
 */
void BulletUI::UpdateAnimationState()
{
    if (bullet_ == nullptr || !initialized_) {
        return;
    }
    
    // 检查是否需要更新动画
    if (NeedsAnimationUpdate()) {
        std::cout << "BulletUI: State changed for " << bullet_->animation.prefix 
                  << " from " << static_cast<int>(last_state_) 
                  << " to " << static_cast<int>(bullet_->animation.state) << std::endl;
        
        // 更新记录的状态
        last_state_ = bullet_->animation.state;
        
        // 根据新状态确定是否循环播放
        bool should_loop = (bullet_->animation.state == State::Flying);  // 飞行时循环，击中时不循环
        
        // 播放新动画
        animation_player_.PlayAnimation(*bullet_, bullet_->animation.state, should_loop);
    }
    
    // 自动更新动画帧（Entity版本的简单前进）
    animation_player_.Update(*bullet_);
    
    // TODO: 后续实现旋转逻辑
    // UpdateRotation();
}

/**
 * @brief 检查是否需要更新动画
 */
bool BulletUI::NeedsAnimationUpdate() const
{
    if (bullet_ == nullptr) {
        return false;
    }
    
    return (last_state_ != bullet_->animation.state);
}

// TODO: 后续实现旋转逻辑
// void BulletUI::UpdateRotation()
// {
//     if (bullet_ == nullptr) {
//         return;
//     }
//     
//     // 计算移动方向
//     sf::Vector2f movement = bullet_->position - last_position_;
//     if (movement.x != 0.0f || movement.y != 0.0f) {
//         // 计算旋转角度
//         rotation_angle_ = std::atan2(movement.y, movement.x);
//         bullet_->animation.rotation = rotation_angle_;
//     }
//     
//     last_position_ = bullet_->position;
// } 