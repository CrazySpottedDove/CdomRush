#pragma once

#include "../bullets/bullets.h"
#include "damage.h"
#include "random.h"
#include "../store/store.h"
class Bullet;
#include "unit.h"
#include <cstddef>
#include <memory>

class Attack {
public:
    virtual void Apply(Store& store, std::shared_ptr<Unit> target) noexcept = 0;
    inline bool IsReady(const Store& store) const noexcept{
        return (store.time - last_time) >= cooldown && chance >= rand01();
    }
    inline void SetLastTime(const double time) noexcept{
        last_time = time;
    }
    Damage damage;
    Attack(const Damage& damage, double cooldown, double chance = 1.0,
           double last_time = 0.0) noexcept
        : damage(damage)
        , cooldown(cooldown)
        , chance(chance)
        , last_time(last_time)
    {}
private:
    double cooldown;
    double last_time = 0.0;
    double chance = 1.0;
};

// TODO: 近战攻击
class MeleeAttack: public Attack {
public:
    MeleeAttack(const Damage& damage, double cooldown, double chance = 1.0,
                double last_time = 0.0) noexcept
        : Attack(damage, cooldown, chance, last_time)
    {}
    void Apply(Store& store, std::shared_ptr<Unit> target) noexcept override;
};

// TODO: 远程攻击
class RangedAttack: public Attack {
public:
    RangedAttack(const Damage& damage, double cooldown, double chance = 1.0,
                 double last_time = 0.0, std::shared_ptr<Bullet> bullet = nullptr) noexcept
        : Attack(damage, cooldown, chance, last_time)
        , bullet(bullet)
    {}
    void Apply(Store& store, std::shared_ptr<Unit> target) noexcept override;
    std::shared_ptr<Bullet> bullet;
};

