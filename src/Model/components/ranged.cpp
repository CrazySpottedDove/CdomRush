#include "Model/components/ranged.h"
#include "Manager/store/store.h"

void RangedAttack::Apply(Store& store, ID source, ID target, std::string prefix_) noexcept
{
    Bullet* bullet = store.template_manager.CreateBullet(bullet_type);                // 获取对应类型的子弹
    bullet->damage_event.source     = source;                        // 设置源为目标单位
    bullet->damage_event.target     = target;                        // 设置目标
    bullet->radius                  = radius;                        // 设置爆炸半径
    bullet->damage_event.data.value = value;                         // 设置伤害值
    bullet->animation.state         = State::Flying;                 // 设置状态为飞行
    bullet->bullet_offset           = bullet_start_offset;           // 设置子弹起始偏移位置
    bullet->animation.prefix = prefix_; // 设置动画前缀
    store.QueueBullet(bullet);                                       // 将子弹加入存储

    SetLastTime(store.time);   // 更新上次攻击时间
}