#include "ViewModel/GameViewModel/fx/fx.h"
#include "Common/action.h"
#include "Common/animation.h"
#include "Common/macros.h"
#include "Common/state.h"
#include "Common/type.h"
#include "Common/viewData.h"
#include "ViewModel/GameViewModel/store/store.h"

void Fx::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(
        ViewData(&animations, position, FX_LAYER)
    );
}

LevelFlag::LevelFlag()
{
    animations.emplace_back(Animation(State::Idle, "map_flag"));
}

void ActionFx::QueueViewData(Store& store){
    store.GetViewDataQueue()->emplace(
        ViewData(&animations, position, UI_UPPER_LAYER)
    );
}

Map::Map()
{
    animations.emplace_back(Animation(State::Idle, "map_background"));
    animations[0].anchor_x = 0.0;
    animations[0].anchor_y = 0.0;
    position.x             = -157.5;
    position.y             = 768;
}

void Map::QueueViewData(Store& store){
    store.GetViewDataQueue()->emplace(
        ViewData(&animations, position,MAP_LAYER)
    );
}

CommonUpgradeIcon::CommonUpgradeIcon()
{
    // animations.emplace_back(Animation(State::Enabled,))
}

Explosion::Explosion()
{
    animations.emplace_back(Animation(State::Hit, "explosion_big"));
}

BloodSplat::BloodSplat()
{
    animations.emplace_back(Animation(State::Hit,"fx_blood_splat_red"));
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

UpgradeToMageButton::UpgradeToMageButton()
{
    animations.emplace_back(Animation(State::Disabled, "icon_mage"));
    animations[0].actions.emplace_back(
        Action(ActionType::UpgradeTower, UpgradeTowerParams{INVALID_ID, TowerType::Mage1, 100}));
}

void UpgradeToMageButton::Update(Store& store)
{
    if (store.gold < 100) {
        animations[0].current_state = State::Disabled;
    }
    else {
        animations[0].current_state = State::Enabled;
    }
}

UpgradeToEngineerButton::UpgradeToEngineerButton()
{
    animations.emplace_back(Animation(State::Disabled, "icon_engineer"));
    animations[0].actions.emplace_back(
        Action(ActionType::UpgradeTower, UpgradeTowerParams{INVALID_ID, TowerType::Engineer1, 125}));
}

void UpgradeToEngineerButton::Update(Store& store)
{
    if (store.gold < 125) {
        animations[0].current_state = State::Disabled;
    }
    else {
        animations[0].current_state = State::Enabled;
    }
}

GoldStat::GoldStat(){
    position.x = -130;
    position.y = 740;
}

void GoldStat::QueueViewData(Store& store){
    store.GetViewDataQueue()->emplace(
        ViewData(std::to_string(store.gold),position ,UI_UPPER_LAYER )
    );
}