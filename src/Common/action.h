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
    CreateActionFx,
    Delete,
    None
};

struct UpgradeTowerParams
{
    ID        tower_id; // 要替换的塔的 ID
    TowerType new_tower_type; // 新的塔的类型
    int       cost; // 升级的价格
};

struct SelectLevelParams
{
    std::string level_name;
};

struct CreateActionFxParams{
    FxType fx_type;
    Position position; // 在 View 层被赋值
    ID id; // 任何拥有 CreateActionFx 事件的实体，都应该把它的 ID 赋给这个参数
    Position offset;
};

// Delete Action 不带任何参数，就是让 ViewModel 层删除所有的 ActionFx 实体。

// TODO: 日后处理别的事件的时候，就定义对应类型的 Params 结构体，然后添加到 Params 的 variant 中。
using Params = std::variant<std::monostate, UpgradeTowerParams, SelectLevelParams, CreateActionFxParams>;

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

