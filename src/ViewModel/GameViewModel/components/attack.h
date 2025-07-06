#pragma once
#include <string>
class Unit;
class Store;

class Attack
{
public:
    std::string attack_sound;
    virtual bool IsReady(const Store& store) const noexcept;

    void SetLastTime(const double time) noexcept ;
    Attack() = default;
    Attack(double cooldown, double chance = 1.0, std::string attack_sound_ = "")
        : cooldown(cooldown)
        , chance(chance)
        , last_time(0)
        , attack_sound(attack_sound_) {};
    virtual ~Attack() = default;

private:
    double cooldown;
    double last_time = 0.0;
    double chance    = 1.0;
};
