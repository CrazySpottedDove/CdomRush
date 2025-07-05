#pragma once

#include "Common/type.h"
#include "ViewModel/GameViewModel/bullets/bullets.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"
#include "ViewModel/GameViewModel/fx/fx.h"
#include "ViewModel/GameViewModel/towers/towers.h"
#include "ViewModel/GameViewModel/soldiers/soldiers.h"
#include <unordered_map>

class TemplateManager
{
public:
    Bullet* CreateBullet(const BulletType type) const ;
    Tower* CreateTower(const TowerType type) const ;
    Enemy* CreateEnemy(const EnemyType type) const ;
    Fx* CreateFx(const FxType type) const;
    ActionFx* CreateActionFx(const FxType type) const;
    Soldier* CreateSoldier(const SoldierType type) const;
    TemplateManager();
private:
    std::unordered_map<BulletType, Bullet*> bullet_map;
    std::unordered_map<TowerType, Tower*>   tower_map;
    std::unordered_map<EnemyType, Enemy*>   enemy_map;
    std::unordered_map<FxType, Fx*>        fx_map;
    std::unordered_map<SoldierType, Soldier*> soldier_map;
    std::unordered_map<FxType, ActionFx*> action_fx_map;
};