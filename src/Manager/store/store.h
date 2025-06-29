#pragma once
#include "Manager/pathManager/pathManager.h"
#include "Manager/resourceManager/animationManager.h"
#include "Model/components/damage.h"
#include <memory>
#include <vector>
class Bullet;
class Tower;
class Enemy;
class Soldier;

class Store
{
public:
    double              time = 0.0;
    double              gold = 0.0;
    int                 life = 20;
    const std::vector<Enemy*>& GetEnemies() const { return enemies; }

    const std::vector<Tower*>& GetTowers() const { return towers; }

    const std::vector<Bullet*>& GetBullets() const { return bullets; }

    const std::vector<Soldier*>& GetSoldiers() const { return soldiers; }

    void QueueEnemy(Enemy* enemy) { enemies.push_back(enemy); }

    void QueueTower(Tower* tower) { towers.push_back(tower); }

    void QueueBullet(Bullet* bullet) { bullets.push_back(bullet); }

    void QueueSoldier(Soldier* soldier) { soldiers.push_back(soldier); }

    void QueueDamageEvent(DamageEvent&& event) { damage_events.push_back(std::move(event)); }

    void QueueDamageEvent(const DamageEvent& event) { damage_events.push_back(event); }

    AnimationManager animation_manager;
    PathManager      path_manager;

    void Update();

private:
    std::vector<Enemy*>      enemies;
    std::vector<Tower*>      towers;
    std::vector<Bullet*>     bullets;
    std::vector<Soldier*>    soldiers;
    // 请注意，damage_event 不是 new 出来的对象
    std::vector<DamageEvent> damage_events;
};