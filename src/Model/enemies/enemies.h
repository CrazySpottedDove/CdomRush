#pragma once


#include "Model/components/path.h"
#include "Model/templates/unit.h"
#include "Function/calc/motion.h"
#include ""
class Enemy : public Unit//一种无害的敌人
{
public:
    int gold;
    int life_cost;
    PathInfo path_info;

    Enemy() = default; // 默认构造函数

    bool Insert(Store& store) override {
        // 在存储中插入敌人
        return true; // 返回 true 表示插入成功
    }
    bool Remove(Store& store) override {
        // 从存储中移除敌人
        return true; // 返回 true 表示移除成功
    }

};

class PassiveEnemy : public Enemy{
public:
    void update(Store& store) override {
        if(this->health.hp <= 0) {
            this->state = State::Death; // 如果生命值为0，进入死亡状态
            store.gold += gold; // 增加金币
        }
        if (this->state == State::Walk || this->state == State::Idle || this->state == State::WalkingLeftRight || this->state == State::WalkingUp || this->state == State::WalkingDown) {
            move_tick(store, *this); // 调用运动函数更新位置
        }
        if(heading==Heading::Right) state = State::WalkingLeftRight; // 如果方向是向右，设置状态为左右行走
        else if(heading==Heading::Up) state = State::WalkingUp; // 如果方向是向上，设置状态为向上行走
        else if(heading==Heading::Down) state = State::WalkingDown; // 如果方向是向下，设置状态为向下行走
        else state = State::WalkingLeftRight; // 如果方向是向左，设置状态为左右行走
    }
}

class activeEnemy : public Enemy{
public:
    Unit* blocker = nullptr; // 用于阻挡敌人前进的单位
    Melee melee; // 近战攻击组件
    void update(Store& store) override {
        if(this->health.hp <= 0) {
            this->state = State::Death; // 如果生命值为0，进入死亡状态
            store.gold += gold; // 增加金币
            return ;
        }
        // 根据当前状态进行不同的处理
        if (this->state == State::Idle) {
            if (this->blocker == nullptr) {
                this->state = State::Walk; // 如果没有人阻挡，那就继续走
                return ;
            }
            else{
                if(this->blocker->state == State::Death) {
                    this->blocker = nullptr; // 如果阻挡单位死亡，清除阻挡单位
                    return ;
                }
                else {
                    for(int i = 0; i < this->melee.attacks.size(); ++i) {
                        if (this->melee.attacks[i]->IsReady(store)) {
                            this->melee.attacks[i]->Apply(store, this->blocker); // 如果攻击准备好了，就攻击阻挡单位
                            this->melee.attacks[i]->SetLastTime(store.time); // 更新上次攻击时间
                            return ;
                        }
                    }
                }
            }
        }
        else if (this->state == State::Walk) {
            move_tick(store, *this); // 调用运动函数更新位置
        }
        // 其他状态处理...
    }
};

class ForestTroll : public PassiveEnemy{
    ForestTroll() {
        this->health = Health(4000,4000); // 设置生命值
        this->armor = Armor(0,0); // 设置护甲
        this->speed = 36; // 设置速度
        this->gold = 200; // 设置击杀奖励
        this->life_cost = 5; // 设置生命损失
        this->state = State::Idle; // 设置初始状态
    }
};