#pragma once
#include "Function/calc/damage.h"
#include "Function/calc/hp.h"
#include "Function/calc/motion.h"
#include "Model/components/melee.h"
#include "Model/components/path.h"
#include "Model/soldiers/soldiers.h"
#include "Model/templates/unit.h"
#include <algorithm>
#include <vector>
class Store;

enum class EnemyType
{
    PassiveEnemy,        // 无害敌人
    ActiveEnemyMelee,   // 近战敌人
    ActiveEnemyRange,   // 远程敌人
    None
};
class Enemy : public Unit   // 默认的敌人有这些东西
{
public:
    sf::Vector2f slot;   // 近战偏移
    EnemyType    type;   // 敌人类型
    int          gold;
    int          life_cost;
    PathInfo     path_info;

    Enemy() = default;   // 默认构造函数

    bool Insert(Store& store) override
    {
        // 在存储中插入敌人
        return true;   // 返回 true 表示插入成功
    }
    bool Remove(Store& store) override
    {
        // 从存储中移除敌人
        return true;   // 返回 true 表示移除成功
    }
};

class PassiveEnemy : public Enemy
{   // 无害敌人
public:
    PassiveEnemy()
    {
        type = EnemyType::PassiveEnemy;    // 设置敌人类型为无害敌人
        slot = sf::Vector2f(0.0f, 0.0f);   // 初始化近战偏移
    }
    void Update(Store& store) override;

};

class ActiveEnemyMelee : public Enemy
{
public:
    ActiveEnemyMelee()
    {
        type = EnemyType::ActiveEnemyMelee;   // 设置敌人类型为近战敌人
        slot = sf::Vector2f(0.0f, 0.0f);       // 初始化近战偏移
    }
    Soldier* blocker = nullptr;   // 用于阻挡敌人前进的单位
    Melee    melee;               // 近战攻击组件

    void Update(Store& store) override;

};

class ForestTroll : public ActiveEnemyMelee
{
public:
    ForestTroll(Position position_);
};