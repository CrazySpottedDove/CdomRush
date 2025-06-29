#pragma once
#include "BaseUI.h"
#include "View/animation/animationPlayer.h"
#include "Model/soldiers/soldiers.h"

/**
 * @brief SoldierUI类 - 处理Soldier实体的UI渲染和状态管理
 * 
 * 功能：
 * 1. 管理Soldier的动画状态切换
 * 2. 处理Soldier的方向翻转逻辑
 * 3. 提供Soldier的渲染功能
 * 4. 自动检测Soldier状态变化并更新动画
 */
class SoldierUI : public BaseUI
{
public:
    /**
     * @brief 构造函数
     * @param soldier Soldier实体指针
     * @param animation_player AnimationPlayer引用
     */
    explicit SoldierUI(Soldier* soldier, AnimationPlayer& animation_player);
    
    /**
     * @brief 渲染Soldier
     * @param window 渲染窗口
     * @param scale 全局缩放比例
     */
    void Render(sf::RenderWindow& window, const sf::Vector2f& scale = {1.0f, 1.0f}) override;
    
    /**
     * @brief 获取关联的Soldier指针
     * @return Soldier* 
     */
    Soldier* GetSoldier() const { return soldier_; }

private:
    Soldier* soldier_;                      ///< 关联的Soldier实体
    AnimationPlayer& animation_player_;     ///< AnimationPlayer引用
    State last_state_;                      ///< 上次的状态，用于检测状态变化
    Heading last_heading_;                  ///< 上次的朝向，用于检测朝向变化
    bool initialized_;                      ///< 是否已初始化动画
    
    /**
     * @brief 更新动画状态（内部使用）
     * 检测Soldier状态变化并自动切换动画
     */
    void UpdateAnimationState();
    
    /**
     * @brief 初始化Soldier动画（内部使用）
     */
    void InitializeAnimation();
    
    /**
     * @brief 检查是否需要更新动画（内部使用）
     * @return true 如果状态或朝向发生变化
     */
    bool NeedsAnimationUpdate() const;
}; 