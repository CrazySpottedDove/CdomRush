#include "TowerUI.h"
#include <iostream>
#include "utils/macros.h"

/**
 * @brief 构造函数
 */
TowerUI::TowerUI(Tower* tower, AnimationPlayer& animation_player)
    : tower_(tower)
    , animation_player_(animation_player)
    , animation_context_()  // 初始化独立的动画上下文
    , last_state_(State::Idle)
    , initialized_(false)
{
    if (tower_) {
        DEBUG_CODE(std::cout << "TowerUI created for tower at (" << tower_->position.x << "," << tower_->position.y
                  << ") with prefix: " << tower_->animation.prefix << std::endl;)
        
    }
}

void TowerUI::Render(sf::RenderWindow& window, const sf::Vector2f& scale)
{
    if (tower_ == nullptr) {
        return;
    }
    
    if (!initialized_) {
        InitializeAnimation();
    }
    
    UpdateAnimationState();
    // 渲染Tower（使用Entity接口，不支持翻转）
    animation_player_.Render(window, *tower_, animation_context_, scale);
}

void TowerUI::InitializeAnimation()
{
    if (tower_ == nullptr) {
        return;
    }
    
    last_state_ = tower_->animation.state;
    // Tower的默认状态通常是Idle，循环播放
    bool should_loop = (tower_->animation.state == State::Idle);
    animation_player_.PlayAnimation(*tower_, animation_context_, tower_->animation.state, should_loop);
    
    initialized_ = true;
    
    DEBUG_CODE(std::cout << "TowerUI: Initialized animation for " << tower_->animation.prefix 
              << " with state " << static_cast<int>(tower_->animation.state) << std::endl;)
}

void TowerUI::UpdateAnimationState()
{
    if (tower_ == nullptr || !initialized_) {
        return;
    }
    
    if (NeedsAnimationUpdate()) {
        DEBUG_CODE(std::cout << "TowerUI: State changed for " << tower_->animation.prefix 
                  << " from " << static_cast<int>(last_state_) 
                  << " to " << static_cast<int>(tower_->animation.state) << std::endl;)
        
        last_state_ = tower_->animation.state;
        
        // 根据新状态确定是否循环播放
        bool should_loop = true;
        if (tower_->animation.state == State::Shoot || tower_->animation.state == State::Attack) {
            should_loop = false;  // 攻击动画不循环
        }
        
        // 播放新动画（Entity接口需要明确指定状态）
        animation_player_.PlayAnimation(*tower_, animation_context_, tower_->animation.state, should_loop);
    }
    
    // 自动更新动画帧（传递animation_context_）
    animation_player_.Update(*tower_, animation_context_);
}

bool TowerUI::NeedsAnimationUpdate() const
{
    if (tower_ == nullptr) {
        return false;
    }
    
    return (last_state_ != tower_->animation.state);
} 