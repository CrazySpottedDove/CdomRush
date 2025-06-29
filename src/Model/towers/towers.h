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
// TODO: 每个 Tower 应该维护一个 Action 列表，这个列表可以被 View 层访问到，从而让 View 层得知需要怎样的 UI 更新
class Tower : public ActiveEntity
{
public:
    Ranged ranged;
    TowerType type = TowerType::None; // 默认塔类型为 None
    Position            position;
    int                total_price;
    std::vector <std::pair<TowerAction, std::pair<TowerType, int>>> tower_actions;
    std::vector<Layer> Layers; // 每个 Tower 的图层列表
    // 每个 Tower 的操作列表，每个操作会对应一个新的tower类型和一个价格
    bool Insert(Store& store) override {
        return true;
    }
    bool Remove(Store& store) override {
        return true;
    }
    virtual void layer_update() = 0; // 每个 Tower 都需要实现自己的图层更新逻辑
    void Update(Store& store) override {
        if(animation.state == State::Idle){
            for(auto& attack : ranged.attacks){
                if(attack.IsReady(store)) {
                    Enemy* target_enemy = calc::find_foremost_enemy(store, position, attack.range);
                    if(target_enemy == nullptr) continue; // 如果没有找到目标敌人，跳过
                    animation.state = State::Shoot; // 设置状态为射击

                    attack.Apply(store, target_enemy); // 执行攻击
                }
            }
        }
        else if(animation.state == State::Shoot){
            if(animation.pending == false) {
                animation.state = State::Idle; // 如果动画未进行，设置状态为闲置
            }
        }
        layer_update();
    }
};

class None : public Tower {
public:
    None(Position position);
    None() = default;
};

class ArcherTower : public Tower {};