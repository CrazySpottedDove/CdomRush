#pragma once
#include "../utils/vector2.h"
#include <cstddef>
#include <vector>
// #include "../store/store.h"
class Store;

class Tower;
class ClickAction;

class ClickAction{
    std::size_t sprite_id;
    int price;
    virtual void OnClick(Store & store, Tower& tower) = 0;
};

class TowerUpgradeAction: public ClickAction{
    void OnClick(Store & store, Tower& tower) override {
    }
};

class PowerUpgradeAction: public ClickAction{
    void OnClick(Store & store, Tower& tower) override {
    }
};

class TowerSellAction: public ClickAction{
    void OnClick(Store & store, Tower& tower) override {
    }
};

class ChangeRallyAction: public ClickAction{
    void OnClick(Store & store, Tower& tower) override {
    }
};

class Tower{
    Vector2 position;
    std::size_t sprite_id;
    int total_price;
    TowerUpgradeAction upgrade_action;
    TowerSellAction sell_action;
};