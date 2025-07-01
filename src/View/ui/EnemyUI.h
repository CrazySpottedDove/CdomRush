#pragma once
#include "BaseUI.h"
#include "View/animation/animationPlayer.h"
#include "Model/enemies/enemies.h"

/**
 * @brief EnemyUI类 - 处理Enemy实体的UI渲染和状态切换，还有flip
 * 
 * 1. 拥有独立的AnimationContext，避免状态冲突
 * 2. 通过context管理动画状态，与其他UI完全隔离
 */
class EnemyUI : public BaseUI
{
public:
    /**
     * @brief 构造函数
     * @param enemy Enemy实体指针
     * @param animation_player AnimationPlayer引用
     */
    explicit EnemyUI(Enemy* enemy, AnimationPlayer& animation_player);
    
    /**
     * @brief 渲染Enemy
     * @param window 渲染窗口
     * @param scale 全局缩放比例
     */
    void Render(sf::RenderWindow& window,
        const Position& position,
        Animation& animation,
        const sf::Vector2f& scale = {1.0f, 1.0f}) override;
    //void Render(sf::RenderWindow& window, const sf::Vector2f& scale = {1.0f, 1.0f}) override;
    
    /**
     * @brief 获取关联的Enemy指针
     * @return Enemy* 
     */
    Enemy* GetEnemy() const { return enemy_; }

private:
    Enemy* enemy_;                          ///< 关联的Enemy实体
    AnimationPlayer& animation_player_;     ///< AnimationPlayer引用
    AnimationContext animation_context_;    ///< 独立的动画上下文
    //State last_state_;                      ///< 上次的状态，用于检测状态变化
    //Heading last_heading_;                  ///< 上次的朝向，用于检测朝向变化
    //bool initialized_;                      ///< 是否已初始化动画
    
    /**
     * @brief 更新动画状态（内部使用）
     * 检测Enemy状态变化并自动切换动画
     */
    //void UpdateAnimationState();
    
    /**
     * @brief 初始化Enemy动画（内部使用）
     */
    //void InitializeAnimation();
    
    /**
     * @brief 检查是否需要更新动画（内部使用）
     * @return true 如果状态或朝向发生变化
     */
    //bool NeedsAnimationUpdate() const;
}; 