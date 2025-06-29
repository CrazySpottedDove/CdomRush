#pragma once
#include "Model/components/attack.h"
#include "Model/components/damage.h"
#include <cstddef>
#include <vector>
// 范围攻击
class MeleeAttack: public Attack{
public:
    MeleeAttack(DamageData data,double radius = 50.0, double cooldown = 1.0, double chance = 1.0)
        : Attack(cooldown, chance), radius(radius) {
        damage_event = DamageEvent(data, nullptr, nullptr);
    }
    double radius; // 伤害半径
    DamageEvent damage_event;
    void Apply(Store& store, Unit* target) noexcept override {
        // 设置伤害事件
        DamageEvent new_event(damage_event);
        new_event.target = target; // 设置目标

        // 结算伤害
        store.QueueDamageEvent(damage_event);
    }
};

class Melee{
public:
    std::vector<MeleeAttack*> attacks;
    MeleeAttack*& operator[](const std::size_t id){
        return attacks[id];
    }
};
