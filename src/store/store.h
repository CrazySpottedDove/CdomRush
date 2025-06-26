#pragma once
#include <memory>
#include <vector>
class Bullet;
class Tower;
class Enemy;
class Soldier;

class Store
{
public:
    double                              time = 0.0;
    std::vector<std::shared_ptr<Enemy>> GetEnemies() { return enemies; }

    std::vector<std::shared_ptr<Tower>> GetTowers() { return towers; }

    std::vector<std::shared_ptr<Bullet>> GetBullets() { return bullets; }

    std::vector<std::shared_ptr<Soldier>> GetSoldiers() { return soldiers; }

    void AddEnemy(std::shared_ptr<Enemy> enemy) { enemies.push_back(enemy); }

    void AddTower(std::shared_ptr<Tower> tower) { towers.push_back(tower); }

    void AddBullet(std::shared_ptr<Bullet> bullet) { bullets.push_back(bullet); }

    void AddSoldier(std::shared_ptr<Soldier> soldier) { soldiers.push_back(soldier); }

    void Update();
private:
    std::vector<std::shared_ptr<Enemy>>   enemies;
    std::vector<std::shared_ptr<Tower>>   towers;
    std::vector<std::shared_ptr<Bullet>>  bullets;
    std::vector<std::shared_ptr<Soldier>> soldiers;
};