#pragma once
#include "Model/towers/towers.h"
#include "utils/macros.h"
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <variant>
class Store;

enum class EventType
{
    SelectLevel,
    UpgradeTower
};

struct EventData
{
    std::variant<std::string, ID> prop1;
    std::variant<TowerType, ID, Position> prop2;
    std::variant<int, ID> prop3;
};

class Event
{
public:
    EventType type;
    EventData data;
};

namespace callback {
    /**
     * @brief 选择关卡
     * @note 仅仅设置当前关卡名，并进入加载状态
     * @param store
     * @param level_name [in] 关卡名称
     */
    void SelectLevel(Store& store,const std::string& level_name);

    /**
     * @brief 升级塔
     *
     * @param store
     * @param tower_id [in] 原塔的 id
     * @param tower_type [in] 新塔的类型
     * @param cost [in] 升级所需的费用
     */
    void UpgradeTower(Store& store, const ID tower_id, const TowerType tower_type, const int cost);

    /**
     * @brief 改变塔的集结点
     *
     * @param store
     * @param tower_id
     * @param position
     */
    void ChangeRallyPoint(Store& store, const ID tower_id, const Position& position);
}