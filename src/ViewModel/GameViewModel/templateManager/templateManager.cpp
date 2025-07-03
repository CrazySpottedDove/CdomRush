#include "templateManager.h"
#include "Common/macros.h"
#include "Common/type.h"
#include "ViewModel/GameViewModel/bullets/bullets.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"
#include "ViewModel/GameViewModel/towers/towers.h"
#include <SFML/System/Vector2.hpp>

TemplateManager::TemplateManager()
{
    bullet_map = {{BulletType::Arrow, new Arrow()},
                  {BulletType::Bolt, new Bolt()},
                  {BulletType::Bomb, new Bomb()}};
    tower_map  = {
        {TowerType::None, new None(sf::Vector2f(0, 0))},
    };
    enemy_map = {{EnemyType::ForestTroll, new ForestTroll()},
                 {EnemyType::orc_armored, new orc_armored()}};
    fx_map    = {
        {FxType::LevelFlag, new LevelFlag()},
        {FxType::Map, new Map()},
    };
}

Bullet* TemplateManager::CreateBullet(const BulletType type) const
{
    auto it = bullet_map.find(type);
    DEBUG_CODE(
        if (it == bullet_map.end()) {
            std::cerr << "Error: Bullet type " << static_cast<int>(type) << "not found."
                      << std::endl;
        } else { INFO("Created Bullet of type: " << static_cast<int>(type)); })
    return it->second->Clone();
}

Tower* TemplateManager::CreateTower(const TowerType type) const
{
    auto it = tower_map.find(type);
    DEBUG_CODE(
        if (it == tower_map.end()) {
            std::cerr << "Error: Tower type " << static_cast<int>(type) << "not found."
                      << std::endl;
        } else { INFO("Creating Tower of type: " << static_cast<int>(type)); })
    return it->second->Clone();
}

Enemy* TemplateManager::CreateEnemy(const EnemyType type) const
{
    auto it = enemy_map.find(type);
    DEBUG_CODE(
        std::cerr << enemy_map.size() << " enemy types available." << std::endl;
        if (it == enemy_map.end()) {
            std::cerr << "Error: Enemy type " << static_cast<int>(type) << "not found."
                      << std::endl;
        } else { INFO("Created Enemy of type: " << static_cast<int>(type)); })
    return it->second->Clone();
}

Fx* TemplateManager::CreateFx(const FxType type) const
{
    auto it = fx_map.find(type);
    DEBUG_CODE(
        if (it == fx_map.end()) {
            ERROR("Error: Fx type " << static_cast<int>(type) << "not found.");
        } else { INFO("Created Fx of type: " << static_cast<int>(type)); })
    return it->second->Clone();
}