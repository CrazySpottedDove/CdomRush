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
                for(register int i=0;i<melee.attacks.size();i++){
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
            if(calc::ready_to_regen(store,*this)) calc::regen(store,*this);
            target_enemy = calc::find_foremost_enemy(store,rally_point,range,true);
            if(target_enemy != INVALID_ID){
                Enemy* enemy = store.GetEnemy(target_enemy);
                
                return ;
            }
            if(position!=rally_point+rally_point_offset){
                calc::soldier_move_tick(store,*this,rally_point+rally_point_offset);
                walkjudge();
                return ;
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
            ActiveEnemyMelee* enemy_ptr = dynamic_cast<ActiveEnemyMelee*>(store.GetEnemy(target_enemy));

            if(position==rally_point+rally_point_offset) animation.current_state = State::Idle;
            else calc::soldier_move_tick(store,*this,rally_point+rally_point_offset),walkjudge();
            return ;
        }
    }
    
    //在状态改变的时候记得改一下last_heal_time

}