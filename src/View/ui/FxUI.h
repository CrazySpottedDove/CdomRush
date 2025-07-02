#pragma once
#include "BaseUI.h"
#include "Model/components/animation.h"
#include "Model/fx/fx.h"
#include "View/animation/animationPlayer.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class FxUI : public BaseUI
{
public:
    explicit FxUI(Fx* fx, AnimationPlayer& animation_player);

    void Render(sf::RenderWindow& window, const Position& position, Animation& animation,
                const sf::Vector2f& scale = {1.0f, 1.0f}) override;

    Fx* GetFx() const { return fx_; }

private:
    Fx*              fx_;
    AnimationPlayer& animation_player_;
    AnimationContext animation_context_;
};