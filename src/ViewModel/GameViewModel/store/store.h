#pragma once
// #include "Function/events/events.h"
// #include "Manager/levelManager/levelManager.h"
// #include "Manager/pathManager/pathManager.h"
// #include "Manager/resourceManager/animationManager.h"
// #include "Manager/templateManager/templateManager.h"
// #include "Manager/waveManager/waveManager.h"

#include "ViewModel/GameViewModel/bullets/bullets.h"
#include "ViewModel/GameViewModel/components/damage.h"
#include "Common/macros.h"
#include "ViewModel/SpriteViewModel/resourceManager.h"
#include "ViewModel/GameViewModel/templateManager/templateManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <unordered_map>
#include <vector>
class Bullet;
class Tower;
class Enemy;
class Soldier;

// TODO: 将 GameState 移动到 App 层
enum class GameState
{
    Begin,         // 游戏打开时的界面，用于选择关卡
    Loading,       // 加载资源时显示的等待界面
    GameStart,     // 开始一场游戏，等待玩家点击开始按钮
    GamePlaying,   // 游戏进行中
    GameOver,      // 游戏结束，显示游戏结果
};

/**
 * @brief Store 是游戏的全局数据存储结构，只负责实体数据的创建与删除，不负责任何实体状态更新与渲染相关的内容。另外， Store 还持有 ResourceManager，在初始化时通过 ResourceManager 的初始化来实现游戏资源数据的加载。
 *
 *
 */
class Store
{
public:
    Store() = default;
    double                                time                        = 0.0;
    double                                gold                        = 0.0;
    int                                   life                        = 20;
    // GameState                             game_state                  = GameState::Begin;
    // bool                                  come_into_level_select_view = true;
    const std::unordered_map<ID, Enemy*>& GetEnemies() const { return enemies; }

    const std::unordered_map<ID, Tower*>& GetTowers() const { return towers; }

    const std::unordered_map<ID, Bullet*>& GetBullets() const { return bullets; }

    const std::unordered_map<ID, Soldier*>& GetSoldiers() const { return soldiers; }

    void QueueEnemy(Enemy* enemy);

    void QueueTower(Tower* tower);

    void QueueBullet(Bullet* bullet);

    void QueueSoldier(Soldier* soldier);

    std::unordered_map<ID, Enemy*>::iterator DequeueEnemy(
        std::unordered_map<ID, Enemy*>::iterator& it);

    std::unordered_map<ID, Tower*>::iterator DequeueTower(
        std::unordered_map<ID, Tower*>::iterator& it);

    std::unordered_map<ID, Bullet*>::iterator DequeueBullet(
        std::unordered_map<ID, Bullet*>::iterator& it);

    std::unordered_map<ID, Soldier*>::iterator DequeueSoldier(
        std::unordered_map<ID, Soldier*>::iterator& it);

    std::unordered_map<ID, Fx*>::iterator DequeueFx(std::unordered_map<ID, Fx*>::iterator& it);

    void DequeueEnemy(const ID id);

    void DequeueTower(const ID id);

    void DequeueBullet(const ID id);

    void DequeueSoldier(const ID id);

    void DequeueFx(const ID id);

    void QueueDamageEvent(DamageEvent&& event) { damage_events.push_back(std::move(event)); }

    void QueueDamageEvent(const DamageEvent& event) { damage_events.push_back(event); }

    // void QueueEvent(const Event& event) { event_queue.push(event); }

    // void QueueEvent(Event&& event) { event_queue.push(std::move(event)); }

    void QueueFx(Fx* fx);

    Enemy* GetEnemy(const ID id) const;

    Tower* GetTower(const ID id) const;

    Bullet* GetBullet(const ID id) const;

    Soldier* GetSoldier(const ID id) const;

    Fx* GetFx(const ID id) const;

    TemplateManager  template_manager;
    ResourceManager resource_manager;
    // 游戏进行的主函数
    // void Game(sf::RenderWindow& window);

private:
    std::unordered_map<ID, Enemy*>   enemies;
    std::unordered_map<ID, Tower*>   towers;
    std::unordered_map<ID, Bullet*>  bullets;
    std::unordered_map<ID, Soldier*> soldiers;
    std::unordered_map<ID, Fx*>      fxs;
    // std::queue<Event>                event_queue;
    ID                               next_id = 0;
    // 请注意，damage_event 不是 new 出来的对象
    std::vector<DamageEvent> damage_events;
    // void                     UpdateDamageEvents(sf::RenderWindow& window);
    // void                     UpdateEnemies(sf::RenderWindow& window);
    // void                     UpdateBullets(sf::RenderWindow& window);
    // void                     UpdateTowers(sf::RenderWindow& window);
    // void                     UpdateSoldiers(sf::RenderWindow& window);

    // void                     UpdateFx(sf::RenderWindow& window);

    // void                     ExecuteEvents();
    // void                     Update(sf::RenderWindow& window);
};