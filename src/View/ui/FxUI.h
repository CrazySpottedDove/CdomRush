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
    explicit FxUI(Fx* fx);

    void OnClick(Store& store, sf::RenderWindow& window) override;
    Fx* GetFx() const { return fx_; }

private:
    Fx*              fx_;
};