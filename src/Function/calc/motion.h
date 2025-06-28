#pragma once
class Unit;
class Store;
namespace calc{
    /**
     * @brief 目标单位在一个游戏刻中移动
     *
     * @param store [in]
     * @param target [in, out]
     */
    void move_tick(const Store& store, Unit& target);
}