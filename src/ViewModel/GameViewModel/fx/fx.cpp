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
    store.GetViewDataQueue()->emplace(ViewData(&animations, position, FX_LAYER));
}

LevelFlag::LevelFlag()
{
    animations.emplace_back(Animation(State::Idle, "map_flag"));
}

void ActionFx::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(ViewData(&animations, position, UI_UPPER_LAYER));
}

Map::Map()
{
    animations.emplace_back(Animation(State::Idle, "map_background"));
    animations[0].anchor_x = 0.0;
    animations[0].anchor_y = 0.0;
    position.x             = -157.5;
    position.y             = 768;
}

void Map::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(ViewData(&animations, position, MAP_LAYER));
}

CommonUpgradeButton::CommonUpgradeButton(const UpgradeTowerParams& params)
{
    price = params.cost;
    animations.emplace_back(Animation(State::Disabled, "common_upgrade_button"));
    animations[0].actions.emplace_back(
        Action(ActionType::UpgradeTower, params));
}

void CommonUpgradeButton::Update(Store& store)
{
    if (store.gold < price) {
        animations[0].current_state = State::Disabled;
    }
    else {
        animations[0].current_state = State::Enabled;
    }
}

Explosion::Explosion()
{
    animations.emplace_back(Animation(State::Hit, "explosion_big"));
}

BloodSplat::BloodSplat()
{
    animations.emplace_back(Animation(State::Hit, "fx_blood_splat_red"));
}

UpgradeToArcherButton::UpgradeToArcherButton(const UpgradeTowerParams& params)
{
    animations.emplace_back(Animation(State::Disabled, "icon_archer"));
    animations[0].actions.emplace_back(
        Action(ActionType::UpgradeTower, params));
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

UpgradeToMageButton::UpgradeToMageButton(const UpgradeTowerParams& params)
{
    animations.emplace_back(Animation(State::Disabled, "icon_mage"));
    animations[0].actions.emplace_back(
        Action(ActionType::UpgradeTower, params));
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

UpgradeToEngineerButton::UpgradeToEngineerButton(const UpgradeTowerParams& params)

{
    animations.emplace_back(Animation(State::Disabled, "icon_engineer"));
    animations[0].actions.emplace_back(
        Action(ActionType::UpgradeTower, params));
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

GoldStat::GoldStat()
{
    position.x = -20;
    position.y = 745;
}

void GoldStat::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(
        ViewData(std::to_string((int)store.gold), position, UI_UPPER_LAYER));
}

LifeStat::LifeStat()
{
    position.x = -80;
    position.y = 745;
}

void LifeStat::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(
        ViewData(std::to_string(store.life), position, UI_UPPER_LAYER));
}

WaveStat::WaveStat()
{
    position.x = 90;
    position.y = 745;
}

void WaveStat::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(
        ViewData(std::to_string(store.current_wave_index + 1) + "/" +
                     std::to_string(store.resource_manager.GetWaves()->size()),
                 position,
                 UI_UPPER_LAYER));
}

TopLeft::TopLeft()
{
    animations.emplace_back(Animation(State::Idle, "top_left"));
    animations[0].anchor_x = 0.0;
    animations[0].anchor_y = 0.0;
    position.x             = -157.5;
    position.y             = 790;
}

void TopLeft::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(ViewData(&animations, position, UI_LOWER_LAYER));
}