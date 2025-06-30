#include "Model/bullets/bullets.h"
#include "Manager/store/store.h"
#include <cmath>

bool Bullet::Insert(Store& store)
{
    target_alive    = true;                            // 初始化目标存活状态
    Unit* target = store.GetEnemy(damage_event.target); // 获取目标单位
    if(target==nullptr) target = store.GetSoldier(damage_event.target); // 如果目标是敌人，则获取士兵
    target_position = target->position;   // 设置目标位置

    Unit* source = store.GetEnemy(damage_event.source); // 获取源单位
    if(source == nullptr) source = store.GetSoldier(damage_event.source); // 如果源是敌人，则获取士兵
    source_position = source->position;   // 设置源位置

    initial_time    = store.time;
    return true;
}

void Arrow::Update(Store& store)
{
    // 更新箭矢位置和动画
    if (animation.state != State::Flying) {
        return;   // 如果不是飞行状态，则不更新
    }

    if(target_alive)
    if (damage_event.target->animation.state == State::Death) {
        target_alive = false;   // 如果目标死亡，则不再更新
    }

    float t = (store.time - initial_time) / totalDuration_;
    if (t >= 1.0f) {
        animation.state = State::Hit;           // 击中了
        store.QueueDamageEvent(damage_event);   // 结算伤害
        return;
    }

    // 计算控制点
    if (target_alive) target_position = damage_event.target->position;   // 更新目标位置
    sf::Vector2f p1 = GetControlPoint(source_position, target_position);
    this->position  = Bezier(t, source_position, p1, target_position);
    sf::Vector2f dBezier = 2.0f * (1.0f - t) * (p1 - source_position) + 2.0f * t * (target_position - p1);
    float angle_radians = std::atan2(dBezier.y, dBezier.x);
    float angle_degrees = angle_radians * 180.0f / 3.14159265f;

    this->animation.rotation = angle_degrees;
    return ;
}

void Bolt::Update(Store& store)
{
    // 更新法球位置和动画
    if (animation.state != State::Flying) {
        return;   // 如果不是飞行状态，则不更新
    }

    float t = (store.time - initial_time) / totalDuration_;
    if (t >= 1.0f) {
        store.QueueDamageEvent(damage_event);   // 结算伤害
        animation.state = State::Hit;           // 击中了
        return;
    }

    if(target_alive)
    if (damage_event.target->animation.state == State::Death) {
        target_alive = false;   // 如果目标死亡，则不再更新
    }

    // 计算法球位置
    if (target_alive) target_position = damage_event.target->position;
    this->position = source_position + t * (target_position - source_position);
    return;
}

void Bomb::Update(Store& store)
{
    // 更新炸弹位置和动画
    if (animation.state != State::Flying) {
        return;   // 如果不是飞行状态，则不更新
    }

    float t = (store.time - initial_time) / totalDuration_;
    if (t >= 1.0f) {
        std::vector<ID> enemy = calc::find_enemies_in_range(store, target_position, radius);
        for (auto& id : enemy) {
            Enemy* target = store.GetEnemy(id);   // 获取目标敌人
            if (target->health.hp > 0) {            // 如果目标存活
                DamageEvent event = damage_event;   // 创建新的伤害事件
                event.target      = target;         // 设置目标
                store.QueueDamageEvent(event);      // 结算伤害
            }
        }
        animation.state = State::Hit;   // 击中了
        return;
    }

    if(target_alive)
    if (damage_event.target->animation.state == State::Death) {
        target_alive = false;   // 如果目标死亡，则不再更新
    }

    // 计算控制点
    if (target_alive)
        damage_event.target->position = damage_event.target->position;   // 更新目标位置
    sf::Vector2f p1 = getControlPoint(source_position, target_position);
    this->position  = Bezier(t, source_position, p1, target_position);
    return;
}