#pragma once
#include "Model/components/health.h"
class Unit;
namespace calc {
    /**
     * @brief 目标恢复指定的生命值
     *
     * @param target [in, out]
     * @param amount [in]
     */
    void heal(Unit& target, const double amount);

    /**
     * @brief 目标再生(根据目标的 health 组件)
     *
     * @param target [in, out]
     */
    void regen(Unit& target);

    /**
     * @brief 判断目标是否死亡
     *
     * @param target [in]
     * @return true 死亡
     * @return false 未死亡
     */
    bool is_dead(const Unit& target);
}