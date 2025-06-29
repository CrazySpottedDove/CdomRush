#pragma once
#include <memory>
#include "Model/enemies/enemies.h"
#include "Model/components/damage.h"
#include "Model/templates/entity.h"
#include "utils/macros.h"
#include "Manager/store/store.h"
#include "Model/fx/fx.h"
#include "Function/calc/damage.h"
#include "Model/templates/unit.h" // 添加以获得Unit的完整类型定义

enum class BulletType
{
    Arrow, // 箭矢
    Bolt,  // 法球
    Bomb   // 炸弹
};
class Bullet: public Entity
{
public:
    double radius; // 爆炸半径
    double initial_time = 0.0; // 初始时间
    double totalDuration_ = 0.0; // 总持续时间
    FxType hit_fx = FxType::None; // 击中效果
    DamageEvent damage_event;
    bool target_alive = true; // 目标是否存活
    Position source_position;
    Position target_position;
    BulletType bullet_type; // 弹道类型

    Bullet(const Bullet & other) = default; // 拷贝构造函数
    Bullet() = default; // 默认构造函数
    bool Insert(Store& store) override{
        target_alive = true; // 初始化目标存活状态
        target_position = damage_event.target->position; // 设置目标位置
        source_position = damage_event.source->position; // 设置源位置
        initial_time = store.time;
        return true;
    }
    bool Remove(Store& store) override{
        return true;
    }

    virtual Bullet* clone() const = 0; // 纯虚函数，用于克隆子弹对象
};

// TODO: 箭矢
// 采用抛物线轨迹
class Arrow : public Bullet
{
public:
    Arrow(){
        bullet_type = BulletType::Arrow; // 设置弹道类型为箭矢
        damage_event = DamageEvent(DamageData(5.5, DamageType::Physical, 0, 0), nullptr, nullptr);
        totalDuration_ = 1.0; // 设置总持续时间
    }; // 默认构造函数
    Arrow(const Arrow & other) = default; // 拷贝构造函数
    Bullet* clone() const override {
        return new Arrow(*this); // 返回一个新的Arrow对象
    }

    sf::Vector2f Arrow::bezier(float t,
                           const sf::Vector2f& p0,
                           const sf::Vector2f& p1,
                           const sf::Vector2f& p2) const {
    float u = 1.0f - t;
    return u * u * p0 + 2.f * u * t * p1 + t * t * p2;
    }

    sf::Vector2f Arrow::getControlPoint(const Position& p0,const Position& p2) const {
        sf::Vector2f mid = (p0 + p2) * 0.5f;
        mid.y -= 100.f; // 控制点向上偏移，形成抛物线
        return mid;
    }

    void Update(Store& store) override {
        // 更新箭矢位置和动画
        if(animation.state != State::Flying) {
            return; // 如果不是飞行状态，则不更新
        }

        if(damage_event.target->animation.state == State::Death){
            target_alive = false; // 如果目标死亡，则不再更新
        }

        float t = (store.time - initial_time) / totalDuration_;
        if (t >= 1.0f) {
            animation.state = State::Hit; // 击中了
            store.QueueDamageEvent(damage_event); // 结算伤害
            return ;
        }

        // 计算控制点
        if(target_alive) target_position = damage_event.target->position; // 更新目标位置
        sf::Vector2f p1 = getControlPoint(source_position, target_position);
        this->position = bezier(t, source_position, p1, target_position);

        return ;
    }
};

// TODO: 法球
// 采用直线追踪轨迹
class Bolt : public Bullet
{
public:
    Bolt(){
        bullet_type = BulletType::Bolt; // 设置弹道类型为法球
        damage_event = DamageEvent(DamageData(15.0, DamageType::Magical, 0, 0), nullptr, nullptr);
        totalDuration_ = 0.5; // 设置总持续时间
    }
    Bolt(const Bolt & other) = default; // 拷贝构造函数

    void Update(Store& store) override {
        // 更新法球位置和动画
        if(animation.state != State::Flying) {
            return; // 如果不是飞行状态，则不更新
        }

        float t = (store.time - initial_time) / totalDuration_;
        if (t >= 1.0f) {
            store.QueueDamageEvent(damage_event); // 结算伤害
            animation.state = State::Hit; // 击中了
            return ;
        }

        // 计算法球位置
        if(target_alive) target_position = damage_event.target->position;
        this->position = source_position + t * (target_position - source_position);
        return ;
    }
};

// TODO: 炸弹
//需要添加爆炸效果
// 采用抛物线轨迹
class Bomb : public Bullet
{
public:
    Bomb(){
        bullet_type = BulletType::Bomb; // 设置弹道类型为炸弹
        damage_event = DamageEvent(DamageData(9.5, DamageType::Explosion, 0, 0), nullptr, nullptr);
        totalDuration_ = 1.5; // 设置总持续时间
        hit_fx = FxType::Explosion; // 爆炸效果
        radius = 60.0; // 设置爆炸半径
    }
    Bomb(const Bomb & other) = default; // 拷贝构造函数

    sf::Vector2f Bomb::bezier(float t,
                           const sf::Vector2f& p0,
                           const sf::Vector2f& p1,
                           const sf::Vector2f& p2) const {
        float u = 1.0f - t;
        return u * u * p0 + 2.f * u * t * p1 + t * t * p2;
    }

    sf::Vector2f getControlPoint(const Position& p0,const Position& p2) const {
        sf::Vector2f mid = (p0 + p2) * 0.5f;
        mid.y -= 75.f; // 控制点向上偏移，形成抛物线
        return mid;
    }

    void Update(Store& store) override {
        // 更新炸弹位置和动画
        if(animation.state != State::Flying) {
            return; // 如果不是飞行状态，则不更新
        }

        float t = (store.time - initial_time) / totalDuration_;
        if (t >= 1.0f) {
            std::vector<Enemy*> enermy = calc::find_enemies_in_range(store,target_position,radius);
            for(auto& target : enermy) {
                if(target->health.hp > 0) { // 如果目标存活
                    DamageEvent event = damage_event; // 创建新的伤害事件
                    event.target = target; // 设置目标
                    store.QueueDamageEvent(event); // 结算伤害
                }
            }
            animation.state = State::Hit; // 击中了
            return ;
        }

        // 计算控制点
        if(target_alive) damage_event.target->position = damage_event.target->position; // 更新目标位置
        sf::Vector2f p1 = getControlPoint(source_position,target_position);
        this->position = bezier(t, source_position, p1, target_position);
        return ;
    }
};