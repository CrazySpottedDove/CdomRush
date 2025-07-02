#pragma once
#include "BaseUI.h"
#include "View/animation/animationPlayer.h"
#include "Model/soldiers/soldiers.h"
#include <SFML/Graphics/RenderWindow.hpp>

/**
 * @brief SoldierUI类 - 处理Soldier实体的UI渲染和状态切换，还有flip
 *
 * 重构后的设计：
 * 1. 拥有独立的AnimationContext，避免状态冲突
 * 2. 通过context管理动画状态，实现与其他UI的完全隔离
 */
class SoldierUI : public BaseUI
{
public:
    /**
     * @brief 构造函数
     * @param soldier Soldier实体指针
     * @param animation_player AnimationPlayer引用
     */
    explicit SoldierUI(Soldier* soldier): soldier_(soldier){}


    void OnClick(Store& store, sf::RenderWindow& window) override;

    /**
     * @brief 获取关联的Soldier指针
     * @return Soldier*
     */
    Soldier* GetSoldier() const { return soldier_; }

private:
    Soldier* soldier_;                       ///< 关联的Soldier实体
};