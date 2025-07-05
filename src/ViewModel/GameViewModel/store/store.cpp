#include "ViewModel/GameViewModel/store/store.h"
#include "Common/action.h"
#include "Common/macros.h"
#include "Common/viewData.h"
#include "ViewModel/GameViewModel/Function/calc/damage.h"
#include "ViewModel/GameViewModel/Function/calc/hp.h"
#include "ViewModel/GameViewModel/Function/calc/motion.h"
#include "ViewModel/GameViewModel/bullets/bullets.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"
#include "ViewModel/GameViewModel/soldiers/soldiers.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

/**
 * @brief 首先考虑结算所有可以结算的伤害事件，尽快完成生命值的更新
 * @note 发现敌人已经死亡或者从 store 中移除时，不执行伤害事件，直接删去
 *
 */
void Store::UpdateDamageEvents()
{
    const auto new_damage_event_end = std::remove_if(
        damage_events.begin(), damage_events.end(), [this](DamageEvent& damage_event) -> bool {
            if (damage_event.target == INVALID_ID ||
                calc::is_dead(*GetEnemy(damage_event.target))) {
                return true;
            }

            if (damage_event.data.apply_delay > 0) {
                --damage_event.data.apply_delay;
                return false;
            }

            calc::enforce_damage(*this, damage_event);
            return true;
        });
    damage_events.erase(new_damage_event_end, damage_events.end());
}

void Store::UpdateEnemies()
{
    auto it = enemies.begin();
    while (it != enemies.end()) {
        Enemy* enemy = it->second;
        if (calc::is_dead(*enemy) && calc::should_remove(*this, *enemy)) {
            DEBUG_CODE(std::cout << "Removing enemy with ID: " << enemy->id << std::endl;
                       std::cout << enemy->health.dead_lifetime << std::endl;
                       std::cout << enemy->health.death_time << std::endl;
                       std::cout << time << std::endl;
            )
            it = DequeueEnemy(it);
            continue;
        }
        if (calc::enemy_reached_defence_point(*this, *enemy)) {
            life -= enemy->life_cost;
            gold += enemy->gold;
            it = DequeueEnemy(it);
            continue;
        }
        enemy->Update(*this);
        QueueViewDataFromEntity(enemy);
        ++it;
    }
}

void Store::UpdateBullets()
{
    auto it = bullets.begin();
    while (it != bullets.end()) {
        Bullet* bullet = it->second;
        if (bullet->animations[0].current_state == State::Hit &&
            resource_manager.GetAnimationGroupMap()
                    ->at(bullet->animations[0].prefix)
                    .at(bullet->animations[0].current_state)
                    .to <= bullet->animations[0].frame_id) {
            bullet->Remove(*this);
            delete bullet;
            it = bullets.erase(it);
            continue;
        }
        bullet->Update(*this);

        QueueViewDataFromEntity(bullet);
        ++it;
    }
}

void Store::UpdateSoldiers()
{
    auto it = soldiers.begin();
    while (it != soldiers.end()) {
        Soldier* soldier = it->second;
        if (calc::is_dead(*soldier) && calc::should_remove(*this, *soldier)) {
            soldier->Remove(*this);
            delete soldier;
            it = soldiers.erase(it);
            continue;
        }
        soldier->Update(*this);
        QueueViewDataFromEntity(soldier);
        ++it;
    }
}

/**
 * @brief 删除和购买塔应该通过事件来处理，塔的 update 中因此没有删除这一项
 *
 * @param window
 */
void Store::UpdateTowers()
{
    auto it = towers.begin();
    while (it != towers.end()) {
        Tower* tower = it->second;
        tower->Update(*this);
        QueueViewDataFromEntity(tower);
        ++it;
    }
}

void Store::UpdateFxs()
{
    auto it = fxs.begin();
    while (it != fxs.end()) {
        Fx* fx = it->second;
        fx->Update(*this);
        QueueViewDataFromEntity(fx);
        ++it;
    }
}

void Store::InitTowers()
{
    for (const auto& tower_essential : *resource_manager.GetTowerEssentials()) {
        Tower* tower       = template_manager.CreateTower(tower_essential.type);
        tower->position    = tower_essential.position;
        tower->rally_point = tower_essential.rally_point;
        QueueTower(tower);
    }
}

void Store::SpawnWaves()
{
    current_wave_time += FRAME_LENGTH;
    std::vector<Wave>& waves = *resource_manager.GetWaves();
    if (current_wave_index >= waves.size()) {
        return;
    }

    if (preparing) {
        if (current_wave_time >= waves[current_wave_index].preparation_time) {
            preparing         = false;
            current_wave_time = 0;
        }
        return;
    }

    while (pending_enemy_queue.size() > 0 &&
           pending_enemy_queue.top().spawn_time <= current_wave_time) {
        const auto& pending_enemy        = pending_enemy_queue.top();
        Enemy*      new_enemy            = template_manager.CreateEnemy(pending_enemy.enemy_type);
        new_enemy->path_info.path_id     = pending_enemy.path_id;
        new_enemy->path_info.subpath_id  = pending_enemy.subpath_id;
        new_enemy->path_info.waypoint_id = 0;
        new_enemy->position =
            (*resource_manager.GetPaths())[pending_enemy.path_id][pending_enemy.subpath_id][0];
        QueueEnemy(new_enemy);
        pending_enemy_queue.pop();
    }

    // 检查当前波次持续时间是否已经结束
    if (current_wave_time >= waves[current_wave_index].duration) {
        ++current_wave_index;
        current_wave_time     = 0;
        preparing             = true;
        current_subwave_index = 0;
        return;
    }

    // 检查当前波次的子波是否需要触发
    while (waves[current_wave_index].sub_waves.size() > current_subwave_index) {
        const auto& sub_wave = waves[current_wave_index].sub_waves[current_subwave_index];
        if (current_wave_time >= sub_wave.time) {
            // 生成子波的敌人
            for (std::size_t i = 0; i < sub_wave.count; ++i) {
                pending_enemy_queue.emplace(PendingEnemy(sub_wave.enemy_type,
                                                         current_wave_time + i * sub_wave.gap,
                                                         sub_wave.path_id,
                                                         sub_wave.subpath_id));
            }
            ++current_subwave_index;
        }
        else {
            break;
        }
    }
}

void Store::QueueViewDataFromEntity(Entity* entity)
{
    view_data_queue.emplace(ViewData{&entity->animations, entity->position});
}

std::unordered_map<ID, Enemy*>::iterator Store::DequeueEnemy(
    std::unordered_map<ID, Enemy*>::iterator& it)
{
    Enemy* enemy = it->second;
    enemy->Remove(*this);
    delete enemy;
    return enemies.erase(it);
};

std::unordered_map<ID, Tower*>::iterator Store::DequeueTower(
    std::unordered_map<ID, Tower*>::iterator& it)
{
    Tower* tower = it->second;
    tower->Remove(*this);
    delete tower;
    return towers.erase(it);
};
std::unordered_map<ID, Bullet*>::iterator Store::DequeueBullet(
    std::unordered_map<ID, Bullet*>::iterator& it)
{
    Bullet* bullet = it->second;
    bullet->Remove(*this);
    delete bullet;
    return bullets.erase(it);
};
std::unordered_map<ID, Soldier*>::iterator Store::DequeueSoldier(
    std::unordered_map<ID, Soldier*>::iterator& it)
{
    Soldier* soldier = it->second;
    soldier->Remove(*this);
    delete soldier;
    return soldiers.erase(it);
};

std::unordered_map<ID, Fx*>::iterator Store::DequeueFx(std::unordered_map<ID, Fx*>::iterator& it)
{
    Fx* fx = it->second;
    fx->Remove(*this);
    delete fx;
    return fxs.erase(it);
}

void Store::DequeueEnemy(const ID id)
{
    auto it = enemies.find(id);
    DEBUG_CODE(if (it == enemies.end()) {
        throw std::runtime_error("Attempted to dequeue a non-existent enemy.");
    })
    Enemy* enemy = it->second;
    enemy->Remove(*this);
    delete enemy;
    enemies.erase(it);
}

void Store::DequeueTower(const ID id)
{
    auto it = towers.find(id);
    DEBUG_CODE(if (it == towers.end()) {
        throw std::runtime_error("Attempted to dequeue a non-existent tower.");
    })
    Tower* tower = it->second;
    tower->Remove(*this);
    delete tower;
    towers.erase(it);
}
void Store::DequeueBullet(const ID id)
{
    auto it = bullets.find(id);
    DEBUG_CODE(if (it == bullets.end()) {
        throw std::runtime_error("Attempted to dequeue a non-existent bullet.");
    })
    Bullet* bullet = it->second;
    bullet->Remove(*this);
    delete bullet;
    bullets.erase(it);
}
void Store::DequeueSoldier(const ID id)
{
    auto it = soldiers.find(id);
    DEBUG_CODE(if (it == soldiers.end()) {
        throw std::runtime_error("Attempted to dequeue a non-existent soldier.");
    })
    Soldier* soldier = it->second;
    soldier->Remove(*this);
    delete soldier;
    soldiers.erase(it);
}

void Store::DequeueFx(const ID id)
{
    auto it = fxs.find(id);
    DEBUG_CODE(if (it == fxs.end()) {
        throw std::runtime_error("Attempted to dequeue a non-existent fx.");
    })
    Fx* fx = it->second;
    fx->Remove(*this);
    delete fx;
    fxs.erase(it);
}

Enemy* Store::GetEnemy(const ID id) const
{
    auto it = enemies.find(id);
    return it != enemies.end() ? it->second : nullptr;
}
Tower* Store::GetTower(const ID id) const
{
    auto it = towers.find(id);
    return it != towers.end() ? it->second : nullptr;
}
Bullet* Store::GetBullet(const ID id) const
{
    auto it = bullets.find(id);
    return it != bullets.end() ? it->second : nullptr;
}
Soldier* Store::GetSoldier(const ID id) const
{
    auto it = soldiers.find(id);
    return it != soldiers.end() ? it->second : nullptr;
}

Fx* Store::GetFx(const ID id) const
{
    auto it = fxs.find(id);
    return it != fxs.end() ? it->second : nullptr;
}

void Store::QueueEnemy(Enemy* enemy)
{
    enemy->id        = next_id;
    enemies[next_id] = enemy;
    ++next_id;
    enemy->Insert(*this);
}
void Store::QueueTower(Tower* tower)
{
    tower->id       = next_id;
    towers[next_id] = tower;
    ++next_id;
    tower->Insert(*this);
}
void Store::QueueBullet(Bullet* bullet)
{
    bullet->id       = next_id;
    bullets[next_id] = bullet;
    ++next_id;
    bullet->Insert(*this);
    INFO("Bullet Source Position: (" << bullet->source_position.x << ", "
                                              << bullet->source_position.y << ")");
    DEBUG_CODE(if (resource_manager.GetAnimationGroupMap()->find(bullet->animations[0].prefix) !=
                   resource_manager.GetAnimationGroupMap()->end()) {
        SUCCESS("Bullet animation prefix found: " << bullet->animations[0].prefix);
    })
}

void Store::QueueSoldier(Soldier* soldier)
{
    soldier->id       = next_id;
    soldiers[next_id] = soldier;
    ++next_id;
    soldier->Insert(*this);
}

void Store::QueueFx(Fx* fx)
{
    fx->id       = next_id;
    fxs[next_id] = fx;
    ++next_id;
    fx->Insert(*this);
}

void Store::QueueActionFx(Fx* action_fx)
{
    action_fx->id       = next_id;
    action_fxs[next_id] = action_fx;
    ++next_id;
    action_fx->Insert(*this);
}

void Store::Clear()
{
    for (auto& [id, enemy] : enemies) {
        delete enemy;
    }
    enemies.clear();
    for (auto& [id, tower] : towers) {
        delete tower;
    }
    towers.clear();
    for (auto& [id, bullet] : bullets) {
        delete bullet;
    }
    bullets.clear();
    for (auto& [id, soldier] : soldiers) {
        delete soldier;
    }
    soldiers.clear();
    for (auto& [id, fx] : fxs) {
        delete fx;
    }
    fxs.clear();
    for (auto& [id, action_fx] : action_fxs) {
        delete action_fx;
    }
    action_fxs.clear();
    damage_events.clear();
    view_data_queue.clear();
    PendingEnemyQueue empty_queue;
    pending_enemy_queue.swap(empty_queue);
}
void Store::ClearViewDataQueue()
{
    view_data_queue.clear();
}
ViewDataQueue* Store::GetViewDataQueue()
{
    return &view_data_queue;
}

void Store::InitLevel(const std::string& level_name)
{
    resource_manager.LoadLevelResources(level_name);
    InitTowers();
}

void Store::ClearFxs()
{
    for (auto& pair : fxs) {
        delete pair.second;
    }
    fxs.clear();
    for (auto& pair : action_fxs) {
        delete pair.second;
    }
    action_fxs.clear();
}

void Store::ClearActionFxs()
{
    for (auto& pair : action_fxs) {
        delete pair.second;
    }
    action_fxs.clear();
}
