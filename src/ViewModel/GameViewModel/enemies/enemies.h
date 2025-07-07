#pragma once
#include "Common/macros.h"
#include "ViewModel/GameViewModel/Function/calc/damage.h"
#include "ViewModel/GameViewModel/Function/calc/hp.h"
#include "ViewModel/GameViewModel/Function/calc/motion.h"
#include "ViewModel/GameViewModel/components/melee.h"
#include "ViewModel/GameViewModel/components/path.h"
#include "ViewModel/GameViewModel/components/ranged.h"
#include "ViewModel/GameViewModel/templates/unit.h"
class Store;

enum class EnemyInnerType
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
    EnemyInnerType    type;   // 敌人类型
    int          gold;
    int          life_cost;
    PathInfo     path_info;

    Enemy() = default;   // 默认构造函数

    bool Insert(Store& store) override
    {
        return true;   // 返回 true 表示插入成功
    }
    bool Remove(Store& store) override
    {
        return true;   // 返回 true 表示移除成功
    }

    virtual Enemy* Clone() const = 0;   // 纯虚函数，用于克隆敌人对象
};

class PassiveEnemy : public Enemy
{   // 无害敌人
public:
    PassiveEnemy()
    {
        type = EnemyInnerType::PassiveEnemy;    // 设置敌人类型为无害敌人
        slot = sf::Vector2f(0.0f, 0.0f);   // 初始化近战偏移
    }
    void Update(Store& store) override;
    // void death_action(Store& store) override ;   // 无害敌人死亡时不执行任何操作
    // Enemy* Clone() const override{
    //     return new PassiveEnemy(*this);   // 返回一个新的PassiveEnemy对象
    // }

};

class ActiveEnemy : public Enemy{
public:
    ID blocker = INVALID_ID;
};

class ActiveEnemyMelee : public ActiveEnemy
{
public:
    ActiveEnemyMelee()
    {
        type = EnemyInnerType::ActiveEnemyMelee;   // 设置敌人类型为近战敌人
        slot = sf::Vector2f(0.0f, 0.0f);       // 初始化近战偏移
    }
    Melee    melee;               // 近战攻击组件

    void Update(Store& store) override;
    // Enemy* Clone() const override{
    //     return new ActiveEnemyMelee(*this);   // 返回一个新的ActiveEnemyMelee对象
    // }
    // void death_action(Store& store) override;
};

class ActiveEnemyRange : public ActiveEnemy
{
public:
    ActiveEnemyRange()
    {
        type = EnemyInnerType::ActiveEnemyRange;   // 设置敌人类型为远程敌人
        slot = sf::Vector2f(0.0f, 0.0f);       // 初始化近战偏移
    }
    Ranged ranged;             // 远程攻击组件
    Melee melee;               // 近战攻击组件
    Position bullet_offset;

    void Update(Store& store) override;

    bool shoot_judge(Store& store);
};

class ForestTroll : public ActiveEnemyMelee
{
public:
    ForestTroll(Position position_ = sf::Vector2f(0,0));
    void death_action(Store& store) override;
    Enemy* Clone() const override
    {
        return new ForestTroll(*this);   // 返回一个新的ForestTroll对象
    }
};

class orc_armored : public ActiveEnemyMelee
{
public:
    orc_armored(Position position_ = sf::Vector2f(0,0));
    void death_action(Store& store) override;
    Enemy* Clone() const override
    {
        return new orc_armored(*this);   // 返回一个新的orc_armored对象
    }
};

class orc_wolf_rider : public ActiveEnemyMelee
{
public:
    orc_wolf_rider(Position position_ = sf::Vector2f(0,0));
    void death_action(Store& store) override;
    Enemy* Clone() const override
    {
        return new orc_wolf_rider(*this);   // 返回一个新的orc_wolf_rider对象
    }
};

class worg : public ActiveEnemyMelee
{
public:
    worg(Position position_ = sf::Vector2f(0,0));
    void death_action(Store& store) override;
    Enemy* Clone() const override
    {
        return new worg(*this);   // 返回一个新的orc_wolf_rider对象
    }
};

class goblin_zapper : public ActiveEnemyRange
{
public:
    goblin_zapper(Position position_ = sf::Vector2f(0,0));
    void death_action(Store& store) override;
    Enemy* Clone() const override
    {
        return new goblin_zapper(*this);
    }
};