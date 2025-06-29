#pragma once

#include <vector>
#include <algorithm>
#include "Model/components/path.h"
#include "Model/templates/unit.h"
#include "Function/calc/motion.h"
#include "Function/calc/hp.h"
#include "Function/calc/damage.h"
#include "Model/components/melee.h"
enum class EnemyType
{
    PassiveEnemy, // 无害敌人
    ActiveEnemy_Melee, // 近战敌人
    ActiveEnemy_Range,  // 远程敌人
    None
};
class Enemy : public Unit//默认的敌人有这些东西
{
public:
    EnemyType type; // 敌人类型
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

class PassiveEnemy : public Enemy{//无害敌人
public:
    PassiveEnemy(){
        type = EnemyType::PassiveEnemy; // 设置敌人类型为无害敌人
    }
    void Update(Store& store) override {
        if(state==State::Death) return; // 如果敌人处于死亡状态，跳过更新
        if(this->health.hp <= 0) {
            this->state = State::Death; // 如果生命值为0，进入死亡状态
            store.gold += gold; // 增加金币
            return ;
        }
        if (is_moving()) { // 如果敌人正在移动
            calc::move_tick(store, *this); // 调用运动函数更新位置
        }
        state= walkjudge(); // 根据当前方向设置状态
        return ;
    }
};

class ActiveEnemy_Melee : public Enemy{
public:
    ActiveEnemy_Melee() {
        type = EnemyType::ActiveEnemy_Melee; // 设置敌人类型为近战敌人
    }
    Unit* blocker = nullptr; // 用于阻挡敌人前进的单位
    Melee melee; // 近战攻击组件

    void Update(Store& store) override {
        if(state==State::Death) return; // 如果敌人处于死亡状态，跳过更新
        if(calc::is_dead(*this)) {
            this->state = State::Death; // 如果生命值为0，进入死亡状态
            store.gold += gold; // 增加金币
            return ;
        }
        if (this->state == State::Idle) {
            if (this->blocker == nullptr) {
                this->state = walkjudge();
                return ;
            }
            if(this->blocker->state == State::Death) {
                this->blocker = nullptr; // 如果阻挡单位死亡，清除阻挡单位
                return ;
            }
            for(int i = 0; i < this->melee.attacks.size(); ++i) {
                if (this->melee.attacks[i]->IsReady(store)) {
                    std::vector<Unit*> targets = calc::find_soldiers_in_range(store, this->position, this->melee.attacks[i]->radius);
                    if(std::find(targets.begin(),targets.end(),this->blocker) == targets.end()) continue;
                    for(auto& target : targets) {
                        if(target->health.hp <= 0) continue; // 如果目标已经死亡，跳过
                        this->melee.attacks[i]->Apply(store, target); // 应用攻击
                    }
                    this->melee.attacks[i]->SetLastTime(store.time); // 更新上次攻击时间
                    this->state = State::Attack; // 攻击状态
                    return ;
                }
            }
            return ;
        }
        if (is_moving()) { // 如果敌人正在移动
            calc::move_tick(store, *this); // 调用运动函数更新位置
            state = walkjudge(); // 根据当前方向设置状态
            if(this->blocker != nullptr) state = State::Idle; // 如果有阻挡单位，设置状态为闲置
            return ;
        }
        else if (this->state == State::Attack){
            if(animation.pending==0) state = State::Idle;
            return ;
        }
    }
};

class ForestTroll : public ActiveEnemy_Melee{
    public:
    ForestTroll();
}