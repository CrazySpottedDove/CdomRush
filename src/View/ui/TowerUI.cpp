#include "TowerUI.h"
#include <iostream>

TowerUI::TowerUI(Tower* tower, AnimationPlayer& animation_player)
    : tower_(tower)
    , animation_player_(animation_player)
    , last_state_(State::Idle)
    , initialized_(false)
{
    if (tower_ == nullptr) {
        std::cerr << "Warning: TowerUI created with null tower pointer" << std::endl;
        return;
    }
    
    std::cout << "TowerUI created for tower at (" << tower_->position.x << "," << tower_->position.y 
              << ") with prefix: " << tower_->animation.prefix << std::endl;
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
    animation_player_.Render(window, *tower_, scale);
}

void TowerUI::InitializeAnimation()
{
    if (tower_ == nullptr) {
        return;
    }
    
    last_state_ = tower_->animation.state;
    bool should_loop = true;
    animation_player_.PlayAnimation(*tower_, tower_->animation.state, should_loop);
    
    initialized_ = true;
    
    std::cout << "TowerUI: Initialized animation for " << tower_->animation.prefix 
              << " with state " << static_cast<int>(tower_->animation.state) << std::endl;
}

void TowerUI::UpdateAnimationState()
{
    if (tower_ == nullptr || !initialized_) {
        return;
    }
    
    if (NeedsAnimationUpdate()) {
        std::cout << "TowerUI: State changed for " << tower_->animation.prefix 
                  << " from " << static_cast<int>(last_state_) 
                  << " to " << static_cast<int>(tower_->animation.state) << std::endl;
        
        last_state_ = tower_->animation.state;
        bool should_loop = true;
        animation_player_.PlayAnimation(*tower_, tower_->animation.state, should_loop);
    }
    
    animation_player_.Update(*tower_);
}

bool TowerUI::NeedsAnimationUpdate() const
{
    if (tower_ == nullptr) {
        return false;
    }
    
    return (last_state_ != tower_->animation.state);
} 