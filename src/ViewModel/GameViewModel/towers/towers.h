#pragma once
#include "ViewModel/GameViewModel/templates/activeEntity.h"
#include "Common/macros.h"
#include "ViewModel/GameViewModel/components/ranged.h"
#include "Common/type.h"
class Store;
class Soldier;
class Tower;


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
    Position rally_point = sf::Vector2f(0,0); // 集结点位置
    tower_heading heading = tower_heading::Down; // 默认塔的朝向为 Up
    Ranged ranged;
    TowerType type = TowerType::None; // 默认塔类型为 None
    int                total_price;

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

    virtual Position return_offset() const {
        return sf::Vector2f(0, 0); // 默认偏移位置为 (0, 0)
    }
};

class None : public Tower {
public:
    None(Position position = sf::Vector2f(0,0));
    void layer_update() override{};
    Tower* Clone() override {
        return new None(*this);
    }
};

class Archer : public Tower {
public:
    bool shooter = 1;
    Archer() = default; // 显式声明为public
    void layer_update() override;
};
class Archer1 : public Archer {
public:
    Archer1(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Archer1(*this);
    }
    Position return_offset() const override {
        if(shooter) return sf::Vector2f(-10, 50); // 特定偏移位置
        else return sf::Vector2f(10, 50); // 特定偏移位置
    }
};
class Archer2 : public Archer {
public:
    Archer2(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Archer2(*this);
    }
    Position return_offset() const override {
        if(shooter) return sf::Vector2f(-10, 50); // 特定偏移位置
        else return sf::Vector2f(10, 50); // 特定偏移位置
    }
};
class Archer3 : public Archer {
public:
    Archer3(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Archer3(*this);
    }
    Position return_offset() const override {
        if(shooter) return sf::Vector2f(-10, 50); // 特定偏移位置
        else return sf::Vector2f(10, 50); // 特定偏移位置
    }
};

class Engineer : public Tower {
public:
    Engineer() = default; // 显式声明为public
    void layer_update() override;
};
class Engineer1 : public Engineer {
public:
    Engineer1(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Engineer1(*this);
    }
    Position return_offset() const override {
        return sf::Vector2f(0, 50); // 特定偏移位置
    }
};
class Engineer2 : public Engineer {
public:
    Engineer2(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Engineer2(*this);
    }
    Position return_offset() const override {
        return sf::Vector2f(0,53); // 特定偏移位置
    }
};
class Engineer3 : public Engineer {
public:
    Engineer3(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Engineer3(*this);
    }
    Position return_offset() const override {
        return sf::Vector2f(0,57); // 特定偏移位置
    }
};

class Mage : public Tower {
public:
    Mage() = default; // 显式声明为public
    void layer_update() override;
};
class Mage1 : public Mage {
public:
    Mage1(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Mage1(*this);
    }
    Position return_offset() const override {
        if(heading == tower_heading::Up) return sf::Vector2f(8, 66); // 特定偏移位置
        else return sf::Vector2f(-5, 62); // 特定偏移位置
    }
};
class Mage2 : public Mage {
public:
    Mage2(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Mage2(*this);
    }
    Position return_offset() const override {
        if(heading == tower_heading::Up) return sf::Vector2f(8, 66); // 特定偏移位置
        else return sf::Vector2f(-5, 64); // 特定偏移位置
    }
};
class Mage3 : public Mage {
public:
    Mage3(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Mage3(*this);
    }
    Position return_offset() const override {
        if(heading == tower_heading::Up) return sf::Vector2f(8, 70); // 特定偏移位置
        else return sf::Vector2f(-5, 69); // 特定偏移位置
    }
};

class Barracks : public Tower{
public:
    Position rally_point;
    std::vector<ID> soldiers;

    void layer_update() override;
    void Update(Store& store);
    virtual SoldierType return_soldier_type() = 0;
};
class Barracks1 : public Barracks{
public:
    Barracks1(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Barracks1(*this);
    }
    SoldierType return_soldier_type() override{
        return SoldierType::SoldierMeleelv1;
    }
};
class Barracks2 : public Barracks{
public:
    Barracks2(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Barracks2(*this);
    }
    SoldierType return_soldier_type() override{
        return SoldierType::SoldierMeleelv1;
    }
};
class Barracks3 : public Barracks{
public:
    Barracks3(Position position = sf::Vector2f(0,0), int total_price = 0);
    Tower* Clone() override {
        return new Barracks3(*this);
    }
    SoldierType return_soldier_type() override{
        return SoldierType::SoldierMeleelv1;
    }
};