#include "UIManager.h"
#include "View/ui/BulletUI.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <algorithm>
#include <memory>

/**
 * @brief 构造函数
 */
UIManager::UIManager()
    : store_(nullptr)
    , animation_player_(nullptr)
    , current_map_prefix_("")           // 初始化时没有地图
    , current_map_position_(0.0f, 0.0f) // 默认位置
    , current_map_scale_(1.0f, 1.0f)    // 默认缩放
{
    std::cout << "UIManager: Initialized with AnimationPlayer" << std::endl;
}

void UIManager::QueueBulletUI(Bullet* bullet){
    bullet_uis_[bullet] = std::make_unique<BulletUI>(bullet, *animation_player_);
}

void UIManager::DeQueueBulletUI(Bullet* bullet){
    auto it = bullet_uis_.find(bullet);
    bullet_uis_.erase(it);
}

void UIManager::QueueEnemyUI(Enemy* enemy){
    enemy_uis_[enemy] = std::make_unique<EnemyUI>(enemy, *animation_player_);
}

void UIManager::DeQueueEnemyUI(Enemy* enemy){
    auto it = enemy_uis_.find(enemy);
    enemy_uis_.erase(it);
}

void UIManager::QueueSoldierUI(Soldier* soldier){
    soldier_uis_[soldier] = std::make_unique<SoldierUI>(soldier, *animation_player_);
}

void UIManager::DeQueueSoldierUI(Soldier* soldier){
    auto it = soldier_uis_.find(soldier);
    soldier_uis_.erase(it);
}

void UIManager::QueueTowerUI(Tower* tower){
    tower_uis_[tower] = std::make_unique<TowerUI>(tower, *animation_player_);
}

void UIManager::DeQueueTowerUI(Tower* tower){
    auto it = tower_uis_.find(tower);
    tower_uis_.erase(it);
}

void UIManager::RenderEnemyUI(sf::RenderWindow&window, Enemy* enemy, const sf::Vector2f& scale){
    const auto it = enemy_uis_.find(enemy);
    it->second->Render(window, scale);
}

void UIManager::RenderSoldierUI(sf::RenderWindow&window, Soldier* soldier, const sf::Vector2f& scale){
    const auto it = soldier_uis_.find(soldier);
    it->second->Render(window, scale);
}

void UIManager::RenderBulletUI(sf::RenderWindow&window, Bullet* bullet, const sf::Vector2f& scale){
    const auto it = bullet_uis_.find(bullet);
    it->second->Render(window, scale);
}

void UIManager::RenderTowerUI(sf::RenderWindow&window, Tower* tower, const sf::Vector2f& scale){
    const auto it = tower_uis_.find(tower);
    it->second->Render(window, scale);
}

// /**
//  * @brief 更新UI管理器
//  */
// void UIManager::Update()
// {
//     // 同步所有实体的UI对象
//     SyncEnemyUIs();
//     SyncSoldierUIs();
//     SyncBulletUIs();
//     SyncTowerUIs();
// }

/**
 * @brief 渲染所有UI
 */
void UIManager::RenderAll(sf::RenderWindow& window, const sf::Vector2f& scale)
{
    // 根据当前地图状态决定是否渲染地图
    if (HasCurrentMap()) {
        RenderMap(window, current_map_prefix_, current_map_position_, current_map_scale_);
    }

    // 然后按层次渲染实体：Tower -> Enemy -> Soldier -> Bullet

    // 1. 渲染Tower（最底层）
    for (auto& [tower_ptr, tower_ui] : tower_uis_) {
        if (tower_ui) {
            tower_ui->Render(window, scale);
        }
    }

    // 2. 渲染Enemy（敌人层）
    for (auto& [enemy_ptr, enemy_ui] : enemy_uis_) {
        if (enemy_ui) {
            enemy_ui->Render(window, scale);
        }
    }

    // 3. 渲染Soldier（士兵层）
    for (auto& [soldier_ptr, soldier_ui] : soldier_uis_) {
        if (soldier_ui) {
            soldier_ui->Render(window, scale);
        }
    }

    // 4. 渲染Bullet（子弹层，最顶层）
    for (auto& [bullet_ptr, bullet_ui] : bullet_uis_) {
        if (bullet_ui) {
            bullet_ui->Render(window, scale);
        }
    }
}

/**
 * @brief 单独渲染地图
 */
void UIManager::RenderMap(sf::RenderWindow& window, const std::string& map_prefix,
                         const sf::Vector2f& position, const sf::Vector2f& scale)
{
    if (animation_player_) {
        animation_player_->RenderMap(window, map_prefix, position, scale);
    }
}

/**
 * @brief 获取统计信息
 */
UIManager::UIStats UIManager::GetStats() const
{
    return {
        enemy_uis_.size(),
        soldier_uis_.size(),
        bullet_uis_.size(),
        tower_uis_.size()
    };
}

// /**
//  * @brief 同步Enemy列表
//  */
// void UIManager::SyncEnemyUIs()
// {
//     const auto& enemies =  store_.GetEnemies();

//     CleanupDeadUIs(enemy_uis_,enemies);//删除死去的即enemies里面没有的

//     for(auto&[enemy_id, enemy]:enemies)//增添新的enemy
//     {
//         if(enemy != nullptr && enemy_uis_.find(enemy) == enemy_uis_.end())
//         {
//             std::cout << "UIManager: Creating new EnemyUI for enemy at (" << enemy->position.x <<","<<enemy->position.y <<")" <<std::endl;
//             enemy_uis_[enemy] = std::make_unique<EnemyUI>(enemy,*animation_player_);
//         }
//     }
// }

// /**
//  * @brief 同步Soldier列表
//  */
// void UIManager::SyncSoldierUIs()
// {
//     const auto& soldiers = store_.GetSoldiers();

//     // 清理不存在的Soldier UI
//     CleanupDeadUIs(soldier_uis_, soldiers);

//     // 为新Soldier创建UI
//     for (Soldier* soldier : soldiers) {
//         if (soldier != nullptr && soldier_uis_.find(soldier) == soldier_uis_.end()) {
//             std::cout << "UIManager: Creating new SoldierUI for soldier at ("
//                       << soldier->position.x << "," << soldier->position.y << ")" << std::endl;
//             soldier_uis_[soldier] = std::make_unique<SoldierUI>(soldier, *animation_player_);
//         }
//     }
// }

// /**
//  * @brief 同步Bullet列表
//  */
// void UIManager::SyncBulletUIs()
// {
//     const auto& bullets = store_.GetBullets();

//     // 清理不存在的Bullet UI
//     CleanupDeadUIs(bullet_uis_, bullets);

//     // 为新Bullet创建UI
//     for (Bullet* bullet : bullets) {
//         if (bullet != nullptr && bullet_uis_.find(bullet) == bullet_uis_.end()) {
//             std::cout << "UIManager: Creating new BulletUI for bullet at ("
//                       << bullet->position.x << "," << bullet->position.y << ")" << std::endl;
//             bullet_uis_[bullet] = std::make_unique<BulletUI>(bullet, *animation_player_);
//         }
//     }
// }

// /**
//  * @brief 同步Tower列表
//  */
// void UIManager::SyncTowerUIs()
// {
//     const auto& towers = store_.GetTowers();

//     // 清理不存在的Tower UI
//     CleanupDeadUIs(tower_uis_, towers);

//     // 为新Tower创建UI
//     for (Tower* tower : towers) {
//         if (tower != nullptr && tower_uis_.find(tower) == tower_uis_.end()) {
//             std::cout << "UIManager: Creating new TowerUI for tower at ("
//                       << tower->position.x << "," << tower->position.y << ")" << std::endl;
//             tower_uis_[tower] = std::make_unique<TowerUI>(tower, *animation_player_);
//         }
//     }
// }

// /**
//  * @brief 清理死亡实体的UI
//  */
// template<typename EntityType, typename UIType>
// void UIManager::CleanupDeadUIs(std::unordered_map<EntityType*, std::unique_ptr<UIType>>& ui_map,
//                                 const std::vector<EntityType*>& entities)
// {
//     auto ui_it = ui_map.begin();
//     while (ui_it != ui_map.end())
//     {
//         EntityType* entity_ptr = ui_it->first;
//         bool entity_exists = std::find(entities.begin(),entities.end(),entity_ptr) != entities.end();
//         if(!entity_exists)
//         {
//             ui_it = ui_map.erase(ui_it);
//             std::cout << "UIManager: Removing UI for deleted entity" << std::endl;
//         }
//         else
//         {
//             ++ui_it;
//         }
//     }
// }

// ===============================
// 地图管理功能实现
// ===============================

/**
 * @brief 设置当前地图
 */
void UIManager::SetCurrentMap(const std::string& map_prefix,
                             const sf::Vector2f& position,
                             const sf::Vector2f& scale)
{
    current_map_prefix_ = map_prefix;
    current_map_position_ = position;
    current_map_scale_ = scale;

    std::cout << "UIManager: Set current map to '" << map_prefix
              << "' at position (" << position.x << "," << position.y
              << ") with scale (" << scale.x << "," << scale.y << ")" << std::endl;
}

/**
 * @brief 清除当前地图
 */
void UIManager::ClearCurrentMap()
{
    if (!current_map_prefix_.empty()) {
        std::cout << "UIManager: Clearing map '" << current_map_prefix_ << "'" << std::endl;
        current_map_prefix_.clear();
        current_map_position_ = {0.0f, 0.0f};
        current_map_scale_ = {1.0f, 1.0f};
    }
}