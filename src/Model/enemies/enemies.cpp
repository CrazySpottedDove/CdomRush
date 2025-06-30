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
                std::vector<ID> targets = calc::find_soldiers_in_range(
                    store, Blocker->position, this->melee.attacks[i]->radius);
                if (std::find(targets.begin(), targets.end(), this->blocker) == targets.end())
                    continue;
                for (auto& id : targets) {
                    Soldier* target = store.GetSoldier(id);   // 获取目标单位
                    if (target->health.hp <= 0) continue;           // 如果目标已经死亡，跳过
                    this->melee.attacks[i]->Apply(store, id);   // 应用攻击
                }
                this->melee.attacks[i]->SetLastTime(store.time);   // 更新上次攻击时间
                this->animation.state = State::Attack;             // 攻击状态
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
    this->animation.prefix = "aura_forest_troll_regen";
    this->animation.state = State::Idle;   // 设置动画状态为闲置
}
