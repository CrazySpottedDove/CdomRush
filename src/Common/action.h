#pragma once

// 点击一个实体后可以触发的 UI 事件。如，点击一座塔后，View 层检查 Animation 中的 actions
// 域，发现有事件种类为 UpgradeTower，于是渲染对应的 UI 界面；发现有
// CheckTowerStatus，于是在下方渲染了塔的具体信息。
#include "Common/type.h"
#include "Common/macros.h"
#include <string>
#include <variant>

struct Animation;

enum class ActionType
{
    UpgradeTower,
    ChangeRallyPoint,
    SellTower,
    CheckSoldierStatus,
    CheckEnemyStatus,
    CheckTowerStatus,
    SelectLevel,
    None
};

struct UpgradeTowerParams
{
    ID        tower_id;
    TowerType new_tower_type;
    int       cost;
    Animation* animation;
    sf::Vector2f offset;  // 渲染升级界面时的偏移量
};

struct SelectLevelParams
{
    std::string level_name;
};

// TODO: 日后处理别的事件的时候，就定义对应类型的 Params 结构体，然后添加到 Params 的 variant 中。
using Params = std::variant<std::monostate, UpgradeTowerParams, SelectLevelParams>;

struct Action
{
    ActionType type = ActionType::None;
    Params     param = std::monostate{};

    template<typename ParamsType>
    Action(const ActionType type, const ParamsType& params)
        : type(type)
        , param(params)
    {}

    Action(const ActionType type)
        : type(type)
        , param(std::monostate{})
    {}

    Action() = default;
};

