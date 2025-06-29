
#pragma once
#include "Model/components/buff.h"
#include "Model/components/damage.h"
#include "utils/macros.h"
#include <vector>
class Unit;
class Store;
class Enemy;
class Soldier;
namespace calc{
    /**
     * @brief 通过 buff 域更新伤害数值
     * @note 仅在 enforce_damage 中使用
     *
     * @param buff
     * @param value
     */
    void update_damage_value(const Buff& buff, double& value);

    /**
     * @brief 结算一个伤害事件
     *
     * @param damage_event [in]
     */
    void enforce_damage(const DamageEvent& damage_event);

    /**
     * @brief 找到离家最近的敌人
     *
     * @param store [in]
     * @param position [in]
     * @param range [in]
     * @note 找不到敌人时，返回 nullptr
     * @return Unit*
     */
    Enemy* find_foremost_enemy(const Store& store, const Position& position, const double range, const bool require_active = false);

    /**
     * @brief 找到最近的士兵
     *
     * @param store [in]
     * @param position [in]
     * @param range [in]
     * @note 找不到士兵时，返回 nullptr
     * @return Unit*
     */
    Soldier* find_nearest_soldier(const Store& store, const Position& position, const double range);

    /**
     * @brief 找到范围里的敌人
     *
     * @param store
     * @param position
     * @param range
     * @return std::vector<Unit*>
     */
    std::vector<Enemy*> find_enemies_in_range(const Store& store, const Position& position, const double range);

    /**
     * @brief 找到范围里的士兵
     *
     * @param store
     * @param position
     * @param range
     * @return std::vector<Unit*>
     */
    std::vector<Soldier*> find_soldiers_in_range(const Store& store, const Position& position,
                                             const double range);
}