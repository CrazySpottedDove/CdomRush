#pragma once
#include "BaseUI.h"
#include "View/animation/animationPlayer.h"
#include "Model/towers/towers.h"

/**
 * @brief TowerUI类 - 处理Tower实体的UI渲染和动画管理
 * 
 * 重构后的设计：
 * 1. 拥有独立的AnimationContext，避免状态冲突
 * 2. 通过context管理动画状态，实现与其他UI的完全隔离
 * 3. 管理Tower的基础动画播放（Entity类型）
 * 4. 处理Tower的攻击动画和空闲动画切换
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
    AnimationContext animation_context_;    ///< 独立的动画上下文
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