#pragma once

enum class TowerType
{
    None,
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

enum class EnemyType
{
    ForestTroll,   // 森林巨魔
    orc_armored,
    orc_wolf_rider
};

enum class BulletType
{
    Arrow,   // 箭矢
    Bolt,    // 法球
    Bomb     // 炸弹
};

enum class FxType
{
    Explosion,
    LevelFlag,
    CommonUpgradeIcon,
    SoldierStatusWidget,
    EnemyStatusWidget,
    TowerStatusWidget,
    Map,
    None
};


