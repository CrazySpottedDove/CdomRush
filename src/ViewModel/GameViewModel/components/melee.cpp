#include "ViewModel/GameViewModel/components/melee.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/soldiers/soldiers.h"

void MeleeAttack::Apply(Store& store, ID source, ID target, SourceType type) noexcept
{
    std::vector<ID> targets;
    if(radius <=0.01){
        targets.push_back(target); // 只攻击目标本身
    }
    else if(type == SourceType::Enemy){
        if(store.GetSoldier(target) == nullptr) return; // 如果目标已经不在了，直接返回
        targets = calc::find_soldiers_in_range(store, store.GetSoldier(target)->position, radius);
    }
    else{
        if(store.GetEnemy(target) == nullptr) return; // 如果目标已经不在了，直接返回
        targets = calc::find_enemies_in_range(store, store.GetEnemy(target)->position, radius);
    }

    // 设置伤害事件
    for(auto id : targets) {
        DamageEvent new_event(damage_event);
        new_event.target = id;   // 设置目标
        store.QueueDamageEvent(new_event);
    }
}