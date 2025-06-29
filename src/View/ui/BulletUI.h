#pragma once
#include "BaseUI.h"
#include "View/animation/animationPlayer.h"
#include "Model/bullets/bullets.h"

/**
 * @brief BulletUI类 - 处理Bullet实体的UI渲染和动画管理
 * 
 * 功能：
 * 1. 管理Bullet的基础动画播放
 * 2. 处理Bullet的旋转逻辑（后续实现）
 * 3. 提供Bullet的渲染功能
 * 4. 自动检测Bullet状态变化并更新动画
 */
class BulletUI : public BaseUI
{
public:
    /**
     * @brief 构造函数
     * @param bullet Bullet实体指针
     * @param animation_player AnimationPlayer引用
     */
    explicit BulletUI(Bullet* bullet, AnimationPlayer& animation_player);
    
    /**
     * @brief 渲染Bullet
     * @param window 渲染窗口
     * @param scale 全局缩放比例
     */
    void Render(sf::RenderWindow& window, const sf::Vector2f& scale = {1.0f, 1.0f}) override;
    
    /**
     * @brief 获取关联的Bullet指针
     * @return Bullet* 
     */
    Bullet* GetBullet() const { return bullet_; }

private:
    Bullet* bullet_;                        ///< 关联的Bullet实体
    AnimationPlayer& animation_player_;     ///< AnimationPlayer引用
    State last_state_;                      ///< 上次的状态，用于检测状态变化
    bool initialized_;                      ///< 是否已初始化动画
    // TODO: 旋转相关成员变量
    // double rotation_angle_;              ///< 当前旋转角度（弧度）
    // sf::Vector2f last_position_;         ///< 上次位置，用于计算旋转方向
    
    /**
     * @brief 更新动画状态（内部使用）
     * 检测Bullet状态变化并自动切换动画
     */
    void UpdateAnimationState();
    
    /**
     * @brief 初始化Bullet动画（内部使用）
     */
    void InitializeAnimation();
    
    /**
     * @brief 检查是否需要更新动画（内部使用）
     * @return true 如果状态发生变化
     */
    bool NeedsAnimationUpdate() const;
    
    // TODO: 后续实现旋转逻辑
    // /**
    //  * @brief 更新旋转角度（内部使用）
    //  * 根据Bullet的移动方向计算旋转角度
    //  */
    // void UpdateRotation();
}; 