#pragma once
#include "ViewModel/GameViewModel/components/damage.h"
#include "ViewModel/GameViewModel/templates/entity.h"
#include "Common/macros.h"
#include "ViewModel/GameViewModel/fx/fx.h"
#include "ViewModel/GameViewModel/Function/calc/damage.h"
#include "Common/type.h"

class Store;


class Bullet: public Entity
{
public:
    Position bullet_offset = {0.0f, 0.0f}; // 子弹起始偏移位置
    double radius = 0.0; // 爆炸半径
    double initial_time = 0.0; // 初始时间
    double totalDuration_ = 0.0; // 总持续时间
    FxType hit_fx = FxType::None; // 击中效果
    DamageEvent damage_event;
    Position source_position;
    Position target_position;
    BulletType bullet_type; // 弹道类型
    SourceType source_type; // 源类型
    bool target_alive = true; // 目标是否存活

    Bullet(const Bullet & other) = default; // 拷贝构造函数
    Bullet() = default; // 默认构造函数
    bool Insert(Store& store) override;
    bool Remove(Store& store) override{
        return true;
    }
    void check_position(Store& store);

    virtual Bullet* Clone() const = 0; // 纯虚函数，用于克隆子弹对象
};

// TODO: 箭矢
// 采用抛物线轨迹
class Arrow : public Bullet
{
public:
    Arrow(ID sourceID = INVALID_ID, ID targetID = INVALID_ID){
        bullet_type = BulletType::Arrow; // 设置弹道类型为箭矢
        damage_event = DamageEvent(DamageData(0, DamageType::Physical, 0, 0), sourceID, targetID);
        totalDuration_ = 1.0; // 设置总持续时间
        hit_fx = FxType::None; // 箭矢没有特殊的击中效果
        radius = 0.0; // 箭矢没有爆炸半径
        animations.push_back(Animation(State::Flying,"arrow"));
    }; // 默认构造函数
    Arrow(const Arrow & other) = default; // 拷贝构造函数
    Bullet* Clone() const override {
        return new Arrow(*this); // 返回一个新的Arrow对象
    }

    sf::Vector2f Bezier(float t,
                           const sf::Vector2f& p0,
                           const sf::Vector2f& p1,
                           const sf::Vector2f& p2) const {
    float u = 1.0f - t;
    return u * u * p0 + 2.f * u * t * p1 + t * t * p2;
    }

    sf::Vector2f GetControlPoint(const Position& p0,const Position& p2) const {
        sf::Vector2f mid = (p0 + p2) * 0.5f;
        mid.y += 100.f; // 控制点向上偏移，形成抛物线
        return mid;
    }

    void Update(Store& store) override;
};

// TODO: 法球
// 采用直线追踪轨迹
class Bolt : public Bullet
{
public:
    Bolt(ID sourceID = INVALID_ID, ID targetID = INVALID_ID){
        bullet_type = BulletType::Bolt; // 设置弹道类型为法球
        damage_event = DamageEvent(DamageData(0.0, DamageType::Magical, 0, 0), sourceID, targetID);
        totalDuration_ = 0.5; // 设置总持续时间
        hit_fx = FxType::None; // 法球没有特殊的击中效果
        radius = 0.0; // 法球没有爆炸半径
        animations.push_back(Animation(State::Flying,"bolt"));
    }
    Bolt(const Bolt & other) = default; // 拷贝构造函数
    Bullet* Clone() const override {
        return new Bolt(*this); // 返回一个新的Arrow对象
    }

    void Update(Store& store) override;
};

// TODO: 炸弹
//需要添加爆炸效果
// 采用抛物线轨迹
class Bomb : public Bullet
{
public:
    Bomb(ID sourceID = INVALID_ID, ID targetID = INVALID_ID){
        bullet_type = BulletType::Bomb; // 设置弹道类型为炸弹
        damage_event = DamageEvent(DamageData(0, DamageType::Explosion, 0, 0), sourceID, targetID);
        totalDuration_ = 1; // 设置总持续时间
        hit_fx = FxType::Explosion; // 爆炸效果
        radius = 60.0; // 设置爆炸半径
        animations.push_back(Animation(State::Flying,"bombs_0001"));
    }
    Bomb(const Bomb & other) = default; // 拷贝构造函数
    Bullet* Clone() const override {
        return new Bomb(*this); // 返回一个新的Arrow对象
    }

    sf::Vector2f Bezier(float t,
                           const sf::Vector2f& p0,
                           const sf::Vector2f& p1,
                           const sf::Vector2f& p2) const {
        float u = 1.0f - t;
        return u * u * p0 + 2.f * u * t * p1 + t * t * p2;
    }

    sf::Vector2f getControlPoint(const Position& p0,const Position& p2) const {
        sf::Vector2f mid = (p0 + p2) * 0.5f;
        mid.y += 75.f; // 控制点向上偏移，形成抛物线
        return mid;
    }

    void Update(Store& store) override ;
};