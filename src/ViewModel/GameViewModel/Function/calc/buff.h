#pragma once
#include "Model/components/buff.h"
class Unit;
namespace calc {
    /**
     * @brief 目标获得伤害增益
     *
     * @param target
     * @param amount
     */
    void damage_inc(Unit& target, const double amount);

    /**
     * @brief 目标减少伤害增益
     *
     * @param target
     * @param amount
     */
    void damage_dec(Unit& target, const double amount);

    /**
     * @brief 目标伤害增益乘法
     *
     * @param target
     * @param factor
     */
    void damage_mul(Unit& target, const double factor);

    /**
     * @brief 目标伤害增益除法
     *
     * @param target
     * @param factor (请保证不为 0)
     */
    void damage_div(Unit& target, const double factor);

    /**
     * @brief 目标获得移速增益
     *
     * @param target
     * @param amount
     */
    void speed_inc(Unit& target, const double amount);

    /**
     * @brief 目标减少移速增益
     *
     * @param target
     * @param amount
     */
    void speed_dec(Unit& target, const double amount);

    /**
     * @brief 目标移速增益乘法
     *
     * @param target
     * @param factor
     */
    void speed_mul(Unit& target, const double factor);

    /**
     * @brief 目标移速增益除法
     *
     * @param target
     * @param factor
     */
    void speed_div(Unit& target, const double factor);

    /**
     * @brief 目标获得物理护甲增益
     *
     * @param target
     * @param amount
     */
    void physical_armor_inc(Unit& target, const double amount);

    /**
     * @brief 目标减少物理护甲增益
     *
     * @param target
     * @param amount
     */
    void physical_armor_dec(Unit& target, const double amount);

    /**
     * @brief 目标法术护甲增益乘法
     *
     * @param target
     * @param factor
     */
    void magical_armor_inc(Unit& target, const double amount);

    /**
     * @brief 目标法术护甲增益除法
     *
     * @param target
     * @param factor
     */
    void magical_armor_dec(Unit& target, const double amount);
}