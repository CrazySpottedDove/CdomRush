#include "Model/components/ranged.h"
#include "Manager/store/store.h"

void RangedAttack::Apply(Store& store, ID source, ID target) noexcept
{
    Bullet* bullet =
        (store.template_manager.bullet_map[bullet_type])->Clone();   // 获取对应类型的子弹
    bullet->damage_event.target     = target;                        // 设置目标
    bullet->radius                  = radius;                        // 设置爆炸半径
    bullet->damage_event.data.value = value;                         // 设置伤害值
    bullet->animation.state         = State::Flying;                 // 设置状态为飞行
    bullet->damage_event.source     = source;                        // 设置源为目标单位
    store.QueueBullet(bullet);                                       // 将子弹加入存储

    SetLastTime(store.time);   // 更新上次攻击时间
}