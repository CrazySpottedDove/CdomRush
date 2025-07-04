#include "soldiers.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"


void SoldierMelee::Update(Store& store){
    if(animations[0].current_state == State::Death) return ;
    if(health.hp<=0){
        animations[0].current_state = State::Death;
        death_action();
        health.death_time = store.time;
        return ;
    }

    if(animations[0].current_state == State::Idle){
        heading = Heading::Left;
        if(target_enemy!=INVALID_ID){
            Enemy* enemy_ptr = store.GetEnemy(target_enemy);
            if(enemy_ptr==nullptr){
                target_enemy = INVALID_ID;
                return ;
            }
            if(enemy_ptr->animations[0].current_state == State::Death){
                target_enemy = INVALID_ID;
                return ;
            }

            if(enemy_ptr->slot + slot + enemy_ptr->position != position){
                calc::soldier_move_tick(store,*this,enemy_ptr->slot + slot + enemy_ptr->position);
                walkjudge();
                return ;
            }
            else{
                for(int i=0;i<melee.attacks.size();i++){
                    if (this->melee.attacks[i].IsReady(store)) {
                        melee.attacks[i].Apply(store,id,target_enemy,SourceType::Soldier);   // 执行近战攻击
                        this->animations[0].current_state = State::Attack;   // 设置状态为攻击
                        return;
                    }
                }
                return ;
            }
        }
        else{
            target_enemy = calc::find_foremost_enemy(store,rally_point,range,true);
            if(target_enemy != INVALID_ID){
                ActiveEnemy* enemy_ptr = dynamic_cast<ActiveEnemy*>(store.GetEnemy(target_enemy));
                enemy_ptr->blocker = id;
                return ;
            }
            if(position!=rally_point+rally_point_offset){
                health.last_regen_time = store.time;
                calc::soldier_move_tick(store,*this,rally_point+rally_point_offset);
                walkjudge();
                return ;
            }
            if(position == rally_point + rally_point_offset){
                if(calc::ready_to_regen(store,*this)) calc::regen(store,*this);
            }
        }
        return ;
    }

    if(is_moving()){
        if(target_enemy==INVALID_ID){
            if(position==rally_point+rally_point_offset) animations[0].current_state = State::Idle;
            else calc::soldier_move_tick(store,*this,rally_point+rally_point_offset),walkjudge();
            return ;
        }
        else{
            Enemy* enemy_ptr = store.GetEnemy(target_enemy);
            if(position==enemy_ptr->slot+slot+enemy_ptr->position) animations[0].current_state = State::Idle;
            else calc::soldier_move_tick(store,*this,enemy_ptr->slot+slot+enemy_ptr->position),walkjudge();
            return ;
        }
    }
    
    if(animations[0].current_state == State::Attack){
        if(animations[0].pending == true) return ;
        animations[0].current_state = State::Idle;
        animations[0].pending = true;   // 设置动画为进行中
        return;
    }

    return ;
}


SoldierMeleelv1:: SoldierMeleelv1(Position position_,Position rally_point_,Position offset_){
    position = position_;
    rally_point = rally_point_;
    rally_point_offset = offset_;
    speed = 30;
    animations.push_back(Animation(State::Idle,"forest_troll"));
    melee.attacks.push_back(MeleeAttack(DamageData(100.0, DamageType::Physical, 0.0, 15), 50.0, 1.0, 1.0));
    melee[0].damage_event.source = id;
    slot  = sf::Vector2f(-35.0f, 0.0f);   // 初始化近战偏移
    Hit_offset = sf::Vector2f(30.0f,0.0f);   // 设置受击偏移位置
}