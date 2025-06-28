#pragma once
#include "Model/templates/entity.h"
#include "utils/macros.h"

class Store;

class Tower;
enum class TowerType{
    Archer1,
    Archer2,
    Archer3,
    Engineer1,
    Engineer2,
    Engineer3,
    Mage1,
    Mage2,
    Mage3,
    Barrack1,
    Barrack2,
    Barrack3
};

enum class TowerAction{
    Upgrade,
    Sell,
    ChangeRally,
    PowerUpgrade
};

// TODO: 每个 Tower 应该维护一个 Action 列表，这个列表可以被 View 层访问到，从而让 View 层得知需要怎样的 UI 更新
class Tower : public Entity
{
    Position            position;
    int                total_price;
};