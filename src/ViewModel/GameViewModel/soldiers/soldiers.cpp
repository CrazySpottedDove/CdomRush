#include "soldiers.h"
#include "Common/macros.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/templates/unit.h"
#include "ViewModel/GameViewModel/towers/towers.h"


void SoldierMelee::Update(Store& store)
{
    if (rally_point != last_rally_point) {
        if (target_enemy != INVALID_ID) {
            ActiveEnemy* enemy_ptr = dynamic_cast<ActiveEnemy*>(store.GetEnemy(target_enemy));
            if (enemy_ptr != nullptr) enemy_ptr->blocker = INVALID_ID;
            target_enemy = INVALID_ID;
        }
        calc::soldier_move_tick(store, *this, rally_point + rally_point_offset), animations[0].current_state = walkjudge();
        last_rally_point = rally_point;
        return;
    }
    last_rally_point = rally_point;
    Barrack* source  = dynamic_cast<Barrack*>(store.GetTower(source_barrack));
    if (source == nullptr) return;
    if (animations[0].current_state == State::Death) return;
    if (health.hp <= 0) {
        animations[0].current_state = State::Death;
        death_action(store);
        health.death_time = store.time;
        return;
    }

    if (animations[0].current_state == State::Idle) {
        if (target_enemy != INVALID_ID) {
            Enemy* enemy_ptr = store.GetEnemy(target_enemy);
            if (enemy_ptr == nullptr) {
                target_enemy = INVALID_ID;
                return;
            }
            if (enemy_ptr->animations[0].current_state == State::Death) {
                target_enemy = INVALID_ID;
                return;
            }

            if (enemy_ptr->slot + slot + enemy_ptr->position != position) {
                calc::soldier_move_tick(store, *this, enemy_ptr->slot + slot + enemy_ptr->position);
                animations[0].current_state = walkjudge();
                return;
            }
            else {
                animations[0].flip = true;
                for (int i = 0; i < melee.attacks.size(); i++) {
                    if (this->melee.attacks[i].IsReady(store)) {
                        melee.attacks[i].Apply(
                            store, id, target_enemy, SourceType::Soldier);   // 执行近战攻击
                        this->animations[0].current_state = State::Attack;   // 设置状态为攻击
                        return;
                    }
                }
                return;
            }
        }
        else {
            if ((rally_point - position).lengthSquared() >
                source->rally_range * source->rally_range) {
                health.last_regen_time = store.time;
                calc::soldier_move_tick(store, *this, rally_point + rally_point_offset);
                animations[0].current_state = walkjudge();
                return;
            }
            target_enemy = calc::find_foremost_enemy(store, rally_point, range, true);
            if (target_enemy != INVALID_ID) {
                ActiveEnemy* enemy_ptr = dynamic_cast<ActiveEnemy*>(store.GetEnemy(target_enemy));
                enemy_ptr->blocker     = id;
                INFO("SoldierMelee::Update: found enemy " << target_enemy);
                return;
            }
            if (position != rally_point + rally_point_offset) {
                health.last_regen_time = store.time;
                calc::soldier_move_tick(store, *this, rally_point + rally_point_offset);
                animations[0].current_state = walkjudge();
                return;
            }
            if (position == rally_point + rally_point_offset) {
                if (calc::ready_to_regen(store, *this)) calc::regen(store, *this);
            }
        }
        return;
    }

    if (is_moving()) {
        if (target_enemy == INVALID_ID) {
            if (position == rally_point + rally_point_offset)
                animations[0].current_state = State::Idle;
            else
                calc::soldier_move_tick(store, *this, rally_point + rally_point_offset),
                    animations[0].current_state = walkjudge();
            return;
        }
        else {
            Enemy* enemy_ptr = store.GetEnemy(target_enemy);
            if (position == enemy_ptr->slot + slot + enemy_ptr->position)
                animations[0].current_state = State::Idle;
            else
                calc::soldier_move_tick(store, *this, enemy_ptr->slot + slot + enemy_ptr->position),
                    animations[0].current_state = walkjudge();
            return;
        }
    }

    if (animations[0].current_state == State::Attack) {
        if (animations[0].pending == true) return;
        animations[0].current_state = State::Idle;
        animations[0].pending       = true;   // 设置动画为进行中
        return;
    }

    return;
}

SoldierMeleelv1::SoldierMeleelv1(Position position_, Position rally_point_, Position offset_)
{
    position             = position_;
    rally_point          = rally_point_;
    rally_point_offset   = offset_;
    range                = 60;
    speed                = 75;
    health               = Health(50, 50);
    health.dead_lifetime = 10;
    health_bar_offset    = Position(0, 25.16);
    animations.push_back(Animation(State::Idle, "soldier_lvl1"));
    animations[0].anchor_y = 0.83;
    melee.attacks.push_back(MeleeAttack(
        DamageData(2.0, DamageType::Physical, 0.0, 5), -1, 60.0, 1.0, 1.0, "MeleeSword"));
    melee[0].damage_event.source = id;
    slot                         = sf::Vector2f(5.0f, 0.0f);    // 初始化近战偏移
    Hit_offset                   = sf::Vector2f(0.0f, 12.0f);   // 设置受击偏移位置
    heading                      = Heading::None;
}

SoldierMeleelv2::SoldierMeleelv2(Position position_, Position rally_point_, Position offset_)
{
    position             = position_;
    rally_point          = rally_point_;
    rally_point_offset   = offset_;
    speed                = 75;
    range                = 60;
    health               = Health(100, 100);
    armor                = Armor(0.15, 0);
    health.dead_lifetime = 10;
    health_bar_offset    = Position(0, 25.16);
    animations.push_back(Animation(State::Idle, "soldier_lvl2"));
    animations[0].anchor_y = 0.83;
    melee.attacks.push_back(MeleeAttack(
        DamageData(4.0, DamageType::Physical, 0.0, 5), -1, 60.0, 1.3, 1.0, "MeleeSword"));
    melee[0].damage_event.source = id;
    slot                         = sf::Vector2f(5.0f, 0.0f);    // 初始化近战偏移
    Hit_offset                   = sf::Vector2f(0.0f, 12.0f);   // 设置受击偏移位置
    heading                      = Heading::None;
}

SoldierMeleelv3::SoldierMeleelv3(Position position_, Position rally_point_, Position offset_)
{
    position             = position_;
    rally_point          = rally_point_;
    rally_point_offset   = offset_;
    speed                = 75;
    range                = 60;
    health               = Health(150, 150);
    armor                = Armor(0.3, 0);
    health.dead_lifetime = 10;
    health_bar_offset    = Position(0, 25.16);
    animations.push_back(Animation(State::Idle, "soldier_lvl3"));
    animations[0].anchor_y = 0.83;
    melee.attacks.push_back(MeleeAttack(
        DamageData(9.0, DamageType::Physical, 0.0, 5), -1, 60.0, 1.3, 1.0, "MeleeSword"));
    melee[0].damage_event.source = id;
    slot                         = sf::Vector2f(5.0f, 0.0f);    // 初始化近战偏移
    Hit_offset                   = sf::Vector2f(0.0f, 12.0f);   // 设置受击偏移位置
    heading                      = Heading::None;
}