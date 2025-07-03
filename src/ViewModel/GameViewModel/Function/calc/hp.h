#pragma once
#include "ViewModel/GameViewModel/components/health.h"
class Unit;
class Store;
namespace calc {
/**
 * @brief 目标恢复指定的生命值
 *
 * @param target [in, out]
 * @param amount [in]
 */
void heal(Unit& target, const double amount);

/**
 * @brief 用本方法确定士兵能否自然恢复生命值
 *
 * @param store
 * @param self
 * @return true
 * @return false
 */
bool ready_to_regen(const Store& store, const Unit& self);

/**
 * @brief 目标再生(根据目标的 health 组件)
 * @note 在士兵没在拦截敌人，且状态为 Idle 时，才考虑调用本方法
 * @note 在士兵的 Update 中，当士兵刚刚从拦截变为 Idle 时，将 health 组件的 last_regen_time 设置为 store.time
 *
 * @param target [in, out]
 */
void regen(const Store& store, Unit& self);

/**
 * @brief 判断目标是否死亡
 *
 * @param target [in]
 * @return true 死亡
 * @return false 未死亡
 */
bool is_dead(const Unit& self);

/**
 * @brief 判断目标是否应该被移除
 * @note 调用该方法前，请确保 is_dead 为 true
 * @param store [in]
 * @param target [in]
 * @return true 应该被移除
 */
bool should_remove(const Store& store, const Unit& target);
}   // namespace calc