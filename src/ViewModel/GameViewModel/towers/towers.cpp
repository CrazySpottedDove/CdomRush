#include "ViewModel/GameViewModel/towers/towers.h"
#include "ViewModel/GameViewModel/components/ranged.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"

#include "ViewModel/GameViewModel/store/store.h"


void Tower::Update(Store& store){
    if(animation.current_state == State::Idle){
        for(auto& attack : ranged.attacks){
            if(attack.IsReady(store)) {
                ID target_enemy = calc::find_foremost_enemy(store, position, attack.range);
                Enemy* target_enemy_ptr = store.GetEnemy(target_enemy);

                if(target_enemy == INVALID_ID) continue; // 如果没有找到目标敌人，跳过
                if(target_enemy_ptr == nullptr) continue;

                animation.current_state = State::Shoot; // 设置状态为射击
                if(target_enemy_ptr->position.y < position.y) heading = tower_heading::Up; // 如果目标敌人在塔的上方，设置塔的朝向为 Up
                else heading = tower_heading::Down; // 如果目标敌人在塔的下方，设置塔的朝向为 Down
                attack.Apply(store, this->id, target_enemy, return_offset(),SourceType::Tower); // 应用攻击
                layer_update();
                return ;
            }
        }
    }
    else if(animation.current_state == State::Shoot){
        if(animation.pending == false) {
            animation.current_state = State::Idle; // 如果动画未进行，设置状态为闲置
        }
    }
    layer_update();
    return ;
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
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "build_terrain_0001"},Position{0.0f, 12.0f}};
    Layers.push_back(layer1);
    animation.current_state = State::Idle; // 设置初始状态为闲置
}


void Archer::layer_update() {
    if(animation.current_state == State::Idle && heading == tower_heading::Down){
        Layers[3].animation.current_state = State::IdleDown; // 设置朝下的动画状态
        Layers[4].animation.current_state = State::IdleDown; // 设置朝下的动画
    }
    else if(animation.current_state == State::Idle && heading == tower_heading::Up){
        Layers[3].animation.current_state = State::IdleUp; // 设置朝上的动画状态
        Layers[4].animation.current_state = State::IdleUp; // 设置朝上的动画
    }
    else if(animation.current_state == State::Shoot && heading == tower_heading::Down){
        if(shooter){
            Layers[3].animation.current_state = State::ShootingDown; // 设置射击朝下的动画状态
            Layers[4].animation.current_state = State::IdleDown; // 设置射击朝下的动画
        }
        else{
            Layers[3].animation.current_state = State::IdleDown; // 设置射击朝下的动画状态
            Layers[4].animation.current_state = State::ShootingDown; // 设置射击朝下的动画
        }
        shooter = ~ shooter; // 切换射手状态
    }
    else if(animation.current_state == State::Shoot && heading == tower_heading::Up){
        if(shooter){
            Layers[3].animation.current_state = State::ShootingUp; // 设置射击朝上的动画状态
            Layers[4].animation.current_state = State::IdleUp; // 设置射击朝上的动画
        }
        else{
            Layers[3].animation.current_state = State::IdleUp; // 设置射击朝上的动画状态
            Layers[4].animation.current_state = State::ShootingUp; // 设置射击朝上的动画
        }
        shooter = ~ shooter; // 切换射手状态
    }
}
Archer1::Archer1(Position position_, int total_price_) {
    type = TowerType::Archer1; // 设置塔类型为弓箭手1
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Archer2, 110}}, // 70,110,160
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(0.8, 140.0, BulletType::Arrow, 0.0, 5.5, "arrow", 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_archer_%04i"},Position{0.0f, 12.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "archer_tower_0001"},Position{0.0f, 37.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl1"},Position{-9.0f, 51.0f}};
    Layer layer4{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl1"},Position{9.0f, 51.0f}};

    Layers.push_back(layer1);Layers.push_back(layer2);Layers.push_back(layer3);Layers.push_back(layer4);

    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}
Archer2::Archer2(Position position_, int total_price_) {
    type = TowerType::Archer2; // 设置塔类型为弓箭手2
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Archer3, 160}}, // 70,110,160
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(0.6, 160.0, BulletType::Arrow, 0.0, 10.0, "arrow", 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_archer_%04i"},Position{0.0f, 12.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "archer_tower_0002"},Position{0.0f, 37.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl2"},Position{-9.0f, 52.0f}};
    Layer layer4{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl2"},Position{9.0f, 52.0f}};

    Layers.push_back(layer1);Layers.push_back(layer2);Layers.push_back(layer3);Layers.push_back(layer4);

    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}
Archer3::Archer3(Position position_, int total_price_) {
    type = TowerType::Archer3; // 设置塔类型为弓箭手3
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(0.5, 180.0, BulletType::Arrow, 0.0, 15.0, "arrow", 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_archer_%04i"},Position{0.0f, 12.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "archer_tower_0003"},Position{0.0f, 37.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl3"},Position{-9.0f, 52.0f}};
    Layer layer4{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl3"},Position{9.0f, 52.0f}};

    Layers.push_back(layer1);
    Layers.push_back(layer2);
    Layers.push_back(layer3);
    Layers.push_back(layer4);

    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}

void Engineer::layer_update(){
    if(animation.current_state == State::Idle){
        Layers[2].animation.current_state = State::Idle; // 设置工程师的闲置
        Layers[3].animation.current_state = State::Idle; // 设置工程师的闲置
        Layers[4].animation.current_state = State::Idle; // 设置工程师的闲置
        Layers[5].animation.current_state = State::Idle; // 设置工程师的闲置
        Layers[6].animation.current_state = State::Idle; // 设置工程师的闲置
        Layers[7].animation.current_state = State::Idle; // 设置工程师的闲置
        Layers[8].animation.current_state = State::Idle; // 设置工程师的闲置
    }
    else{
        Layers[2].animation.current_state = State::Shoot; // 设置工程师的闲置
        Layers[3].animation.current_state = State::Shoot; // 设置工程师的闲置
        Layers[4].animation.current_state = State::Shoot; // 设置工程师的闲置
        Layers[5].animation.current_state = State::Shoot; // 设置工程师的闲置
        Layers[6].animation.current_state = State::Shoot; // 设置工程师的闲置
        Layers[7].animation.current_state = State::Shoot; // 设置工程师的闲置
        Layers[8].animation.current_state = State::Shoot; // 设置工程师的闲置
    }
    return ;
}
Engineer1::Engineer1(Position position_, int total_price_) {
    type = TowerType::Engineer1; // 设置塔类型为工程师1
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Engineer2, 220}}, // 125,220,320
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(3.0, 160.0, BulletType::Bomb, 62.4, 13.0, "bombs_0001", 1.0)); // 添加攻击

    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_artillery_%04i"},Position{0.0f, 15.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl1_layer1"},Position{0.0f, 41.0f}};
    Layer layer3{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl1_layer2"},Position{0.0f, 41.0f}};
    Layer layer4{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl1_layer3"},Position{0.0f, 41.0f}};
    Layer layer5{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl1_layer4"},Position{0.0f, 41.0f}};
    Layer layer6{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl1_layer5"},Position{0.0f, 41.0f}};
    Layer layer7{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl1_layer6"},Position{0.0f, 41.0f}};
    Layer layer8{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl1_layer7"},Position{0.0f, 41.0f}};

    Layers.push_back(layer1);Layers.push_back(layer2);Layers.push_back(layer3);Layers.push_back(layer4);
    Layers.push_back(layer5);Layers.push_back(layer6);Layers.push_back(layer7);Layers.push_back(layer8);

    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}
Engineer2::Engineer2(Position position_, int total_price_) {
    type = TowerType::Engineer2; // 设置塔类型为工程师2
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Engineer3, 320}}, // 125,220,320
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(3, 160.0, BulletType::Bomb, 62.4, 33.0, "bombs_0002", 1.0)); // 添加攻击

    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_artillery_%04i"},Position{0.0f, 15.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl2_layer1"},Position{0.0f, 42.0f}};
    Layer layer3{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl2_layer2"},Position{0.0f, 42.0f}};
    Layer layer4{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl2_layer3"},Position{0.0f, 42.0f}};
    Layer layer5{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl2_layer4"},Position{0.0f, 42.0f}};
    Layer layer6{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl2_layer5"},Position{0.0f, 42.0f}};
    Layer layer7{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl2_layer6"},Position{0.0f, 42.0f}};
    Layer layer8{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl2_layer7"},Position{0.0f, 42.0f}};

    Layers.push_back(layer1);Layers.push_back(layer2);Layers.push_back(layer3);Layers.push_back(layer4);
    Layers.push_back(layer5);Layers.push_back(layer6);Layers.push_back(layer7);Layers.push_back(layer8);
    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
};
Engineer3::Engineer3(Position position_, int total_price_) {
    type = TowerType::Engineer3; // 设置塔类型为工程师3
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(2.5, 180.0, BulletType::Bomb, 67.2, 50.0, "bombs_0002", 1.0)); // 添加攻击

    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_artillery_%04i"},Position{0.0f, 15.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl3_layer1"},Position{0.0f, 43.0f}};
    Layer layer3{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl3_layer2"},Position{0.0f, 43.0f}};
    Layer layer4{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl3_layer3"},Position{0.0f, 43.0f}};
    Layer layer5{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl3_layer4"},Position{0.0f, 43.0f}};
    Layer layer6{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl3_layer5"},Position{0.0f, 43.0f}};
    Layer layer7{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl3_layer6"},Position{0.0f, 43.0f}};
    Layer layer8{Animation{State::Idle, 0.0, true, 0, "towerengineerlvl3_layer7"},Position{0.0f, 43.0f}};

    Layers.push_back(layer1);Layers.push_back(layer2);Layers.push_back(layer3);Layers.push_back(layer4);
    Layers.push_back(layer5);Layers.push_back(layer6);Layers.push_back(layer7);Layers.push_back(layer8);
    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
};

void Mage::layer_update() {
    if(animation.current_state == State::Idle && heading == tower_heading::Down){
        Layers[2].animation.current_state = State::Idle; // 设置朝下的动画状态
        Layers[3].animation.current_state = State::IdleDown; // 设置朝下的动画
    }
    else if(animation.current_state == State::Idle && heading == tower_heading::Up){
        Layers[2].animation.current_state = State::Idle; // 设置朝上的动画状态
        Layers[3].animation.current_state = State::IdleUp; // 设置朝上的动画
    }
    else if(animation.current_state == State::Shoot && heading == tower_heading::Down){
        Layers[2].animation.current_state = State::Shoot; // 设置射击朝下的动画状态
        Layers[3].animation.current_state = State::ShootingDown; // 设置射击朝下的动画
    }
    else if(animation.current_state == State::Shoot && heading == tower_heading::Up){
        Layers[2].animation.current_state = State::Shoot; // 设置射击朝上的动画状态
        Layers[3].animation.current_state = State::ShootingUp; // 设置射击朝上的动画
    }
}
Mage1::Mage1(Position position_, int total_price_) {
    type = TowerType::Mage1; // 设置塔类型为法师1
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Mage2, 160}}, // 100,160,240
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(1.5, 140.0, BulletType::Bolt, 0.0, 15, "bolt_1", 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_mage_%04i"},Position{0.0f, 15.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "towermagelvl1"},Position{0.0f, 30.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shootermage"},Position{1.0f, 57.0f}};

    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}
Mage2::Mage2(Position position_, int total_price_) {
    type = TowerType::Mage2; // 设置塔类型为法师2
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Mage3, 240}}, // 100,160,240
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(1.5, 160.0, BulletType::Bolt, 0.0, 36, "bolt_2", 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_mage_%04i"},Position{0.0f, 15.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "towermagelvl2"},Position{0.0f, 30.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shootermage"},Position{1.0f, 57.0f}};

    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}
Mage3::Mage3(Position position_, int total_price_) {
    type = TowerType::Mage3; // 设置塔类型为法师3
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(1.5, 180.0, BulletType::Bolt, 0.0, 63, "bolt_3", 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_mage_%04i"},Position{0.0f, 15.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "towermagelvl3"},Position{0.0f, 30.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shootermage"},Position{1.0f, 62.0f}};

    animation.current_state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}