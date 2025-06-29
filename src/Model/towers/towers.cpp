#include "Model/towers/towers.h"

class None : public Tower {
public:
    None() = default; // 默认构造函数
    None(Position position_) {
        position = position_;
        total_price = 0;
        tower_actions = {
            {TowerAction::Upgrade, {TowerType::Archer1, 70}},
            {TowerAction::Upgrade, {TowerType::Engineer1, 125}},//125,220,320
            {TowerAction::Upgrade, {TowerType::Mage1, 70}}
        };
    }
};