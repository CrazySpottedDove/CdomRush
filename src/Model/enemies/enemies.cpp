#include "enemies.h"
#include "Manager/store/store.h"

void PassiveEnemy::Update(Store& store)
{
    if (animation.state == State::Death) return;   // 如果敌人处于死亡状态，跳过更新
    if (this->health.hp <= 0) {
        this->animation.state = State::Death;   // 如果生命值为0，进入死亡状态
        store.gold += gold;                     // 增加金币
        return;
    }
    if (is_moving()) {                         // 如果敌人正在移动
        calc::enemy_move_tick(store, *this);   // 调用运动函数更新位置
    }
    animation.state = walkjudge();   // 根据当前方向设置状态
    return;
}

void ActiveEnemyMelee::Update(Store& store)
{
    if (animation.state == State::Death) return;   // 如果敌人处于死亡状态，跳过更新
    if (calc::is_dead(*this)) {
        this->animation.state = State::Death;   // 如果生命值为0，进入死亡状态
        store.gold += gold;                     // 增加金币
        return;
    }
    if (this->animation.state == State::Idle) {
        heading = Heading::Right;   // 设置方向为向右
        Soldier* Blocker = store.GetSoldier(this->blocker);   // 获取阻挡单位
        if (Blocker == nullptr) {
            this->animation.state = walkjudge();
            return;
        }
        if (Blocker->animation.state == State::Death) {
            blocker = INVALID_ID;   // 如果阻挡单位死亡，清除阻挡单位
            return;
        }

        if (Blocker->slot + slot + position != Blocker->position) return;

        for (int i = 0; i < this->melee.attacks.size(); ++i) {
            if (this->melee.attacks[i]->IsReady(store)) {
                melee.attacks[i]->Apply(store,id,blocker,SourceType::Enemy);   // 执行近战攻击
                this->animation.state = State::Attack;   // 设置状态为攻击
                return;
            }
        }
        return;
    }
    if (is_moving()) {                         // 如果敌人正在移动
        calc::enemy_move_tick(store, *this);   // 调用运动函数更新位置
        animation.state = walkjudge();         // 根据当前方向设置状态
        if (blocker != INVALID_ID)
            animation.state = State::Idle;   // 如果有阻挡单位，设置状态为闲置
        return;
    }
    else if (this->animation.state == State::Attack) {
        if (animation.pending == false) animation.state = State::Idle;
        animation.pending = true;   // 设置动画为进行中
        return;
    }
    return;
}


ForestTroll::ForestTroll(Position position_)
{
    this->health          = Health(4000, 4000);   // 设置生命值
    this->armor           = Armor(0, 0);          // 设置护甲
    this->speed           = 36;                   // 设置速度
    this->gold            = 200;                  // 设置击杀奖励
    this->life_cost       = 5;                    // 设置生命损失
    this->animation.state = State::Idle;          // 设置初始状态
    this->melee.attacks.push_back(
        new MeleeAttack(DamageData(100.0, DamageType::Physical, 0.0, 0), 50.0, 1.0, 1.0));
    this->melee[0]->damage_event.source = id;
    this->slot     = sf::Vector2f(35.0f, 0.0f);   // 初始化近战偏移
    this->position = position_;                   // 设置初始位置
    this->animation.prefix = "forest_troll";
    this->animation.state = State::Idle;   // 设置动画状态为闲置
    this->Hit_offset = sf::Vector2f(30.0f,0.0f);   // 设置受击偏移位置
}

orc_armored::orc_armored(Position position_)
{
    this->health          = Health(400, 400);   // 设置生命值
    this->armor           = Armor(0.8, 0);          // 设置护甲
    this->speed           = 24;                   // 设置速度
    this->gold            = 30;                  // 设置击杀奖励
    this->life_cost       = 5;                    // 设置生命损失
    this->animation.state = State::Idle;          // 设置初始状态
    this->melee.attacks.push_back(
        new MeleeAttack(DamageData(30.0, DamageType::Physical, 0.0, 0), 0.0, 1.0, 1.0));
    this->melee[0]->damage_event.source = id;
    this->slot     = sf::Vector2f(18.0f, 0.0f);   // 初始化近战偏移
    this->position = position_;                   // 设置初始位置
    this->animation.prefix = "enemy_orc_armored";
    this->animation.state = State::Idle;   // 设置动画状态为闲置
    this->Hit_offset = sf::Vector2f(0.0f,14.0f);   // 设置受击偏移位置
}