#include "enemies.h"

class ForestTroll : public ActiveEnemy_Melee{
    ForestTroll() {
        this->health = Health(4000,4000); // 设置生命值
        this->armor = Armor(0,0); // 设置护甲
        this->speed = 36; // 设置速度
        this->gold = 200; // 设置击杀奖励
        this->life_cost = 5; // 设置生命损失
        this->state = State::Idle; // 设置初始状态
        this->melee.attacks.push_back(new MeleeAttack(DamageData(100.0, DamageType::Physical, 0.0, 0), 50.0, 1.0, 1.0));
        this->melee[0]->damage_event.source = static_cast<Entity*>(this); // 设置攻击来源为当前敌人
        this->slot = sf::Vector2f(35.0f, 0.0f); // 初始化近战偏移
    }
};