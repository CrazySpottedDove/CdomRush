#include "Function/events/events.h"
#include "Manager/store/store.h"
#include "Model/towers/towers.h"

// 任何方法都不应当直接修改 Store 的状态。 Store 的状态只在它自身的成员函数中被修改。
namespace callback {
    void SelectLevel(Store& store, const std::string& level_name) {
        store.level_manager.SetCurrentLevelName(level_name);
    }
    
    void UpgradeTower(Store& store, const ID tower_id, const TowerType tower_type, const int cost){
        store.gold -= cost;
        Tower* old_tower = store.GetTower(tower_id);
        Tower* new_tower = store.template_manager.CreateTower(tower_type);

        new_tower->total_price = old_tower->total_price + cost;
        new_tower->position = old_tower->position;

        store.QueueTower(new_tower);
        store.DequeueTower(tower_id);
    }
}