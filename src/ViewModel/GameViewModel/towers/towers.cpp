#include "ViewModel/GameViewModel/towers/towers.h"
#include "Common/action.h"
#include "Common/macros.h"
#include "Common/type.h"
#include "ViewModel/GameViewModel/components/ranged.h"
#include "ViewModel/GameViewModel/enemies/enemies.h"
#include "ViewModel/GameViewModel/store/store.h"
#include <ostream>


void Tower::Update(Store& store)
{
    pending_update();
    if (animations[0].current_state == State::Idle) {
        for (auto& attack : ranged.attacks) {
            if (attack.IsReady(store)) {
                ID     target_enemy     = calc::find_foremost_enemy(store, position, attack.range);
                Enemy* target_enemy_ptr = store.GetEnemy(target_enemy);

                if (target_enemy == INVALID_ID) continue;   // 如果没有找到目标敌人，跳过
                if (target_enemy_ptr == nullptr) continue;

                animations[0].current_state = State::Shoot;   // 设置状态为射击
                if (target_enemy_ptr->position.y > position.y)
                    heading = tower_heading::Up;   // 如果目标敌人在塔的上方，设置塔的朝向为 Up
                else
                    heading = tower_heading::Down;   // 如果目标敌人在塔的下方，设置塔的朝向为 Down
                attack.Apply(store,
                             this->id,
                             target_enemy,
                             return_offset(),
                             SourceType::Tower);   // 应用攻击
                layer_update(1);
                return;
            }
        }
    }
    else if (animations[0].current_state == State::Shoot) {
        if (animations[0].pending == false) {
            animations[0].current_state = State::Idle;   // 如果动画未进行，设置状态为闲置
        }
    }
    layer_update(0);
    return;
}

void None::pending_update() {}

None::None(Position position_)
{
    animations.push_back(Animation(State::Idle, "build_terrain_0001", Position{0.0f, 12.0f}, true));
    animations.push_back(Animation(State::Idle, "build_terrain_0001", Position{0.0f, 12.0f}));
    // animations[1].anchor_x = 0.5;
    // animations[1].anchor_y = 1;
    type        = TowerType::None;
    position    = position_;
    total_price = 0;

    // animations[0].actions.push_back(
    //     Action(ActionType::UpgradeTower, UpgradeTowerParams{id, TowerType::Archer1, 70}));
    // animations[0].actions.push_back(
    //     Action(ActionType::UpgradeTower, UpgradeTowerParams{id, TowerType::Engineer1, 125}));
    // animations[0].actions.push_back(
    //     Action(ActionType::UpgradeTower, UpgradeTowerParams{id, TowerType::Mage1, 100}));
}

bool None::Insert(Store& store)
{
    animations[0].actions.emplace_back(Action(ActionType::CreateActionFx,
                                              CreateActionFxParams{
                                                  FxType::UpgradeToArcherButton,
                                                  position,
                                                  id,
                                                  Position{20, 20},
                                              }));
    return true;
}

void Archer::pending_update()
{
    animations[0].pending =
        animations[3].pending || animations[4].pending;   // 更新动画的 pending 状态
}
void Archer::layer_update(bool flag)
{
    if (animations[0].current_state == State::Idle && heading == tower_heading::Down) {
        animations[3].current_state = State::IdleDown;   // 设置朝下的动画状态
        animations[4].current_state = State::IdleDown;   // 设置朝下的动画
    }
    else if (animations[0].current_state == State::Idle && heading == tower_heading::Up) {
        animations[3].current_state = State::IdleUp;   // 设置朝上的动画状态
        animations[4].current_state = State::IdleUp;   // 设置朝上的动画
    }
    else if (flag && heading == tower_heading::Down) {
        if (shooter) {
            animations[3].current_state = State::ShootingDown;   // 设置射击朝下的动画状态
            animations[4].current_state = State::IdleDown;       // 设置射击朝下的动画
        }
        else {
            animations[3].current_state = State::IdleDown;       // 设置射击朝下的动画状态
            animations[4].current_state = State::ShootingDown;   // 设置射击朝下的动画
        }
        shooter = !shooter;   // 切换射手状态
    }
    else if (flag && heading == tower_heading::Up) {
        if (shooter) {
            animations[3].current_state = State::ShootingUp;   // 设置射击朝上的动画状态
            animations[4].current_state = State::IdleUp;       // 设置射击朝上的动画
        }
        else {
            animations[3].current_state = State::IdleUp;       // 设置射击朝上的动画状态
            animations[4].current_state = State::ShootingUp;   // 设置射击朝上的动画
        }
        shooter = !shooter;   // 切换射手状态
    }
}
Archer1::Archer1(Position position_, int total_price_)
{
    type        = TowerType::Archer1;   // 设置塔类型为弓箭手1
    position    = position_;
    total_price = total_price_;
    animations.push_back(Animation(State::Idle, "terrain_archer1", Position{0.0f, 12.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_archer1", Position{0.0f, 12.0f}));
    animations[1].anchor_y = 0.5;
    animations.push_back(Animation(State::Idle, "archer_tower1", Position{0.0f, 37.0f}));
    animations[2].anchor_y = 0.5;
    animations.push_back(
        Animation(State::IdleDown, "tower_archer_lvl1_shooter", Position{-9.0f, 51.0f}));
    animations.push_back(
        Animation(State::IdleDown, "tower_archer_lvl1_shooter", Position{9.0f, 51.0f}));
    animations[0].actions.push_back(
        Action(ActionType::UpgradeTower, UpgradeTowerParams{id, TowerType::Archer2, 110}));
    animations[0].actions.push_back(
        Action(ActionType::SellTower, UpgradeTowerParams{id, TowerType::None, -total_price}));
    ranged.attacks.push_back(
        RangedAttack(0.8, 140.0, BulletType::Arrow, 0.0, 5.5, "arrow", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
}
Archer2::Archer2(Position position_, int total_price_)
{
    type        = TowerType::Archer2;   // 设置塔类型为弓箭手2
    position    = position_;
    total_price = total_price_;
    animations.push_back(Animation(State::Idle, "terrain_archer2", Position{0.0f, 12.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_archer2", Position{0.0f, 12.0f}));
    animations.push_back(Animation(State::Idle, "archer_tower2", Position{0.0f, 37.0f}));
    animations.push_back(
        Animation(State::IdleDown, "tower_archer_lvl2_shooter", Position{-9.0f, 52.0f}));
    animations.push_back(
        Animation(State::IdleDown, "tower_archer_lvl2_shooter", Position{9.0f, 52.0f}));
    animations[0].actions.push_back(
        Action(ActionType::UpgradeTower, UpgradeTowerParams{id, TowerType::Archer3, 160}));
    animations[0].actions.push_back(
        Action(ActionType::SellTower, UpgradeTowerParams{id, TowerType::None, -total_price}));
    ranged.attacks.push_back(
        RangedAttack(0.6, 160.0, BulletType::Arrow, 0.0, 10.0, "arrow", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
}
Archer3::Archer3(Position position_, int total_price_)
{
    type        = TowerType::Archer3;   // 设置塔类型为弓箭手3
    position    = position_;
    total_price = total_price_;
    animations.push_back(Animation(State::Idle, "terrain_archer3", Position{0.0f, 12.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_archer3", Position{0.0f, 12.0f}));
    animations.push_back(Animation(State::Idle, "archer_tower3", Position{0.0f, 37.0f}));
    animations.push_back(
        Animation(State::IdleDown, "tower_archer_lvl3_shooter", Position{-9.0f, 52.0f}));
    animations.push_back(
        Animation(State::IdleDown, "tower_archer_lvl3_shooter", Position{9.0f, 52.0f}));
    animations[0].actions.push_back(
        Action(ActionType::SellTower, UpgradeTowerParams{id, TowerType::None, -total_price}));
    ranged.attacks.push_back(
        RangedAttack(0.5, 180.0, BulletType::Arrow, 0.0, 15.0, "arrow", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
}

void Engineer::pending_update()
{
    animations[0].pending = animations[2].pending;
}
void Engineer::layer_update(bool flag)
{
    if (animations[0].current_state == State::Idle) {
        animations[2].current_state = State::Idle;   // 设置工程师的闲置
        animations[3].current_state = State::Idle;   // 设置工程师的闲置
        animations[4].current_state = State::Idle;   // 设置工程师的闲置
        animations[5].current_state = State::Idle;   // 设置工程师的闲置
        animations[6].current_state = State::Idle;   // 设置工程师的闲置
        animations[7].current_state = State::Idle;   // 设置工程师的闲置
        animations[8].current_state = State::Idle;   // 设置工程师的闲置
    }
    else if (flag) {
        animations[2].current_state = State::Shoot;   // 设置工程师的闲置
        animations[3].current_state = State::Shoot;   // 设置工程师的闲置
        animations[4].current_state = State::Shoot;   // 设置工程师的闲置
        animations[5].current_state = State::Shoot;   // 设置工程师的闲置
        animations[6].current_state = State::Shoot;   // 设置工程师的闲置
        animations[7].current_state = State::Shoot;   // 设置工程师的闲置
        animations[8].current_state = State::Shoot;   // 设置工程师的闲置
    }
    return;
}
Engineer1::Engineer1(Position position_, int total_price_)
{
    type        = TowerType::Engineer1;   // 设置塔类型为工程师1
    position    = position_;
    total_price = total_price_;
    animations.push_back(
        Animation(State::Idle, "terrain_artillery_0001", Position{0.0f, 15.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_artillery_0001", Position{0.0f, 15.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl1_layer1", Position{0.0f, 41.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl1_layer2", Position{0.0f, 41.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl1_layer3", Position{0.0f, 41.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl1_layer4", Position{0.0f, 41.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl1_layer5", Position{0.0f, 41.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl1_layer6", Position{0.0f, 41.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl1_layer7", Position{0.0f, 41.0f}));
    animations[0].actions.push_back(
        Action(ActionType::UpgradeTower, UpgradeTowerParams{id, TowerType::Engineer2, 220}));
    animations[0].actions.push_back(
        Action(ActionType::SellTower, UpgradeTowerParams{id, TowerType::None, -total_price}));
    ranged.attacks.push_back(
        RangedAttack(3.0, 160.0, BulletType::Bomb, 62.4, 13.0, "bombs_0001", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
}
Engineer2::Engineer2(Position position_, int total_price_)
{
    type        = TowerType::Engineer2;   // 设置塔类型为工程师2
    position    = position_;
    total_price = total_price_;
    animations.push_back(
        Animation(State::Idle, "terrain_artillery_0002", Position{0.0f, 15.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_artillery_0002", Position{0.0f, 15.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl2_layer1", Position{0.0f, 42.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl2_layer2", Position{0.0f, 42.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl2_layer3", Position{0.0f, 42.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl2_layer4", Position{0.0f, 42.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl2_layer5", Position{0.0f, 42.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl2_layer6", Position{0.0f, 42.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl2_layer7", Position{0.0f, 42.0f}));
    animations[0].actions.push_back(
        Action(ActionType::UpgradeTower, UpgradeTowerParams{id, TowerType::Engineer3, 320}));
    animations[0].actions.push_back(
        Action(ActionType::SellTower, UpgradeTowerParams{id, TowerType::None, -total_price}));
    ranged.attacks.push_back(
        RangedAttack(3, 160.0, BulletType::Bomb, 62.4, 33.0, "bombs_0002", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
};
Engineer3::Engineer3(Position position_, int total_price_)
{
    type        = TowerType::Engineer3;   // 设置塔类型为工程师3
    position    = position_;
    total_price = total_price_;
    animations.push_back(
        Animation(State::Idle, "terrain_artillery_0003", Position{0.0f, 15.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_artillery_0003", Position{0.0f, 15.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl3_layer1", Position{0.0f, 43.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl3_layer2", Position{0.0f, 43.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl3_layer3", Position{0.0f, 43.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl3_layer4", Position{0.0f, 43.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl3_layer5", Position{0.0f, 43.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl3_layer6", Position{0.0f, 43.0f}));
    animations.push_back(
        Animation(State::Idle, "tower_artillery_lvl3_layer7", Position{0.0f, 43.0f}));
    animations[0].actions.push_back(
        Action(ActionType::SellTower, UpgradeTowerParams{id, TowerType::None, -total_price}));
    ranged.attacks.push_back(
        RangedAttack(2.5, 180.0, BulletType::Bomb, 67.2, 50.0, "bombs_0003", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
};

void Mage::pending_update()
{
    animations[0].pending = animations[3].pending;
}
void Mage::layer_update(bool flag)
{
    if (animations[0].current_state == State::Idle && heading == tower_heading::Down) {
        animations[2].current_state = State::Idle;       // 设置朝下的动画状态
        animations[3].current_state = State::IdleDown;   // 设置朝下的动画
    }
    else if (animations[0].current_state == State::Idle && heading == tower_heading::Up) {
        animations[2].current_state = State::Idle;     // 设置朝上的动画状态
        animations[3].current_state = State::IdleUp;   // 设置朝上的动画
    }
    else if (flag && heading == tower_heading::Down) {
        animations[2].current_state = State::Shoot;          // 设置射击朝下的动画状态
        animations[3].current_state = State::ShootingDown;   // 设置射击朝下的动画
    }
    else if (flag && heading == tower_heading::Up) {
        animations[2].current_state = State::Shoot;        // 设置射击朝上的动画状态
        animations[3].current_state = State::ShootingUp;   // 设置射击朝上的动画
    }
}
Mage1::Mage1(Position position_, int total_price_)
{
    type        = TowerType::Mage1;   // 设置塔类型为法师1
    position    = position_;
    total_price = total_price_;
    animations.push_back(
        Animation(State::Idle, "terrain_artillery_0001", Position{0.0f, 15.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_artillery_0001", Position{0.0f, 15.0f}));
    animations.push_back(Animation(State::Idle, "mage_lvl1", Position{0.0f, 30.0f}));
    animations.push_back(Animation(State::Idle, "mage_shooter", Position{0.0f, 57.0f}));
    // animations[0].actions.push_back(Action(ActionType))
    // animations[0].actions.push_back(Action(ActionType::UpgradeTower,UpgradeTowerParams{id,TowerType::Mage2,
    // 160}));
    // animations[0].actions.push_back(Action(ActionType::SellTower,UpgradeTowerParams{id,TowerType::None,
    // -total_price}));
    ranged.attacks.push_back(
        RangedAttack(1.5, 140.0, BulletType::Bolt, 0.0, 15, "magebolt", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
}
Mage2::Mage2(Position position_, int total_price_)
{
    type        = TowerType::Mage2;   // 设置塔类型为法师2
    position    = position_;
    total_price = total_price_;
    animations.push_back(
        Animation(State::Idle, "terrain_artillery_0002", Position{0.0f, 15.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_artillery_0002", Position{0.0f, 15.0f}));
    animations.push_back(Animation(State::Idle, "mage_lvl2", Position{0.0f, 30.0f}));
    animations.push_back(Animation(State::Idle, "mage_shooter", Position{0.0f, 57.0f}));
    animations[0].actions.push_back(
        Action(ActionType::UpgradeTower, UpgradeTowerParams{id, TowerType::Mage3, 240}));
    animations[0].actions.push_back(
        Action(ActionType::SellTower, UpgradeTowerParams{id, TowerType::None, -total_price}));
    ranged.attacks.push_back(
        RangedAttack(1.5, 160.0, BulletType::Bolt, 0.0, 36, "magebolt", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
}
Mage3::Mage3(Position position_, int total_price_)
{
    type        = TowerType::Mage3;   // 设置塔类型为法师3
    position    = position_;
    total_price = total_price_;
    animations.push_back(
        Animation(State::Idle, "terrain_artillery_0003", Position{0.0f, 15.0f}, true));
    animations.push_back(Animation(State::Idle, "terrain_artillery_0003", Position{0.0f, 15.0f}));
    animations.push_back(Animation(State::Idle, "mage_lvl3", Position{0.0f, 30.0f}));
    animations.push_back(Animation(State::Idle, "mage_shooter", Position{0.0f, 62.0f}));
    animations[0].actions.push_back(
        Action(ActionType::SellTower, UpgradeTowerParams{id, TowerType::None, -total_price}));
    ranged.attacks.push_back(
        RangedAttack(1.5, 180.0, BulletType::Bolt, 0.0, 63, "magebolt", 1.0));   // 添加攻击

    heading = tower_heading::Down;   // 默认塔的朝向为 Down
}

void Barracks::pending_update()
{
    animations[0].pending = animations[2].pending;
}
void Barracks::layer_update(bool flag)
{
    if (flag)
        animations[2].current_state = State::DoorOpen;
    else if (animations[0].current_state == State::DoorClose)
        animations[2].current_state = State::DoorClose;
    return ;
}
void Barracks::Update(Store& store)
{
    pending_update();
    for(auto it = soldiers.begin();it!=soldiers.end();){
        if(store.GetSoldier(*it)==nullptr){
            soldiers.erase(it);
        }
        else{
            it++;
        }
    }
    bool solider_size_changed = false;
    while (soldiers.size() < 3) {
        animations[0].current_state = State::DoorOpen;
        Soldier* soldierx           = store.template_manager.CreateSoldier(return_soldier_type());
        soldierx->position = position + return_offset();
        store.QueueSoldier(soldierx);
        soldiers.push_back(soldierx->id);
        solider_size_changed = true;
    }
    if (solider_size_changed) {
        for (int i = 0; i < 3; i++) {

            Soldier* si     = store.GetSoldier(soldiers[i]);
            si->rally_point = rally_point;
            if (i == 0) si->rally_point_offset = sf::Vector2f(-17.3, -10);
            if (i == 1) si->rally_point_offset = sf::Vector2f(17.3, -10);
            if (i == 2) si->rally_point_offset = sf::Vector2f(0, 20);
        }
        animations[0].current_state = State::DoorOpen;
        layer_update(1);
    }
    if (animations[0].current_state == State::DoorOpen) {
        if (!animations[2].pending) {
            animations[0].current_state = State::DoorClose;
        }
    }
    layer_update(0);
}

Barracks1::Barracks1(Position position_, int total_price_)
{
    type        = TowerType::Barrack1;
    position    = position_;
    total_price = total_price_;
    heading = tower_heading::Down;
}
Barracks2::Barracks2(Position position_, int total_price_) {
    type        = TowerType::Barrack2;
    position    = position_;
    total_price = total_price_;
    heading = tower_heading::Down;
}
Barracks3::Barracks3(Position position_, int total_price_) {
    type        = TowerType::Barrack3;
    position    = position_;
    total_price = total_price_;
    heading = tower_heading::Down;
}