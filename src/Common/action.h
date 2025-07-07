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
    std::string sound_name;
};

struct SelectLevelParams
{
    std::string level_name;
};

struct SellTowerParams
{
    ID tower_id;   // 要出售的塔的 ID
    // int       price; //出售的价格
    // TowerType tower_type;
    // Position  offset; // 出售有无效果需要渲染？
};

using CreateActionFxProps = std::variant<std::monostate,UpgradeTowerParams, SellTowerParams, std::string>;

struct CreateActionFxParams{
    FxType fx_type;
    Position position; // 在 View 层被赋值
    // ID id; // 任何拥有 CreateActionFx 事件的实体，都应该把它的 ID 赋给这个参数
    Position offset;
    CreateActionFxProps props = std::monostate{}; // 可能是 UpgradeTowerParams 或 SellTowerParams
    template<typename PropType>
    CreateActionFxParams(const FxType fx_type, const Position& position, const Position& offset,
                         const PropType& props)
        : fx_type(fx_type)
        , position(position)
        , offset(offset)
        , props(props)
    {}
};

// Delete Action 不带任何参数，就是让 ViewModel 层删除所有的 ActionFx 实体。

struct CheckSoldierStatusParams
{
    ID soldier_id; // 要检查的士兵 ID
    //Status? 士兵状态
};

struct CheckEnemyStatusParams
{
    ID enemy_id;
    //Status? 敌人状态
};

struct CheckTowerStatusParams
{
    ID tower_id;
    //Status? 塔状态
    // 可能还需要其他信息，比如塔的等级、攻击力、金钱等
};

struct ChangeRallyPointParams
{
    ID        soldier_id; // 要改变集结点的士兵 ID
    Position  new_rally_point;
};



using Params = std::variant<std::monostate, UpgradeTowerParams, SelectLevelParams, CreateActionFxParams,CheckSoldierStatusParams,
                            CheckEnemyStatusParams, CheckTowerStatusParams, ChangeRallyPointParams,SellTowerParams>;

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

