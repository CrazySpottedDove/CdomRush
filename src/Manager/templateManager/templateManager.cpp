#include "Manager/templateManager/templateManager.h"
#include "Model/bullets/bullets.h"
#include "Model/enemies/enemies.h"
#include "Model/towers/towers.h"
#include "utils/macros.h"
#include <SFML/System/Vector2.hpp>

TemplateManager::TemplateManager()
{
    bullet_map = {
        // {BulletType::Arrow, new Arrow()},
        // {BulletType::Bolt, new Bolt()},
        // {BulletType::Bomb, new Bomb()}
    };
    tower_map = {
        {TowerType::None, new None(sf::Vector2f(0, 0))},
    };
    enemy_map = {
        {EnemyType::ForestTroll, new ForestTroll()
        },{
            EnemyType::orc_armored, new orc_armored()
        }
    };
    fx_map = {
        {FxType::LevelFlag, new LevelFlag()},
    };
}

Bullet* TemplateManager::CreateBullet(const BulletType type) const
{
    auto it = bullet_map.find(type);
    DEBUG_CODE(
        if (it == bullet_map.end()) {
            std::cerr << "Error: Bullet type " << static_cast<int>(type) << "not found."
                      << std::endl;
        } else { std::cout << "Creating Bullet of type: " << static_cast<int>(type) << std::endl; })
    return it->second->Clone();
}

Tower* TemplateManager::CreateTower(const TowerType type) const
{
    auto it = tower_map.find(type);
    DEBUG_CODE(
        if (it == tower_map.end()) {
            std::cerr << "Error: Tower type " << static_cast<int>(type) << "not found."
                      << std::endl;
        } else { std::cout << "Creating Tower of type: " << static_cast<int>(type) << std::endl; })
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
        } else { std::cout << "Creating Enemy of type: " << static_cast<int>(type) << std::endl; })
    return it->second->Clone();
}

Fx* TemplateManager::CreateFx(const FxType type) const
{
    auto it = fx_map.find(type);
    DEBUG_CODE(
        if (it == fx_map.end()) {
            std::cerr << "Error: Fx type " << static_cast<int>(type) << "not found."
                      << std::endl;
        } else { std::cout << "Creating Fx of type: " << static_cast<int>(type) << std::endl; })
    return it->second->Clone();
}