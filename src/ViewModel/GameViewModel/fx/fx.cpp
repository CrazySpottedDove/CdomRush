#include "ViewModel/GameViewModel/fx/fx.h"
#include "Common/action.h"
#include "Common/animation.h"
#include "Common/macros.h"
#include "Common/state.h"
#include "Common/type.h"
#include "ViewModel/GameViewModel/store/store.h"

LevelFlag::LevelFlag()
{
    animations.emplace_back(Animation(State::Idle, "map_flag"));
}

Map::Map()
{
    animations.emplace_back(Animation(State::Idle, "map_background"));
    animations[0].anchor_x = 0.0;
    animations[0].anchor_y = 0.0;
    position.x             = -157.5;
    position.y             = 768;
}

CommonUpgradeIcon::CommonUpgradeIcon()
{
    // animations.emplace_back(Animation(State::Enabled,))
}

Explosion::Explosion()
{
    animations.emplace_back(Animation(State::Hit, "explosion_big"));
}

UpgradeToArcherButton::UpgradeToArcherButton()
{
    animations.emplace_back(Animation(State::Disabled, "icon_archer"));
    animations[0].actions.emplace_back(
        Action(ActionType::UpgradeTower, UpgradeTowerParams{INVALID_ID, TowerType::Archer1, 70}));
}

void UpgradeToArcherButton::Update(Store& store)
{
    if (store.gold < 70) {
        animations[0].current_state = State::Disabled;
    }
    else {
        animations[0].current_state = State::Enabled;
    }
}