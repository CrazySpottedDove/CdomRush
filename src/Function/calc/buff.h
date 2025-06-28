#pragma once
#include "Model/components/buff.h"
class Unit;
namespace calc {
    void damage_inc(Unit& target, const double amount);
    void damage_dec(Unit& target, const double amount);
    void damage_mul(Unit& target, const double factor);
    void damage_div(Unit& target, const double factor);
    void speed_inc(Unit& target, const double amount);
    void speed_dec(Unit& target, const double amount);
    void speed_mul(Unit& target, const double factor);
    void speed_div(Unit& target, const double factor);
    void physical_armor_inc(Unit& target, const double amount);
    void physical_armor_dec(Unit& target, const double amount);
    void magical_armor_inc(Unit& target, const double amount);
    void magical_armor_dec(Unit& target, const double amount);
}