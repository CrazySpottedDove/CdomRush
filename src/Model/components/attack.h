#pragma once
class Unit;
class Store;

class Attack
{
public:
    // virtual void Apply(Store& store, Unit* target) noexcept = 0;
    virtual bool IsReady(const Store& store) const noexcept;

    void SetLastTime(const double time) noexcept ;
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
