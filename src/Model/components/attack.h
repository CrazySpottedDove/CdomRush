#pragma once
class Unit;
#include "Manager/store/store.h"
#include "damage.h"
class Attack
{
public:
    // virtual void Apply(Store& store, Unit* target) noexcept = 0;
    virtual bool IsReady(const Store& store) const noexcept
    {
        return (store.time - last_time) >= cooldown;
    }
    void SetLastTime(const double time) noexcept { last_time = time; }
    Attack() = default;
    Attack(double cooldown, double chance = 1.0)
        : cooldown(cooldown)
        , chance(chance)
        , last_time(0) {};
    virtual ~Attack() = default;

private:
    double cooldown;
    double last_time = 0.0;
    double chance    = 1.0;
};
