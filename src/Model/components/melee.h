#pragma once
#include "Model/components/attack.h"
#include "Model/components/damage.h"
#include <cstddef>
#include <vector>
// 范围攻击
enum class SourceType
{
    Enemy,  // 敌人
    Soldier // 士兵
};
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
    std::vector<MeleeAttack*> attacks;
    MeleeAttack*& operator[](const std::size_t id){
        return attacks[id];
    }
};
