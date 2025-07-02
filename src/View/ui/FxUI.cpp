#include "FxUI.h"
#include "Model/components/animation.h"
#include "View/animation/animationPlayer.h"
#include "utils/macros.h"
#include <iostream>

FxUI::FxUI(Fx* fx, AnimationPlayer& animation_player)
    : fx_(fx)
    , animation_player_(animation_player)
    , animation_context_()
{
    DEBUG_CODE(
        if (fx_ == nullptr) { throw std::runtime_error("FxUI: fx pointer is null"); } std::cout
            << "FxUI created for fx with id: " << fx->id << std::endl;)
}

void FxUI::Render(sf::RenderWindow& window, const Position& position,Animation& animation, const sf::Vector2f& scale)
{
    if (fx_ == nullptr) {
        std::cerr << "FxUI: fx pointer is null, cannot render." << std::endl;
        return;
    }

    // TODO: Death 的处理
    if(animation_player_.IsAnimationFinished(animation, animation_context_)){
        return;
    }

    animation_player_.UpdateAnimation(animation,animation_context_);

    animation_player_.RenderAnimation(window, position, animation, animation_context_,scale);
    // 更新动画到下一帧
}