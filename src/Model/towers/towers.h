#pragma once
#include "Model/templates/activeEntity.h"
#include "utils/macros.h"
#include "Model/components/ranged.h"

class Store;

class Tower;
enum class TowerType{
    None,
    Archer1,
    Archer2,
    Archer3,
    Engineer1,
    Engineer2,
    Engineer3,
    Mage1,
    Mage2,
    Mage3,
    Barrack1,
    Barrack2,
    Barrack3
};

enum class TowerAction{
    Upgrade,
    Sell,
    ChangeRally
};

struct Layer{
    Animation animation;
    Position offset;
};

enum class tower_heading{
    Up,
    Down
};
// TODO: 每个 Tower 应该维护一个 Action 列表，这个列表可以被 View 层访问到，从而让 View 层得知需要怎样的 UI 更新
class Tower : public ActiveEntity
{
public:
    tower_heading heading = tower_heading::Down; // 默认塔的朝向为 Up
    Ranged ranged;
    TowerType type = TowerType::None; // 默认塔类型为 None
    Position            position;
    int                total_price;
    // 每个 Tower 的操作列表，每个操作会对应一个新的tower类型和一个价格:
    std::vector <std::pair<TowerAction, std::pair<TowerType, int>>> tower_actions;
    // 每个 Tower 的图层列表:
    std::vector<Layer> Layers;
    
    bool Insert(Store& store) override {
        return true;
    }
    bool Remove(Store& store) override {
        return true;
    }
    virtual void layer_update() = 0; // 每个 Tower 都需要实现自己的图层更新逻辑
    
    void Update(Store& store) override;

    Tower(const Tower&) = default; // 拷贝构造函数
    Tower() = default; // 默认构造函数

    virtual Tower* Clone() = 0;
};

class None : public Tower {
public:
    None(Position position);
    None() = delete;
    void layer_update() override{};
};

class Archer : public Tower {
public:
    Archer() = default; // 显式声明为public
    void layer_update() override;
};

class Archer1 : public Archer {
public:
    Archer1(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Archer1(*this);
    }
};

class Archer2 : public Archer {
public:
    Archer2(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Archer2(*this);
    }
};

class Archer3 : public Archer {
public:
    Archer3(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Archer3(*this);
    }
};