#pragma once
#include "Model/bullets/bullets.h"
#include "Model/components/attack.h"
#include "Manager/store/store.h"
#include <cstddef>
#include <vector>

class RangedAttack : public Attack
{
public:
    double value; // 伤害值
    double radius; // 爆炸半径
    double  range;   // 射程
    BulletType bullet_type; // 弹道类型
    RangedAttack(double cooldown, double range, BulletType bullet_type_, double radius_, double value_, double chance = 1.0)
        : Attack(cooldown, chance)
        , range(range)
        , bullet_type(bullet_type_)
        , radius(radius_)
        , value(value_){};
    

    void Apply(Store& store, Unit* target, ActiveEntity* source) noexcept {
        Bullet *bullet = (store.template_manager.bullet_map[bullet_type])->clone(); // 获取对应类型的子弹
        bullet->damage_event.target = target; // 设置目标
        bullet->radius = radius; // 设置爆炸半径
        bullet->damage_event.data.value = value; // 设置伤害值
        bullet->animation.state = State::Flying; // 设置状态为飞行
        bullet->damage_event.source = source; // 设置源为目标单位
        store.QueueBullet(bullet); // 将子弹加入存储
    }
};

class Ranged
{
public:
    std::vector<RangedAttack> attacks;
    RangedAttack&             operator[](const std::size_t idx) { return attacks[idx]; }
};