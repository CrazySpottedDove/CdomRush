#include "enemies.h"
#include "ViewModel/GameViewModel/components/health.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/Function/calc/damage.h"
#include "ViewModel/GameViewModel/soldiers/soldiers.h"

void PassiveEnemy::Update(Store& store)
{
    if (animations[0].current_state == State::Death) return;   // 如果敌人处于死亡状态，跳过更新
    if (this->health.hp <= 0) {
        this->animations[0].current_state = State::Death;   // 如果生命值为0，进入死亡状态
        death_action();   // 执行死亡行为
        health.death_time = store.time;
        store.gold += gold;                     // 增加金币
        return;
    }
    if (is_moving()) {                         // 如果敌人正在移动
        calc::enemy_move_tick(store, *this);   // 调用运动函数更新位置
    }
    animations[0].current_state = walkjudge();   // 根据当前方向设置状态
    return;
}

void ActiveEnemyMelee::Update(Store& store)
{
    if (animations[0].current_state == State::Death) return;   // 如果敌人处于死亡状态，跳过更新
    if (this->health.hp <= 0) {
        this->animations[0].current_state = State::Death;   // 如果生命值为0，进入死亡状态
        death_action();   // 执行死亡行为
        health.death_time = store.time;
        store.gold += gold;                     // 增加金币
        return;
    }
    if (this->animations[0].current_state == State::Idle) {
        heading = Heading::Right;   // 设置方向为向右
        Soldier* Blocker = store.GetSoldier(this->blocker);   // 获取阻挡单位
        if (Blocker == nullptr) {
            this->animations[0].current_state = walkjudge();
            return;
        }
        if (Blocker->animations[0].current_state == State::Death) {
            blocker = INVALID_ID;   // 如果阻挡单位死亡，清除阻挡单位
            return;
        }

        if (Blocker->slot + slot + position != Blocker->position) return;

        for (int i = 0; i < this->melee.attacks.size(); ++i) {
            if (this->melee.attacks[i].IsReady(store)) {
                melee.attacks[i].Apply(store,id,blocker,SourceType::Enemy);   // 执行近战攻击
                this->animations[0].current_state = State::Attack;   // 设置状态为攻击
                return;
            }
        }
        return;
    }
    if (is_moving()) {                         // 如果敌人正在移动
        calc::enemy_move_tick(store, *this);   // 调用运动函数更新位置
        if (blocker != INVALID_ID) animations[0].current_state = State::Idle;   // 如果有阻挡单位，设置状态为闲置
        else animations[0].current_state = walkjudge();         // 根据当前方向设置状态
        return;
    }
    else if (this->animations[0].current_state == State::Attack) {
        if(animations[0].pending == true) return ;
        animations[0].current_state = State::Idle;
        animations[0].pending = true;   // 设置动画为进行中
        return;
    }
    return;
}

bool ActiveEnemyRange::shoot_judge(Store& store){
    bool target_in_range = 0;//是否有目标要射击
    for(int i = 0; i < this->ranged.attacks.size(); ++i) {
        ID target_soldier = calc::find_nearest_soldier(store,position,ranged.attacks[i].range);
        Soldier* target_soldier_ptr = store.GetSoldier(target_soldier);
        if(target_soldier == INVALID_ID) continue;
        if(target_soldier_ptr == nullptr) continue;
        target_in_range = 1;
        if (this->ranged.attacks[i].IsReady(store)) {
            ranged.attacks[i].Apply(store,id,target_soldier,bullet_offset,SourceType::Enemy);   // 执行远程攻击
            this->animations[0].current_state = State::Shoot;   // 设置状态为射击
            return true;
        }
    }
    if(target_in_range) return true;   // 如果有目标在射程内，返回true
    return false;   // 如果没有目标在射程内，返回false
}

void ActiveEnemyRange::Update(Store& store)
{
    if (animations[0].current_state == State::Death) return;   // 如果敌人处于死亡状态，跳过更新
    if (this->health.hp <= 0) {
        this->animations[0].current_state = State::Death;   // 如果生命值为0，进入死亡状态
        death_action();   // 执行死亡行为
        health.death_time = store.time;
        store.gold += gold;                     // 增加金币
        return;
    }

    if(animations[0].current_state == State::Idle) {
        heading = Heading::Right;   // 设置方向为向右
        Soldier* Blocker = store.GetSoldier(this->blocker);   // 获取阻挡单位

        if(Blocker != nullptr && Blocker->slot + slot + position == Blocker->position){
            //走近战路线
            for (int i = 0; i < this->melee.attacks.size(); ++i) {
                if (this->melee.attacks[i].IsReady(store)) {
                    melee.attacks[i].Apply(store,id,blocker,SourceType::Enemy);   // 执行近战攻击
                    this->animations[0].current_state = State::Attack;   // 设置状态为攻击
                    return;
                }
            }
            return ;
        }
        if(Blocker == nullptr){
            if(shoot_judge(store)) return ;
            this->animations[0].current_state = walkjudge();   // 如果没有阻挡单位，也没有射击目标，那就设置状态为行走
            return ;
        }
        if(Blocker->animations[0].current_state == State::Death){
            blocker = INVALID_ID;
            return ;
        }

        if (Blocker->slot + slot + position != Blocker->position) return;


    }

    else if(is_moving()){
        calc::enemy_move_tick(store,*this);
        if(blocker!=INVALID_ID) animations[0].current_state = State::Idle;
        else if(!shoot_judge(store)) animations[0].current_state = walkjudge();
        return ;
    }

    else if (this->animations[0].current_state == State::Attack) {
        if(animations[0].pending == true) return ;
        animations[0].current_state = State::Idle;
        animations[0].pending = true;   // 设置动画为进行中
        return;
    }

    else if (this->animations[0].current_state == State::Shoot) {
        if(animations[0].pending == true) return ;
        animations[0].current_state = State::Idle;
        animations[0].pending = true;   // 设置动画为进行中
        return;
    }

    return ;
}

ForestTroll::ForestTroll(Position position_)
{
    this->health          = Health(4000, 4000);   // 设置生命值
    // this->health = Health(100,100);
    this->armor           = Armor(0, 0);          // 设置护甲
    // this->speed           = 36;                   // 设置速度
    this->speed = 100;
    this->gold            = 200;                  // 设置击杀奖励
    this->life_cost       = 5;                    // 设置生命损失
    this->animations.push_back(Animation(State::Idle,"forest_troll"));
    this->animations[0].anchor_y = 0.79;
    this->melee.attacks.push_back(MeleeAttack(DamageData(100.0, DamageType::Physical, 0.0, 15), 50.0, 1.0, 1.0));
    this->melee[0].damage_event.source = id;
    this->slot     = sf::Vector2f(35.0f, 0.0f);   // 初始化近战偏移
    this->position = position_;                   // 设置初始位置
    this->Hit_offset = sf::Vector2f(0.0f,30.0f);   // 设置受击偏移位置
    this->health_bar_offset = sf::Vector2f(0.0f,76.0f);
}

orc_armored::orc_armored(Position position_)
{
    this->health          = Health(400, 400);   // 设置生命值
    this->armor           = Armor(0.8, 0);          // 设置护甲
    this->speed           = 24;                   // 设置速度
    this->gold            = 30;                  // 设置击杀奖励
    this->life_cost       = 5;                    // 设置生命损失
    this->animations.push_back(Animation(State::Idle,"orc_armored"));
    this->melee.attacks.push_back(MeleeAttack(DamageData(30.0, DamageType::Physical, 0.0, 6), 0.0, 1.0, 1.0));
    this->melee[0].damage_event.source = id;
    this->slot     = sf::Vector2f(18.0f, 0.0f);   // 初始化近战偏移
    this->position = position_;                   // 设置初始位置
    this->Hit_offset = sf::Vector2f(0.0f,14.0f);   // 设置受击偏移位置
    this->health_bar_offset = sf::Vector2f(0.0f,24.0f);
}

orc_wolf_rider::orc_wolf_rider(Position position_)
{
    this->health          = Health(400, 400);   // 设置生命值
    this->armor           = Armor(0, 0.8);          // 设置护甲
    this->speed           = 42;                   // 设置速度
    this->gold            = 25;                  // 设置击杀奖励
    this->life_cost       = 2;                    // 设置生命损失
    this->animations.push_back(Animation(State::Idle,"orc_wolf_rider"));
    this->melee.attacks.push_back(MeleeAttack(DamageData(30.0, DamageType::Physical, 0.0, 9), 0.0, 1.0, 1.5));
    this->melee[0].damage_event.source = id;
    this->slot     = sf::Vector2f(30.0f, 0.0f);   // 初始化近战偏移
    this->position = position_;                   // 设置初始位置
    this->Hit_offset = sf::Vector2f(0.0f,23.0f);   // 设置受击偏移位置
    this->health_bar_offset = sf::Vector2f(0.0f,48.0f);
}