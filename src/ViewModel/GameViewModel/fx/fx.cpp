#include "ViewModel/GameViewModel/fx/fx.h"
#include "Common/action.h"
#include "Common/animation.h"
#include "Common/macros.h"
#include "Common/state.h"
#include "Common/type.h"
#include "Common/viewData.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/towers/towers.h"
#include <variant>

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
    animations[0].actions.emplace_back(Action(ActionType::UpgradeTower, params));
    animations.emplace_back(Animation(State::Idle, "price_tag", Position(0, -30)));
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
    animations[0].actions.emplace_back(Action(ActionType::UpgradeTower, params));
    animations.emplace_back(Animation(State::Idle, "price_tag", Position(0, -30)));
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
    animations[0].actions.emplace_back(Action(ActionType::UpgradeTower, params));
    animations.emplace_back(Animation(State::Idle, "price_tag", Position(0, -30)));
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
    animations[0].actions.emplace_back(Action(ActionType::UpgradeTower, params));
    animations.emplace_back(Animation(State::Idle, "price_tag", Position(0, -30)));
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

UpgradeToBarrackButton::UpgradeToBarrackButton(const UpgradeTowerParams& params)
{
    animations.emplace_back(Animation(State::Disabled, "icon_barrack"));
    animations[0].actions.emplace_back(Action(ActionType::UpgradeTower, params));
    animations.emplace_back(Animation(State::Idle, "price_tag", Position(0, -30)));
}

void UpgradeToBarrackButton::Update(Store& store)
{
    if (store.gold < 70) {
        animations[0].current_state = State::Disabled;
    }
    else {
        animations[0].current_state = State::Enabled;
    }
}

SellTowerButton::SellTowerButton(const SellTowerParams& params)
{
    animations.emplace_back(Animation(State::Enabled, "icon_sell"));
    animations[0].actions.emplace_back(Action(ActionType::SellTower, params));
    animations.emplace_back(Animation(State::Idle, "price_tag", Position(0, -30)));
}

void TextFx::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(ViewData(Stringfy(store), position, TEXT_LAYER));
}

GoldStat::GoldStat()
{
    position.x = -20;
    position.y = 745;
}

std::string GoldStat::Stringfy(Store& store)
{
    return std::to_string((int)store.gold);
}

LifeStat::LifeStat()
{
    position.x = -80;
    position.y = 745;
}

std::string LifeStat::Stringfy(Store& store)
{
    return std::to_string(store.life);
}

WaveStat::WaveStat()
{
    position.x = 90;
    position.y = 745;
}

std::string WaveStat::Stringfy(Store& store)
{
    const int current_wave_count = store.current_wave_index + 1;   // current_wave_index is 0-based
    const int total_wave_count   = store.resource_manager.GetWaves()->size();
    return std::to_string(current_wave_count < total_wave_count ? current_wave_count
                                                                : total_wave_count) +
           "/" + std::to_string(total_wave_count);
}

PlainTextFx::PlainTextFx(const std::string& text)
{
    this->text = text;
}

void GameOverTextFx::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(ViewData(Stringfy(store), position, GAMEOVER_TEXT_LAYER));
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

void ActionTextFx::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(ViewData(Stringfy(store), position, TEXT_LAYER));
}

PlainActionTextFx::PlainActionTextFx(const std::string& text)
{
    this->text = text;
}

LoadingGrass::LoadingGrass()
{
    animations.emplace_back(Animation(State::Idle, "loading_grass"));
    animations[0].anchor_x = 0.0;
    animations[0].anchor_y = 0.0;
    position.x             = -157.5;
    position.y             = 768;
}

GameOverFailure::GameOverFailure()
{
    animations.emplace_back(Animation(State::Idle, "defeat_bg_notxt"));
    position.x = X_CENTER;
    position.y = Y_CENTER;
}

void GameOverFailure::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(ViewData(&animations, position, GAMEOVER_BACKGROUND_LAYER));
}

GameOverVictory::GameOverVictory()
{
    animations.emplace_back(Animation(State::Idle, "victoryBadges_notxt"));
    animations[0].actions.emplace_back(Action(ActionType::BackToBegin, std::monostate{}));
    position.x = X_CENTER;
    position.y = Y_CENTER;
}

void GameOverVictory::QueueViewData(Store& store)
{
    store.GetViewDataQueue()->emplace(ViewData(&animations, position, GAMEOVER_BACKGROUND_LAYER));
    animations[0].actions.emplace_back(Action(ActionType::BackToBegin, std::monostate{}));
}

TowerRangeFx::TowerRangeFx(const ID tower_id)
    : tower_id(tower_id)
{}

void TowerRangeFx::QueueViewData(Store& store)
{
    Tower* tower = store.GetTower(tower_id);
    if (!tower) {
        return;   // 如果塔不存在，直接返回
    }

    INFO("Render Tower Range at position: (" + std::to_string(tower->position.x) + ", " +
         std::to_string(tower->position.y) +
         ") with range: " + std::to_string(tower->ranged.attacks[0].range));
    store.GetViewDataQueue()->emplace(
        ViewData(tower->ranged.attacks[0].range, tower->position, nullptr));
}

BarrackRangeFx::BarrackRangeFx(const ID tower_id)
    : tower_id(tower_id)
{
    animations.emplace_back(Animation());
    animations[0].hidden = true;
    animations[0].actions.emplace_back(
        Action(ActionType::ChangeRallyPoint, ChangeRallyPointParams{tower_id, Position()}));
}

void BarrackRangeFx::QueueViewData(Store& store)
{
    Tower* tower = store.GetTower(tower_id);
    if (!tower) {
        return;   // 如果塔不存在，直接返回
    }

    store.GetViewDataQueue()->emplace(
        ViewData(dynamic_cast<Barrack*>(tower)->rally_range, tower->position, &animations));
}

BarrackChangeRallyPointButton::BarrackChangeRallyPointButton(const ID tower_id)
    : tower_id(tower_id)
{
    animations.emplace_back(
        Animation(State::Idle, "sub_icons_0001", Position(0, 0)));
}

bool BarrackChangeRallyPointButton::Insert(Store& store)
{
    animations[0].actions.emplace_back(
        Action(ActionType::CreateActionFx,
               CreateActionFxParams(FxType::BarrackRange, position, Position(-50, 0), tower_id)));
    return true;
}