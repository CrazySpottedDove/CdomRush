#include "Function/calc/hp.h"
#include "Model/components/state.h"
#include "Model/templates/unit.h"
#include "Manager/store/store.h"

void calc::heal(Unit& target, const double amount){
    auto& health = target.health;
    health.hp += amount;
    if (health.hp > health.hp_max){
        health.hp = health.hp_max;
    }
}

bool calc::ready_to_regen(const Store& store, const Unit& self){
    return store.time - self.health.last_regen_time > self.health.regen_cooldown;
}

void calc::regen(const Store& store, Unit& self){
    auto& health = self.health;
    calc::heal(self, health.regen_factor * health.hp_max);
    health.last_regen_time = store.time;
}

bool calc::is_dead(const Unit& self){
    return self.health.hp <= 0 || self.animation.state == State::Death;
}

bool calc::should_remove(const Store& store, const Unit& self){
    return store.time - self.health.death_time > self.health.dead_lifetime;
}