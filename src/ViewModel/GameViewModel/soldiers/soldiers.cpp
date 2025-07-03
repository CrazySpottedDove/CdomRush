#include "soldiers.h"
#include "ViewModel/GameViewModel/store/store.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"


void SoldierMelee::Update(Store& store){
    if(animation.current_state == State::Death) return ;
    if(health.hp<=0){
        animation.current_state = State::Death;
        death_action();
        health.death_time = store.time;
        return ;
    }

    if(animation.current_state == State::Idle){
        heading = Heading::Left;
        if(target_enemy!=INVALID_ID){
            Enemy* enemy_ptr = store.GetEnemy(target_enemy);
            if(enemy_ptr==nullptr){
                target_enemy = INVALID_ID;
                return ;
            }
            if(enemy_ptr->animation.current_state == State::Death){
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
                        this->animation.current_state = State::Attack;   // 设置状态为攻击
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
            if(position==rally_point+rally_point_offset) animation.current_state = State::Idle;
            else calc::soldier_move_tick(store,*this,rally_point+rally_point_offset),walkjudge();
            return ;
        }
        else{
            Enemy* enemy_ptr = store.GetEnemy(target_enemy);
            if(position==enemy_ptr->slot+slot+enemy_ptr->position) animation.current_state = State::Idle;
            else calc::soldier_move_tick(store,*this,enemy_ptr->slot+slot+enemy_ptr->position),walkjudge();
            return ;
        }
    }
    
    if(animation.current_state == State::Attack){
        if(animation.pending == true) return ;
        animation.current_state = State::Idle;
        animation.pending = true;   // 设置动画为进行中
        return;
    }

    return ;
}