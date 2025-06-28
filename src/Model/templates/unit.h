#pragma once
#include "../components/armor.h"
#include "../components/buff.h"
#include "../components/health.h"
#include "../components/state.h"
#include "entity.h"
#include "utils/macros.h"

// 拥有生命的实体
class Unit : public Entity
{
public:
    Health       health;
    Armor        armor;
    Buff         buff;
    Position     position;
    double       speed;
    State        state;
    virtual bool OnDamage() { return true; };
    virtual ~Unit() = default;
};