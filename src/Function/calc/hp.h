#pragma once
#include "Model/components/health.h"
class Unit;
namespace calc {
    void heal(Unit& target, const double amount);
    void regen(Unit& target);
    bool is_dead(const Unit& target);

}