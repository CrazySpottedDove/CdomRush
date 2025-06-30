#include "Model/towers/towers.h"
#include "Model/components/ranged.h"
#include "Model/enemies/enemies.h"


void Tower::Update(Store& store){
    if(animation.state == State::Idle){
        for(auto& attack : ranged.attacks){
            if(attack.IsReady(store)) {
                Enemy* target_enemy = calc::find_foremost_enemy(store, position, attack.range);
                if(target_enemy == nullptr) continue; // 如果没有找到目标敌人，跳过
                animation.state = State::Shoot; // 设置状态为射击
                attack.Apply(store, target_enemy, this);
                if(target_enemy->position.y < position.y) heading = tower_heading::Up; // 如果目标敌人在塔的上方，设置塔的朝向为 Up
                else heading = tower_heading::Down; // 如果目标敌人在塔的下方，设置塔的朝向为 Down
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


None::None(Position position_) {
    type = TowerType::None;
    position = position_;
    total_price = 0;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Archer1, 70}}, // 70,110,160
        {TowerAction::Upgrade, {TowerType::Engineer1, 125}}, // 125,220,320
        {TowerAction::Upgrade, {TowerType::Mage1, 100}} // 100,160,240
    };
}


Archer1::Archer1(Position position_, int total_price_) {
    type = TowerType::Archer1; // 设置塔类型为弓箭手1
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Archer2, 110}}, // 70,110,160
        {TowerAction::Sell, {TowerType::None, 0 }},
    };
    ranged.attacks.push_back(RangedAttack(0.5, 150.0, BulletType::Arrow, 0.0, 5.5, 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_archer_%04i"},Position{0.0f, 12.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "archer_tower_0001"},Position{0.0f, 37.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl1"},Position{-9.0f, 51.0f}};
    Layer layer4{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl1"},Position{9.0f, 51.0f}};

    Layers.push_back(layer1);
    Layers.push_back(layer2);
    Layers.push_back(layer3);
    Layers.push_back(layer4);

    animation.state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}

void Archer1::layer_update() {
    if(animation.state == State::Idle && heading == tower_heading::Down){
        Layers[3].animation.state = State::IdleDown; // 设置朝下的动画状态
        Layers[4].animation.state = State::IdleDown; // 设置朝下的动画
    }
    else if(animation.state == State::Idle && heading == tower_heading::Up){
        Layers[3].animation.state = State::IdleUp; // 设置朝上的动画状态
        Layers[4].animation.state = State::IdleUp; // 设置朝上的动画
    }
    else if(animation.state == State::Shoot && heading == tower_heading::Down){
        Layers[3].animation.state = State::ShootingDown; // 设置射击朝下的动画状态
        Layers[4].animation.state = State::ShootingDown; // 设置射击朝下的动画
    }
    else if(animation.state == State::Shoot && heading == tower_heading::Up){
        Layers[3].animation.state = State::ShootingUp; // 设置射击朝上的动画状态
        Layers[4].animation.state = State::ShootingUp; // 设置射击朝上的动画
    }
}