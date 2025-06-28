#pragma once
#include "Model/components/attack.h"
#include "Model/components/damage.h"
#include <cstddef>
#include <vector>
// 范围攻击
class MeleeAttack: public Attack{
public:
    double radius; // 伤害半径
    DamageEvent damage_event;
    void Apply(Store& store, Unit* target) noexcept override {
        // 设置伤害事件
        DamageEvent new_event(damage_event);
        new_event.target = target; // 设置目标
        damage_event.source = this;

        // 结算伤害
        store.QueueDamageEvent(damage_event);
        SetLastTime(store.time); // 更新上次攻击时间
    }
};

class Melee{
public:
    std::vector<MeleeAttack*> attacks;
    MeleeAttack& operator[](const std::size_t id){
        return attacks[id];
    }
};
