#pragma once

#include "Model/bullets/bullets.h"
#include "Model/enemies/enemies.h"
#include "Model/towers/towers.h"
#include <unordered_map>
class TemplateManager
{
public:
    std::unordered_map<BulletType, Bullet*> bullet_map;
    std::unordered_map<TowerType, Tower*> tower_map;
    std::unordered_map<EnemyType, Enemy*> enemy_map;
    // soldier_map
    TemplateManager();
};