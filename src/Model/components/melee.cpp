#include "Model/components/melee.h"
#include "Manager/store/store.h"

void MeleeAttack::Apply(Store& store, ID target) noexcept
{
    // 设置伤害事件
    DamageEvent new_event(damage_event);
    new_event.target = target;   // 设置目标

    // 结算伤害
    store.QueueDamageEvent(new_event);
}