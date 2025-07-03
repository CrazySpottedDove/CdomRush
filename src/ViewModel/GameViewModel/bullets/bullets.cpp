#include "ViewModel/GameViewModel/bullets/bullets.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/soldiers/soldiers.h"
#include <cmath>

void Bullet::check_position(Store& store)
{
    // 检查目标是否存活
    if(source_type == SourceType::Tower || source_type == SourceType::Soldier){
        Enemy* target = store.GetEnemy(damage_event.target);   // 获取目标敌人
        if (target->animations[0].current_state == State::Death) target_alive = false;   // 如果目标死亡，则不再更新
        else target_position = target->position + target->Hit_offset;   // 更新目标位置
    }
    if(source_type == SourceType::Enemy){
        Soldier* target = store.GetSoldier(damage_event.target);   // 获取目标士兵
        if (target->animations[0].current_state == State::Death) target_alive = false;   // 如果目标死亡，则不再更新
        else target_position = target->position + target->Hit_offset;   // 更新目标位置
    }
}

bool Bullet::Insert(Store& store)
{
    if(source_type == SourceType::Enemy){
        Enemy* source = store.GetEnemy(damage_event.source);   // 获取源单位
        Unit* target = store.GetSoldier(damage_event.target); // 获取目标单位
        source_position = source->position + bullet_offset; // 设置源位置
        target_position = target->position + target->Hit_offset; // 设置目标位置
        return true;
    }

    if(source_type == SourceType::Tower){
        Tower* source = store.GetTower(damage_event.source);   // 获取源塔
        Unit* target = store.GetEnemy(damage_event.target); // 获取目标敌人
        source_position = source->position + bullet_offset; // 设置源位置
        target_position = target->position + target->Hit_offset; // 设置目标位置
        return true;
    }
    if(source_type == SourceType::Soldier){
        Soldier* source = store.GetSoldier(damage_event.source);   // 获取源士兵
        Unit* target = store.GetEnemy(damage_event.target); // 获取目标敌人
        source_position = source->position + bullet_offset;   // 设置源位置
        target_position = target->position + target->Hit_offset; // 设置目标位置
        return true;
    }

    return false;   // 如果源和目标都不存在，则返回 false
}

void Arrow::Update(Store& store)
{
    // 更新箭矢位置和动画
    if(animations[0].current_state == State::Hit) return ;   // 如果已经击中，则不再更新

    float t = (store.time - initial_time) / totalDuration_;
    if (t >= 1.0f) {
        animations[0].current_state = State::Hit;           // 击中了
        store.QueueDamageEvent(damage_event);   // 结算伤害
        return;
    }

    // 检查目标是否存活
    check_position(store);

    // 计算控制点
    sf::Vector2f p1 = GetControlPoint(source_position, target_position);
    this->position  = Bezier(t, source_position, p1, target_position);
    sf::Vector2f dBezier = 2.0f * (1.0f - t) * (p1 - source_position) + 2.0f * t * (target_position - p1);
    float angle_radians = std::atan2(dBezier.y, dBezier.x);
    float angle_degrees = angle_radians * 180.0f / 3.14159265f;

    this->animations[0].rotation = angle_degrees;
    return ;
}

void Bolt::Update(Store& store)
{
    // 更新法球位置和动画
    if(animations[0].current_state == State::Hit) return ;   // 如果已经击中，则不再更新

    float t = (store.time - initial_time) / totalDuration_;
    if (t >= 1.0f) {
        store.QueueDamageEvent(damage_event);   // 结算伤害
        animations[0].current_state = State::Hit;           // 击中了
        return;
    }

    // 检查目标是否存活
    check_position(store);

    // 计算法球位置
    this->position = source_position + t * (target_position - source_position);
    return;
}

void Bomb::Update(Store& store)
{
    // 更新炸弹位置和动画
    if(animations[0].current_state == State::Hit) return ;   // 如果已经击中，则不再更新

    float t = (store.time - initial_time) / totalDuration_;
    if (t >= 1.0f && (source_type == SourceType::Tower || source_type == SourceType::Soldier)) {
        std::vector<ID> enemy = calc::find_enemies_in_range(store, target_position, radius);
        for (auto& id : enemy) {
            DamageEvent event = damage_event;   // 创建新的伤害事件
            event.target      = id;         // 设置目标
            store.QueueDamageEvent(event);      // 结算伤害
        }
        animations[0].current_state = State::Hit;   // 击中了
        return;
    }

    if(t >= 1.0f && source_type == SourceType::Enemy) {
        std::vector<ID> soldier = calc::find_soldiers_in_range(store, target_position, radius);
        for (auto& id : soldier) {
            DamageEvent event = damage_event;   // 创建新的伤害事件
            event.target      = id;         // 设置目标
            store.QueueDamageEvent(event);      // 结算伤害
        }
        animations[0].current_state = State::Hit;   // 击中了
        return;
    }

    check_position(store);   // 检查目标是否存活

    // 计算控制点
    sf::Vector2f p1 = getControlPoint(source_position, target_position);
    this->position  = Bezier(t, source_position, p1, target_position);
    sf::Vector2f dBezier = 2.0f * (1.0f - t) * (p1 - source_position) + 2.0f * t * (target_position - p1);
    float angle_radians = std::atan2(dBezier.y, dBezier.x);
    float angle_degrees = angle_radians * 180.0f / 3.14159265f;

    this->animations[0].rotation = angle_degrees;
    return;
}