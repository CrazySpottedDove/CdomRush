#pragma once
class Enemy;
class Unit;
class Store;
class Soldier;
namespace calc {
/**
 * @brief 目标单位在一个游戏刻中移动
 * @note 确保仅在单位的状态为移动时才调用本函数
 * @note 会修改目标单位的 heading, position 和行进状态
 * @note 如果移动后，单位到达了路径的终点，
 * @param store [in]
 * @param self [in, out]
 */
void enemy_move_tick(const Store& store, Enemy& self);

/**
 * @brief 目标单位在一个游戏刻中移动
 * @note 确保仅在单位的状态为移动时才调用本函数
 * @note 会修改目标单位的 heading, position 和行进状态
 *
 * @param store [in]
 * @param self [in, out]
 */
void soldier_move_tick(const Store& store, Soldier& self);

/**
 * @brief 判断敌人是否已经到达防守点
 *
 * @param store
 * @param self
 */
bool enemy_reached_defence_point(const Store& store, const Enemy& self);
}   // namespace calc