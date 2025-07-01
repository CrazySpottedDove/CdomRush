#include "Manager/store/store.h"
#include "Function/calc/damage.h"
#include "Function/calc/hp.h"
#include "Function/calc/motion.h"
#include "Function/events/events.h"
#include "Manager/resourceManager/animationManager.h"
#include "Model/bullets/bullets.h"
#include "Model/components/damage.h"
#include "Model/components/state.h"
#include "Model/enemies/enemies.h"
#include "Model/soldiers/soldiers.h"
#include "View/animation/animationPlayer.h"
#include "View/ui/EnemyUI.h"
#include "View/ui/UIManager.h"
#include "utils/macros.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <memory>
#include <thread>


/**
 * @brief 首先考虑结算所有可以结算的伤害事件，尽快完成生命值的更新
 * @note 发现敌人已经死亡或者从 store 中移除时，不执行伤害事件，直接删去
 *
 */
void Store::UpdateDamageEvents(sf::RenderWindow& window)
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

void Store::UpdateEnemies(sf::RenderWindow& window)
{
    auto it = enemies.begin();
    while (it != enemies.end()) {
        Enemy* enemy = it->second;
        if (calc::is_dead(*enemy) && calc::should_remove(*this, *enemy)) {
            ui_manager.DeQueueEnemyUI(enemy);
            delete enemy;
            it = enemies.erase(it);
            continue;
        }
        if (calc::enemy_reached_defence_point(*this, *enemy)) {
            life -= enemy->life_cost;
            gold += enemy->gold;
            it = DequeueEnemy(it);
            continue;
        }
        enemy->Update(*this);
        ui_manager.RenderEnemyUI(window, enemy);
        ++it;
    }
}

void Store::UpdateBullets(sf::RenderWindow& window)
{
    auto it = bullets.begin();
    while (it != bullets.end()) {
        Bullet* bullet = it->second;
        if (bullet->animation.state == State::Hit &&
            animation_manager
                    .RequireAnimationGroup(bullet->animation.prefix, bullet->animation.state)
                    .to <= bullet->animation.frame_id) {
            bullet->Remove(*this);
            ui_manager.DeQueueBulletUI(bullet);
            delete bullet;
            it = bullets.erase(it);
            continue;
        }
        bullet->Update(*this);
        ui_manager.RenderBulletUI(window, bullet);
        ++it;
    }
}

void Store::UpdateSoldiers(sf::RenderWindow& window)
{
    auto it = soldiers.begin();
    while (it != soldiers.end()) {
        Soldier* soldier = it->second;
        if (calc::is_dead(*soldier) && calc::should_remove(*this, *soldier)) {
            soldier->Remove(*this);
            ui_manager.DeQueueSoldierUI(soldier);
            delete soldier;
            it = soldiers.erase(it);
            continue;
        }
        soldier->Update(*this);
        ui_manager.RenderSoldierUI(window, soldier);
        ++it;
    }
}

/**
 * @brief 删除和购买塔应该通过事件来处理，塔的 update 中因此没有删除这一项
 *
 * @param window
 */
void Store::UpdateTowers(sf::RenderWindow& window)
{
    auto it = towers.begin();
    while (it != towers.end()) {
        Tower* tower = it->second;
        tower->Update(*this);
        ui_manager.RenderTowerUI(window, tower);
        ++it;
    }
}

void Store::ExecuteEvents()
{
    while (!event_queue.empty()) {
        const Event event = std::move(event_queue.front());
        event_queue.pop();
        switch (event.type) {
        case EventType::SelectLevel:
            callback::SelectLevel(*this, std::get<std::string>(event.data.prop1));
            game_state = GameState::Loading;
            break;
        }
    }
}

void Store::Update(sf::RenderWindow& window)
{
    UpdateDamageEvents(window);
    UpdateEnemies(window);
    UpdateBullets(window);
    UpdateSoldiers(window);
    UpdateTowers(window);
}

void Store::Game(sf::RenderWindow& window)
{
    static constexpr int FRAME_LENGTH_IN_MILLISECONDS = FRAME_LENGTH * 1000;
    while (true) {
        switch (game_state) {
        case GameState::Begin:
            window.clear();
            ui_manager.RenderMap(window, "map_background");
            window.display();
            ExecuteEvents();
            DEBUG_CODE(game_state = GameState::Loading;)
            break;
        case GameState::GameStart:
            window.clear();
            ui_manager.RenderMap(window, level_manager.GetCurrentLevelName());
            Update(window);
            window.display();
            time += FRAME_LENGTH;
            ExecuteEvents();
            DEBUG_CODE(game_state = GameState::GamePlaying;)
            break;
        case GameState::GamePlaying:
            wave_manager.Update(*this);
            window.clear();
            ui_manager.RenderMap(window, level_manager.GetCurrentLevelName());
            Update(window);
            time += FRAME_LENGTH;
            window.display();
            ExecuteEvents();
            break;
        case GameState::GameOver:
            // AnimationPlayer::DrawGameOver();
            ExecuteEvents();
            break;
        case GameState::Loading:
            // AnimationPlayer::DrawLoading();
            DEBUG_CODE(callback::SelectLevel(*this, "acaroth");)
            level_manager.LoadLevelResource(*this);
            game_state = GameState::GameStart;
            time       = 0.0;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_LENGTH_IN_MILLISECONDS));
    }
}

Store::Store()
{
    ui_manager.store_            = this;
    ui_manager.animation_player_ = std::make_unique<AnimationPlayer>(animation_manager);
}
void Store::QueueSoldier(Soldier* soldier)
{
    soldiers[next_id++] = soldier;
    soldier->Insert(*this);
    ui_manager.QueueSoldierUI(soldier);
}
std::unordered_map<ID, Enemy*>::iterator Store::DequeueEnemy(
    std::unordered_map<ID, Enemy*>::iterator& it)
{
    Enemy* enemy = it->second;
    enemy->Remove(*this);
    ui_manager.DeQueueEnemyUI(enemy);
    delete enemy;
    return enemies.erase(it);
};
std::unordered_map<ID, Tower*>::iterator Store::DequeueTower(
    std::unordered_map<ID, Tower*>::iterator& it)
{
    Tower* tower = it->second;
    tower->Remove(*this);
    ui_manager.DeQueueTowerUI(tower);
    delete tower;
    return towers.erase(it);
};
std::unordered_map<ID, Bullet*>::iterator Store::DequeueBullet(
    std::unordered_map<ID, Bullet*>::iterator& it)
{
    Bullet* bullet = it->second;
    bullet->Remove(*this);
    ui_manager.DeQueueBulletUI(bullet);
    delete bullet;
    return bullets.erase(it);
};
std::unordered_map<ID, Soldier*>::iterator Store::DequeueSoldier(
    std::unordered_map<ID, Soldier*>::iterator& it)
{
    Soldier* soldier = it->second;
    soldier->Remove(*this);
    ui_manager.DeQueueSoldierUI(soldier);
    delete soldier;
    return soldiers.erase(it);
};
void Store::DequeueEnemy(const ID id)
{
    auto it = enemies.find(id);
    DEBUG_CODE(if (it == enemies.end()) {
        throw std::runtime_error("Attempted to dequeue a non-existent enemy.");
    })
    Enemy* enemy = it->second;
    enemy->Remove(*this);
    ui_manager.DeQueueEnemyUI(enemy);
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
    ui_manager.DeQueueTowerUI(tower);
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
    ui_manager.DeQueueBulletUI(bullet);
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
    ui_manager.DeQueueSoldierUI(soldier);
    delete soldier;
    soldiers.erase(it);
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
void Store::QueueEnemy(Enemy* enemy)
{
    enemies[next_id++] = enemy;
    enemy->Insert(*this);
    ui_manager.QueueEnemyUI(enemy);
}
void Store::QueueTower(Tower* tower)
{
    towers[next_id++] = tower;
    tower->Insert(*this);
    ui_manager.QueueTowerUI(tower);
}
void Store::QueueBullet(Bullet* bullet)
{
    bullets[next_id++] = bullet;
    bullet->Insert(*this);
    ui_manager.QueueBulletUI(bullet);
}
