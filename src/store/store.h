#pragma once
#include <memory>
#include <vector>
class Bullet;
class Tower;
class Enemy;
class Soldier;

class Store{
public:
    double time = 0.0;
private:
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Tower>> towers;
    std::vector<std::shared_ptr<Bullet>> bullets;
    std::vector<std::shared_ptr<Soldier>> soldiers;
};