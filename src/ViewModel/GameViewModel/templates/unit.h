#pragma once
#include "ViewModel/GameViewModel/components/armor.h"
#include "ViewModel/GameViewModel/components/health.h"
#include "Common/state.h"
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
    virtual bool OnDamage() { return true; };
    virtual ~Unit() = default;

    bool is_moving(){
        if(animations[0].current_state == State::WalkingLeftRight || animations[0].current_state == State::WalkingUp || animations[0].current_state == State::WalkingDown || animations[0].current_state == State::Running) {
            return true; // 如果状态是左右行走、向上行走、向下行走或奔跑，返回 true
        }
        return false; // 否则返回 false
    }
    State walkjudge(){
        if(heading == Heading::Right) return State::WalkingLeftRight; // 如果方向是向右，设置状态为左右行走
        else if(heading == Heading::Up) return State::WalkingUp; // 如果方向是向上，设置状态为向上行走
        else if(heading == Heading::Down) return State::WalkingDown; // 如果方向是向下，设置状态为向下行走
        else return State::WalkingLeftRight; // 如果方向是向左，设置状态为左右行走
    }
    virtual void death_action() = 0;   // 纯虚函数，用于处理unit死亡后的行为
};