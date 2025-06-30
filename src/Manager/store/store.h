#pragma once
#include "Manager/levelManager/levelManager.h"
#include "Manager/pathManager/pathManager.h"
#include "Manager/resourceManager/animationManager.h"
#include "Manager/templateManager/templateManager.h"
#include "Model/bullets/bullets.h"
#include "Model/components/damage.h"
#include "View/ui/UIManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
class Bullet;
class Tower;
class Enemy;
class Soldier;

enum class GameState
{
    Begin,         // 游戏打开时的界面，用于选择关卡
    Loading,       // 加载资源时显示的等待界面
    GameStart,     // 开始一场游戏，等待玩家点击开始按钮
    GamePlaying,   // 游戏进行中
    GameOver,      // 游戏结束，显示游戏结果
};

class Store
{
public:
    Store();
    double                                time       = 0.0;
    double                                gold       = 0.0;
    int                                   life       = 20;
    GameState                             game_state = GameState::GameStart;
    const std::unordered_map<ID, Enemy*>& GetEnemies() const { return enemies; }

    const std::unordered_map<ID, Tower*>& GetTowers() const { return towers; }

    const std::unordered_map<ID, Bullet*>& GetBullets() const { return bullets; }

    const std::unordered_map<ID, Soldier*>& GetSoldiers() const { return soldiers; }

    void QueueEnemy(Enemy* enemy)
    {
        enemies[next_id++] = enemy;
        enemy->Insert(*this);
    }

    void QueueTower(Tower* tower)
    {
        towers[next_id++] = tower;
        tower->Insert(*this);
    }

    void QueueBullet(Bullet* bullet)
    {
        bullets[next_id++] = bullet;
        bullet->Insert(*this);
    }

    void QueueSoldier(Soldier* soldier)
    {
        soldiers[next_id++] = soldier;
        soldier->Insert(*this);
    }

    void QueueDamageEvent(DamageEvent&& event) { damage_events.push_back(std::move(event)); }

    void QueueDamageEvent(const DamageEvent& event) { damage_events.push_back(event); }

    Bullet* CreateBullet(const BulletType type);

    Enemy* GetEnemy(const ID id) const
    {
        auto it = enemies.find(id);
        return it != enemies.end() ? it->second : nullptr;
    }

    Tower* GetTower(const ID id) const
    {
        auto it = towers.find(id);
        return it != towers.end() ? it->second : nullptr;
    }

    Bullet* GetBullet(const ID id) const
    {
        auto it = bullets.find(id);
        return it != bullets.end() ? it->second : nullptr;
    }

    Soldier* GetSoldier(const ID id) const
    {
        auto it = soldiers.find(id);
        return it != soldiers.end() ? it->second : nullptr;
    }

    AnimationManager animation_manager;
    PathManager      path_manager;
    TemplateManager  template_manager;
    LevelManager     level_manager;
    UIManager        ui_manager;
    void             Update(sf::RenderWindow& window);

    // 游戏进行的主函数
    void Game(sf::RenderWindow& window);

private:
    std::unordered_map<ID, Enemy*>   enemies;
    std::unordered_map<ID, Tower*>   towers;
    std::unordered_map<ID, Bullet*>  bullets;
    std::unordered_map<ID, Soldier*> soldiers;
    ID                               next_id = 0;
    // 请注意，damage_event 不是 new 出来的对象
    std::vector<DamageEvent> damage_events;
    void                     UpdateDamageEvents(sf::RenderWindow& window);
    void                     UpdateEnemies(sf::RenderWindow& window);
    void                     UpdateBullets(sf::RenderWindow& window);
    void                     UpdateTowers(sf::RenderWindow& window);
    void                     UpdateSoldiers(sf::RenderWindow& window);
};