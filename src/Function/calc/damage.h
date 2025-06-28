
#pragma once
#include "Model/components/damage.h"
#include "utils/macros.h"
#include <vector>
class Unit;
class Store;
namespace calc{
    /**
     * @brief 计算伤害结算后的最终值
     *
     * @param damage_event [in]
     * @return double
     */
    double calc_damage(const DamageEvent& damage_event);

    /**
     * @brief 让单位受到一定额度的伤害
     *
     * @param target [in, out]
     * @param amount [in]
     */
    void receive_damage(Unit& target, const double amount);

    /**
     * @brief 找到离家最近的敌人
     *
     * @param store [in]
     * @param position [in]
     * @param range [in]
     * @note 找不到敌人时，返回 nullptr
     * @return Unit*
     */
    Unit* find_foremost_enemy(const Store& store, const Position& position, const double range);

    /**
     * @brief 找到最近的士兵
     *
     * @param store [in]
     * @param position [in]
     * @param range [in]
     * @note 找不到士兵时，返回 nullptr
     * @return Unit*
     */
    Unit* find_nearest_soldier(const Store& store, const Position& position, const double range);

    /**
     * @brief 找到范围里的敌人
     *
     * @param store
     * @param position
     * @param range
     * @return std::vector<Unit*>
     */
    std::vector<Unit*> find_enemies_in_range(const Store& store, const Position& position, const double range);

    /**
     * @brief 找到范围里的士兵
     *
     * @param store
     * @param position
     * @param range
     * @return std::vector<Unit*>
     */
    std::vector<Unit*> find_soldiers_in_range(const Store& store, const Position& position,
                                             const double range);
}