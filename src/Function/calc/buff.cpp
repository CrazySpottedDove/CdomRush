#include "Function/calc/buff.h"
#include "Model/templates/unit.h"
void calc::damage_inc(Unit& target, const double amount)
{
    target.buff.damage_inc += amount;
}

void calc::damage_dec(Unit& target, const double amount)
{
    target.buff.damage_inc -= amount;
}

void calc::damage_mul(Unit& target, const double factor)
{
    target.buff.damage_factor *= factor;
}

void calc::damage_div(Unit& target, const double factor)
{
    target.buff.damage_factor /= factor;
}

void calc::speed_inc(Unit& target, const double amount)
{
    target.buff.speed_inc += amount;
}

void calc::speed_dec(Unit& target, const double amount)
{
    target.buff.speed_inc -= amount;
}

void calc::speed_mul(Unit& target, const double factor)
{
    target.buff.speed_factor *= factor;
}

void calc::speed_div(Unit& target, const double factor)
{
    target.buff.speed_factor /= factor;
}

void calc::physical_armor_inc(Unit& target, const double amount)
{
    target.buff.physical_armor_inc += amount;
}

void calc::physical_armor_dec(Unit& target, const double amount)
{
    target.buff.physical_armor_inc -= amount;
}

void calc::magical_armor_inc(Unit& target, const double amount)
{
    target.buff.magical_armor_inc += amount;
}

void calc::magical_armor_dec(Unit& target, const double amount)
{
    target.buff.magical_armor_inc -= amount;
}

