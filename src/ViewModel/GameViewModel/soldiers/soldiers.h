#pragma once
#include "ViewModel/GameViewModel/Function/calc/damage.h"
#include "ViewModel/GameViewModel/Function/calc/hp.h"
#include "ViewModel/GameViewModel/Function/calc/motion.h"
#include "ViewModel/GameViewModel/components/melee.h"
#include "ViewModel/GameViewModel/components/path.h"
#include "ViewModel/GameViewModel/components/ranged.h"
#include "ViewModel/GameViewModel/templates/unit.h"
#include "Common/type.h"
#include "Common/macros.h"

class Soldier: public Unit{
public:
    sf::Vector2f slot; // 用于近战偏移
    Position last_rally_point;
    Position rally_point;
    Position rally_point_offset;
    double range;
    ID source_barrack;
    ID target_enemy = INVALID_ID;
    Melee            melee;
    virtual Soldier* Clone() = 0;
};

class SoldierMelee: public Soldier{
public:
    void Update(Store& store) override;
};

class SoldierMeleelv1 : public SoldierMelee{
public:
    SoldierMeleelv1(Position position_ = sf::Vector2f(0,0),Position rally_point = sf::Vector2f(0,0),Position offset_ = sf::Vector2f(0,0));
    void death_action(Store& store) override {}
    Soldier* Clone() override {
        return new SoldierMeleelv1(*this);
    }
};
class SoldierMeleelv2 : public SoldierMelee{
public:
    SoldierMeleelv2(Position position_ = sf::Vector2f(0,0),Position rally_point = sf::Vector2f(0,0),Position offset_ = sf::Vector2f(0,0));
    void death_action(Store& store) override {}
    Soldier* Clone() override {
        return new SoldierMeleelv2(*this);
    }
};
class SoldierMeleelv3 : public SoldierMelee{
public:
    SoldierMeleelv3(Position position_ = sf::Vector2f(0,0),Position rally_point_ = sf::Vector2f(0,0),Position offset_ = sf::Vector2f(0,0));
    void death_action(Store& store) override {}
    Soldier* Clone() override {
        return new SoldierMeleelv3(*this);
    }
};