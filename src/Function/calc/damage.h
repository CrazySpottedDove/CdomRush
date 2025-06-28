
#pragma once
#include "Model/components/damage.h"
class Unit;
namespace calc{
    double calc_damage(const DamageEvent& damage_event);
    void receive_damage(Unit& target, const double amount);
}