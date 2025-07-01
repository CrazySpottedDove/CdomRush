#include "Model/towers/towers.h"
#include "Model/components/ranged.h"
#include "Model/enemies/enemies.h"

#include "Manager/store/store.h"


void Tower::update(Store& store){
    if(animation.state == State::Idle){
        for(auto& attack : ranged.attacks){
            if(attack.IsReady(store)) {
                ID target_enemy = calc::find_foremost_enemy(store, position, attack.range);
                Enemy* target_enemy_ptr = store.GetEnemy(target_enemy);

                if(target_enemy == INVALID_ID) continue; // 如果没有找到目标敌人，跳过
                if(target_enemy_ptr == nullptr) continue; 

                animation.state = State::Shoot; // 设置状态为射击
                attack.Apply(store, this->id, target_enemy,return_prefix()); // 应用攻击
                if(target_enemy_ptr->position.y < position.y) heading = tower_heading::Up; // 如果目标敌人在塔的上方，设置塔的朝向为 Up
                else heading = tower_heading::Down; // 如果目标敌人在塔的下方，设置塔的朝向为 Down

                return ;
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
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "build_terrain_0001"},Position{0.0f, 12.0f}};
    Layers.push_back(layer1);
    animation.state = State::Idle; // 设置初始状态为闲置
}


void Archer::layer_update() {
    if(animation.state == State::Idle && heading == tower_heading::Down){
        Layers[3].animation.state = State::IdleDown; // 设置朝下的动画状态
        Layers[4].animation.state = State::IdleDown; // 设置朝下的动画
    }
    else if(animation.state == State::Idle && heading == tower_heading::Up){
        Layers[3].animation.state = State::IdleUp; // 设置朝上的动画状态
        Layers[4].animation.state = State::IdleUp; // 设置朝上的动画
    }
    else if(animation.state == State::Shoot && heading == tower_heading::Down){
        shooter = ~ shooter; // 切换射手状态
        if(shooter){
            Layers[3].animation.state = State::ShootingDown; // 设置射击朝下的动画状态
            Layers[4].animation.state = State::IdleDown; // 设置射击朝下的动画
            ranged.attacks[0].bullet_start_offset = Position{10.0f, 50.0f}; // 设置子弹起始偏移位置
        }
        else{
            Layers[3].animation.state = State::IdleDown; // 设置射击朝下的动画状态
            Layers[4].animation.state = State::ShootingDown; // 设置射击朝下的动画
            ranged.attacks[0].bullet_start_offset = Position{-10.0f, 50.0f}; // 设置子弹起始偏移位置
        }
    }
    else if(animation.state == State::Shoot && heading == tower_heading::Up){
        shooter = ~ shooter; // 切换射手状态
        if(shooter){
            Layers[3].animation.state = State::ShootingUp; // 设置射击朝上的动画状态
            Layers[4].animation.state = State::IdleUp; // 设置射击朝上的动画
            ranged.attacks[0].bullet_start_offset = Position{10.0f, 50.0f}; // 设置子弹起始偏移位置
        }
        else{
            Layers[3].animation.state = State::IdleUp; // 设置射击朝上的动画状态
            Layers[4].animation.state = State::ShootingUp; // 设置射击朝上的动画
            ranged.attacks[0].bullet_start_offset = Position{-10.0f, 50.0f}; // 设置子弹起始偏移位置
        }
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
    ranged.attacks.push_back(RangedAttack(0.8, 140.0, BulletType::Arrow, 0.0, 5.5, sf::Vector2f(-10,50), 1.0)); // 添加攻击
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
Archer2::Archer2(Position position_, int total_price_) {
    type = TowerType::Archer2; // 设置塔类型为弓箭手2
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Upgrade, {TowerType::Archer3, 160}}, // 70,110,160
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(0.6, 160.0, BulletType::Arrow, 0.0, 10.0, sf::Vector2f(-10,50), 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_archer_%04i"},Position{0.0f, 12.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "archer_tower_0002"},Position{0.0f, 37.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl2"},Position{-9.0f, 52.0f}};
    Layer layer4{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl2"},Position{9.0f, 52.0f}};

    Layers.push_back(layer1);
    Layers.push_back(layer2);
    Layers.push_back(layer3);
    Layers.push_back(layer4);

    animation.state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}
Archer3::Archer3(Position position_, int total_price_) {
    type = TowerType::Archer3; // 设置塔类型为弓箭手3
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(0.5, 180.0, BulletType::Arrow, 0.0, 15.0, sf::Vector2f(-10,50), 1.0)); // 添加攻击
    Layer layer1{Animation{State::Idle, 0.0, true, 0, "terrain_archer_%04i"},Position{0.0f, 12.0f}};
    Layer layer2{Animation{State::Idle, 0.0, true, 0, "archer_tower_0003"},Position{0.0f, 37.0f}};
    Layer layer3{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl3"},Position{-9.0f, 52.0f}};
    Layer layer4{Animation{State::IdleDown, 0.0, true, 0, "shooterarcherlvl3"},Position{9.0f, 52.0f}};

    Layers.push_back(layer1);
    Layers.push_back(layer2);
    Layers.push_back(layer3);
    Layers.push_back(layer4);

    animation.state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
}

void Engineer::layer_update(){
    if(animation.state == State::Idle){
        Layers[2].animation.state = State::Idle; // 设置工程师的闲置
        Layers[3].animation.state = State::Idle; // 设置工程师的闲置
        Layers[4].animation.state = State::Idle; // 设置工程师的闲置 
        Layers[5].animation.state = State::Idle; // 设置工程师的闲置
        Layers[6].animation.state = State::Idle; // 设置工程师的闲置
        Layers[7].animation.state = State::Idle; // 设置工程师的闲置
        Layers[8].animation.state = State::Idle; // 设置工程师的闲置
    }
    else{
        Layers[2].animation.state = State::Shoot; // 设置工程师的闲置
        Layers[3].animation.state = State::Shoot; // 设置工程师的闲置
        Layers[4].animation.state = State::Shoot; // 设置工程师的闲置
        Layers[5].animation.state = State::Shoot; // 设置工程师的闲置
        Layers[6].animation.state = State::Shoot; // 设置工程师的闲置
        Layers[7].animation.state = State::Shoot; // 设置工程师的闲置
        Layers[8].animation.state = State::Shoot; // 设置工程师的闲置
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
    ranged.attacks.push_back(RangedAttack(3.0, 160.0, BulletType::Bomb, 62.4, 13.0, sf::Vector2f(0,50), 1.0)); // 添加攻击
    
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

    animation.state = State::Idle; // 设置初始状态为闲置
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
    ranged.attacks.push_back(RangedAttack(3, 160.0, BulletType::Bomb, 62.4, 33.0, sf::Vector2f(0,53), 1.0)); // 添加攻击
    
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
    animation.state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
};
Engineer3::Engineer3(Position position_, int total_price_) {
    type = TowerType::Engineer3; // 设置塔类型为工程师3
    position = position_;
    total_price = total_price_;
    tower_actions = {
        {TowerAction::Sell, {TowerType::None, -total_price }},
    };
    ranged.attacks.push_back(RangedAttack(2.5, 180.0, BulletType::Bomb, 67.2, 50.0, sf::Vector2f(0,57), 1.0)); // 添加攻击
    
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
    animation.state = State::Idle; // 设置初始状态为闲置
    heading = tower_heading::Down; // 默认塔的朝向为 Down
};