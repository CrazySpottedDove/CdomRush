#pragma once
#include "ViewModel/GameViewModel/components/attack.h"
#include "ViewModel/GameViewModel/components/damage.h"
#include <cstddef>
#include <vector>
// 范围攻击

class MeleeAttack: public Attack{
public:
    MeleeAttack(DamageData data,ID sourceID = INVALID_ID,double radius = 50.0, double cooldown = 1.0, double chance = 1.0)
        : Attack(cooldown, chance), radius(radius) {
        damage_event = DamageEvent(data, sourceID, INVALID_ID);
    }
    double radius; // 伤害半径
    DamageEvent damage_event;
    void Apply(Store& store, ID source, ID target, SourceType type) noexcept;
};

class Melee{
public:
    std::vector<MeleeAttack> attacks;
    MeleeAttack& operator[](const std::size_t id){
        return attacks[id];
    }
};
