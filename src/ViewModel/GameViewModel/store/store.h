#pragma once

#include "Common/macros.h"
#include "Common/viewData.h"
#include "ViewModel/GameViewModel/bullets/bullets.h"
#include "ViewModel/GameViewModel/components/damage.h"
#include "ViewModel/GameViewModel/templateManager/templateManager.h"
#include "ViewModel/SpriteViewModel/resourceManager.h"
#include "ViewModel/wave.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

class Bullet;
class Tower;
class Enemy;
class Soldier;

/**
 * @brief Store
 * 是游戏的全局数据存储结构，只负责实体数据的创建，删除与更新函数，不负责任何游戏状态更新与渲染相关的内容。另外，
 * Store 还持有 ResourceManager，在初始化时通过 ResourceManager 的初始化来实现游戏资源数据的加载。
 * @note View 层对 Store 并不可见，Store 只需要将 ViewData 放进自己的 ViewDataQueue
 * 中即可，具体的传递由 App 层完成。
 *
 */
class Store
{
public:
    Store()                           = default;
    double      time                  = 0.0;
    double      gold                  = 0.0;
    int         life                  = 20;
    std::size_t current_wave_index    = 0;
    std::size_t current_subwave_index = 0;
    double      current_wave_time     = 0.0;
    std::string current_level_name;
    bool        preparing = true;

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

    void QueueFx(Fx* fx);

    void QueueActionFx(ActionFx* action_fx);

    Enemy* GetEnemy(const ID id) const;

    Tower* GetTower(const ID id) const;

    Bullet* GetBullet(const ID id) const;

    Soldier* GetSoldier(const ID id) const;

    Fx* GetFx(const ID id) const;

    TemplateManager template_manager;
    ResourceManager resource_manager;

    void UpdateDamageEvents();
    void UpdateEnemies();
    void UpdateBullets();
    void UpdateTowers();
    void UpdateSoldiers();
    void UpdateFxs();
    /**
     * @brief 在关卡进行时调用，生成敌人
     *
     */
    void SpawnWaves();

    ViewDataQueue* GetViewDataQueue();
    void           ClearViewDataQueue();
    void           ClearFxs();
    void           ClearActionFxs();
    void           Clear();

    /**
     * @brief 在进入关卡时调用，更新一个关卡的全部数据
     * @note 在调用前，保证 Clear() 已经被调用，保证 Store 中的数据是干净的。
     */
    void InitLevel(const std::string& level_name);

private:
    std::unordered_map<ID, Enemy*>   enemies;
    std::unordered_map<ID, Tower*>   towers;
    std::unordered_map<ID, Bullet*>  bullets;
    std::unordered_map<ID, Soldier*> soldiers;
    std::unordered_map<ID, Fx*>      fxs;
    std::unordered_map<ID, ActionFx*>      action_fxs;
    ID                               next_id = 0;
    // 请注意，damage_event 不是 new 出来的对象
    std::vector<DamageEvent> damage_events;
    ViewDataQueue            view_data_queue;
    PendingEnemyQueue        pending_enemy_queue;

    /**
     * @brief 在开启新的关卡时调用，初始化塔位
     *
     */
    void InitTowers();
    void QueueViewDataFromEntity(Entity* entity);
};