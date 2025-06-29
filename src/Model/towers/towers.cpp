#include "Model/towers/towers.h"
#include "Model/components/ranged.h"

class None : public Tower {
public:
    None() = default; // 默认构造函数
    None(Position position_) {
        type = TowerType::None;
        position = position_;
        total_price = 0;
        tower_actions = {
            {TowerAction::Upgrade, {TowerType::Archer1, 70}},//70,110,160
            {TowerAction::Upgrade, {TowerType::Engineer1, 125}},//125,220,320
            {TowerAction::Upgrade, {TowerType::Mage1, 100}}//100,160,240
        };
    }
};


class Archer1 : public Tower {
public:
    Archer1(){
        type = TowerType::Archer1; // 设置塔类型为弓箭手1
    }

    Archer1(Position position_, int total_price_) {
        type = TowerType::Archer1; // 设置塔类型为弓箭手1
        position = position_;
        total_price = total_price_;
        tower_actions = {
            {TowerAction::Upgrade, {TowerType::Archer2, 70}},
            {TowerAction::Sell, {TowerType::None, 0 }},
        };
        ranged.attacks.push_back(RangedAttack(0.5, 150.0, BulletType::Arrow,0.0,5.5, 1.0)); // 添加攻击
    }

    void layer_update() override {
        // 更新图层逻辑
        for (auto& layer : Layers) {
            // layer.animation.Update(); // 更新每个图层的动画
        }
    }

    void shoot_bullet(RangedAttack& attack,Store& store,Enemy* & target_enemy) override {
        attack.Apply(store, target_enemy, this); // 应用攻击效果
    }
};