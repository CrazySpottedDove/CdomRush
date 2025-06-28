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
};

class Melee{
public:
    std::vector<MeleeAttack*> attacks;
    MeleeAttack& operator[](const std::size_t id){
        return attacks[id];
    }
};
