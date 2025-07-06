#pragma once
#include "ViewModel/GameViewModel/components/armor.h"
#include "ViewModel/GameViewModel/components/health.h"

#include "ViewModel/GameViewModel/templates/activeEntity.h"
#include "Common/macros.h"

// 拥有生命的实体
enum class UnitType
{
    Soldier,   // 士兵
    Enemy     // 敌人
};

enum class Heading
{
    Up,
    Down,
    Left,
    Right,
    None
};

class Unit : public ActiveEntity
{
public:
    Heading heading = Heading::Right; // 方向
    UnitType     type; // 单位类型
    Health       health;
    Armor        armor;
    double       speed;
    Position Hit_offset = {0.0f, 0.0f}; // 受击偏移位置
    Position health_bar_offset = {0.0f, 0.0f}; // 血条偏移位置
    virtual bool OnDamage();
    virtual ~Unit() = default;
    bool  is_moving();
    State        walkjudge();
    virtual void death_action() = 0;   // 纯虚函数，用于处理unit死亡后的行为
    void         QueueViewData(Store& store) override;
};