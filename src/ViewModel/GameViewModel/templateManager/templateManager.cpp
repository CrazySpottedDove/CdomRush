#include "templateManager.h"
#include "Common/macros.h"
#include "Common/type.h"
#include "ViewModel/GameViewModel/bullets/bullets.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"
#include "ViewModel/GameViewModel/fx/fx.h"
#include "ViewModel/GameViewModel/towers/towers.h"
#include <SFML/System/Vector2.hpp>

TemplateManager::TemplateManager()
{
    bullet_map = {{BulletType::Arrow, new Arrow()},
                  {BulletType::Bolt, new Bolt()},
                  {BulletType::Bomb, new Bomb()}};
    tower_map  = {
        {TowerType::None, new None()},
        {TowerType::Archer1, new Archer1()},
        {TowerType::Archer2, new Archer2()},
        {TowerType::Archer3, new Archer3()},
        {TowerType::Engineer1, new Engineer1()},
        {TowerType::Engineer2, new Engineer2()},
        {TowerType::Engineer3, new Engineer3()},
        {TowerType::Mage1, new Mage1()},
        {TowerType::Mage2, new Mage2()},
        {TowerType::Mage3, new Mage3()},
        // {TowerType::Barrack1, new Barracks1()},
        // {TowerType::Barrack2, new Barracks2()},
        // {TowerType::Barrack3, new Barracks3()}
    };
    enemy_map = {{EnemyType::ForestTroll, new ForestTroll()},
                 {EnemyType::orc_armored, new orc_armored()},
                 {EnemyType::orc_wolf_rider, new orc_wolf_rider()},
                 {EnemyType::worg, new worg()}
                };
    fx_map    = {
        {FxType::LevelFlag, new LevelFlag()},
        {FxType::Map, new Map()},
        {FxType::Explosion, new Explosion()},
        {FxType::BloodSplat, new BloodSplat()},
        {FxType::GoldStat,new GoldStat()},
        {FxType::LifeStat, new LifeStat()},
        {FxType::WaveStat, new WaveStat()},
        {FxType::TopLeft, new TopLeft()},
        {FxType::LoadingGrass, new LoadingGrass()},{
            FxType::GameOverFailure,new GameOverFailure()
        },{
            FxType::GameOverVictory, new GameOverVictory()
        }
    };
    soldier_map   = {{SoldierType::SoldierMeleelv1, new SoldierMeleelv1()},
                     {SoldierType::SoldierMeleelv2, new SoldierMeleelv2()},
                     {SoldierType::SoldierMeleelv3, new SoldierMeleelv3()}};
    action_fx_map = {
                     };
}

TemplateManager::~TemplateManager()
{
    for (auto& pair : bullet_map) {
        delete pair.second;
    }
    for (auto& pair : tower_map) {
        delete pair.second;
    }
    for (auto& pair : enemy_map) {
        delete pair.second;
    }
    for (auto& pair : fx_map) {
        delete pair.second;
    }
    for (auto& pair : soldier_map) {
        delete pair.second;
    }
    for (auto& pair : action_fx_map) {
        delete pair.second;
    }
}

Bullet* TemplateManager::CreateBullet(const BulletType type) const
{
    auto it = bullet_map.find(type);
    DEBUG_CODE(if (it == bullet_map.end()) {
        std::cerr << "Error: Bullet type " << static_cast<int>(type) << "not found." << std::endl;
    } else {
        //  INFO("Created Bullet of type: " << static_cast<int>(type));
    })
    return it->second->Clone();
}

Tower* TemplateManager::CreateTower(const TowerType type) const
{
    auto it = tower_map.find(type);
    DEBUG_CODE(if (it == tower_map.end()) {
        std::cerr << "Error: Tower type " << static_cast<int>(type) << "not found." << std::endl;
    } else {
        // INFO("Creating Tower of type: " << static_cast<int>(type));
    })
    return it->second->Clone();
}

Enemy* TemplateManager::CreateEnemy(const EnemyType type) const
{
    auto it = enemy_map.find(type);
    DEBUG_CODE(if (it == enemy_map.end()) {
        std::cerr << "Error: Enemy type " << static_cast<int>(type) << "not found." << std::endl;
    } else {
        //  INFO("Created Enemy of type: " << static_cast<int>(type));
    })
    return it->second->Clone();
}

Fx* TemplateManager::CreateFx(const FxType type) const
{
    auto it = fx_map.find(type);
    DEBUG_CODE(if (it == fx_map.end()) {
        ERROR("Error: Fx type " << static_cast<int>(type) << "not found.");
    } else {
        // INFO("Created Fx of type: " << static_cast<int>(type));
    })
    return it->second->Clone();
}

Soldier* TemplateManager::CreateSoldier(const SoldierType type) const
{
    auto it = soldier_map.find(type);
    DEBUG_CODE(if (it == soldier_map.end()) {
        ERROR("Error: Soldier type" << static_cast<int>(type) << "not found.");
    } else {
        // INFO("Created Soldier of type: " << static_cast<int>(type));
    })
    return it->second->Clone();
}

ActionFx* TemplateManager::CreateActionFx(const FxType type) const
{
    auto it = action_fx_map.find(type);
    DEBUG_CODE(if (it == action_fx_map.end()) {
        ERROR("Error: ActionFx type " << static_cast<int>(type) << "not found.");
    } else {
        // INFO("Created ActionFx of type: " << static_cast<int>(type));
    })
    return it->second->Clone();
}