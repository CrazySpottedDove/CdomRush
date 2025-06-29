#pragma once
#include "BaseUI.h"
#include "View/animation/animationPlayer.h"
#include "Model/towers/towers.h"

/**
 * @brief TowerUI类 - 处理Tower实体的UI渲染和动画管理
 * 
 * 功能：
 * 1. 管理Tower的基础动画播放
 * 2. 提供Tower的渲染功能
 * 3. 自动检测Tower状态变化并更新动画
 * 4. 处理Tower的UI操作（升级、出售等，后续扩展）
 */
class TowerUI : public BaseUI
{
public:
    /**
     * @brief 构造函数
     * @param tower Tower实体指针
     * @param animation_player AnimationPlayer引用
     */
    explicit TowerUI(Tower* tower, AnimationPlayer& animation_player);
    
    /**
     * @brief 渲染Tower
     * @param window 渲染窗口
     * @param scale 全局缩放比例
     */
    void Render(sf::RenderWindow& window, const sf::Vector2f& scale = {1.0f, 1.0f}) override;
    
    /**
     * @brief 获取关联的Tower指针
     * @return Tower* 
     */
    Tower* GetTower() const { return tower_; }

private:
    Tower* tower_;                          ///< 关联的Tower实体
    AnimationPlayer& animation_player_;     ///< AnimationPlayer引用
    State last_state_;                      ///< 上次的状态，用于检测状态变化
    bool initialized_;                      ///< 是否已初始化动画
    
    /**
     * @brief 更新动画状态（内部使用）
     * 检测Tower状态变化并自动切换动画
     */
    void UpdateAnimationState();
    
    /**
     * @brief 初始化Tower动画（内部使用）
     */
    void InitializeAnimation();
    
    /**
     * @brief 检查是否需要更新动画（内部使用）
     * @return true 如果状态发生变化
     */
    bool NeedsAnimationUpdate() const;
}; 