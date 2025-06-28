#pragma once
#include <memory>
#include "Model/components/damage.h"
#include "Model/templates/entity.h"
#include "utils/macros.h"
#include "Manager/store/store.h"


class Bullet: public Entity
{
public:
    double initial_time = 0.0; // 初始时间
    double totalDuration_ = 0.0; // 总持续时间
    DamageEvent damage_event;
    virtual std::shared_ptr<Bullet> Clone() const noexcept        = 0;

    virtual Bullet() = 0;

    bool Insert(Store& store) override{
        inital_time = store.time;
        return true;
    }
    bool Remove(Store& store) override{
        return true;
    }
};

// TODO: 箭矢
// 采用抛物线轨迹
class Arrow : public Bullet
{
    Arrow(Entity* source, Entity* target, double value, double ignore_armor, int apply_delay) : position(source->position),
    damage_event(DamageData(5.5,DamageType::Physical,0,0),target,source),totalDuration_(1.0) {
        animation.state = State::Flying;
    }


    sf::Vector2f Arrow::bezier(float t,
                           const sf::Vector2f& p0,
                           const sf::Vector2f& p1,
                           const sf::Vector2f& p2) const {
    float u = 1.0f - t;
    return u * u * p0 + 2.f * u * t * p1 + t * t * p2;
    }

    sf::Vector2f Arrow::getControlPoint() const {
        sf::Vector2f mid = (damage_event.source->position + damage_event.target->position) * 0.5f;
        mid.y -= 100.f; // 控制点向上偏移，形成抛物线
        return mid;
    }
    
    void update(Store& store) override {
        // 更新箭矢位置和动画
        if(animation.state != State::Flying) {
            return; // 如果不是飞行状态，则不更新
        }

        float t = (store.time - initial_time_) / totalDuration_;
        if (t >= 1.0f) {
            animation.state = State::Hit; // 击中了
            return ;
        }
        
        // 计算控制点
        sf::Vector2f p1 = getControlPoint();
        this->position = bezier(t, damage_data.source->position, p1, damage_event.target->position);
        return ;
    }
};

// TODO: 法球
// 采用直线追踪轨迹
class Bolt : public Bullet
{

};

// TODO: 炸弹
// OnHit() 需要添加爆炸效果
// 采用抛物线轨迹
class Bomb : public Bullet
{
    
};