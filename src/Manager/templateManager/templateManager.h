#pragma once

#include "Model/bullets/bullets.h"
#include "Model/enemies/enemies.h"
#include "Model/towers/towers.h"
#include <unordered_map>

class TemplateManager
{
public:
    Bullet* CreateBullet(const BulletType type) const ;
    Tower* CreateTower(const TowerType type) const ;
    Enemy* CreateEnemy(const EnemyType type) const ;
    TemplateManager();
private:
    std::unordered_map<BulletType, Bullet*> bullet_map;
    std::unordered_map<TowerType, Tower*>   tower_map;
    std::unordered_map<EnemyType, Enemy*>   enemy_map;

};