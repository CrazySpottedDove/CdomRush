#pragma once
#include "BaseUI.h"
#include "View/animation/animationPlayer.h"
#include "Model/enemies/enemies.h"

/**
 * @brief EnemyUI类 - 处理Enemy实体的UI渲染和状态切换，还有flip
 */
class EnemyUI : public BaseUI
{
public:
    /**
     * @brief 构造函数
     * @param Enemy指针
     * @param AnimationPlayer引用
     */
    explicit EnemyUI(Enemy* enemy, AnimationPlayer& animation_player);
    
    /**
     * @brief 渲染Enemy
     * @param window渲染窗口
     * @param scale全局缩放比例
     */
    void Render(sf::RenderWindow& window, const sf::Vector2f& scale = {1.0f, 1.0f}) override;
    
    /**
     * @brief 获取关联的Enemy指针
     * @return Enemy* 
     */
    Enemy* GetEnemy() const { return enemy_; }

private:
    Enemy* enemy_;                          /// 关联的Enemy实体
    AnimationPlayer& animation_player_;     /// AnimationPlayer引用
    State last_state_;                      /// 上次的状态，用于检测状态变化
    Heading last_heading_;                  /// 上次的朝向，用于检测朝向变化
    bool initialized_;                      /// 是否已初始化动画
    
    /**
     * @brief 更新动画状态
     * 检测Enemy状态变化并自动切换动画
     */
    void UpdateAnimationState();
    
    /**
     * @brief 初始化Enemy动画
     */
    void InitializeAnimation();
    
    /**
     * @brief 检查是否需要更新动画
     * @return true，如果状态或朝向发生变化
     */
    bool NeedsAnimationUpdate() const;
}; 