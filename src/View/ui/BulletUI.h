#pragma once
#include "BaseUI.h"
#include "Model/bullets/bullets.h"
#include <SFML/Graphics/RenderWindow.hpp>

/**
 * @brief BulletUI类 - 处理Bullet实体的UI渲染和动画管理
 *
 * 重构后的设计：
 * 1. 拥有独立的AnimationContext，避免状态冲突
 * 2. 通过context管理动画状态，实现与其他UI的完全隔离
 * 3. 管理Bullet的基础动画播放（Entity类型）
 * 4. 处理Bullet的旋转逻辑（后续实现）
 */
class BulletUI : public BaseUI
{
public:
    /**
     * @brief 构造函数
     * @param bullet Bullet实体指针
     * @param animation_player AnimationPlayer引用
     */
    explicit BulletUI(Bullet* bullet):bullet_(bullet){}

    void OnClick(Store& store, sf::RenderWindow& window) override;

    /**
     * @brief 获取关联的Bullet指针
     * @return Bullet*
     */
    Bullet* GetBullet() const { return bullet_; }

private:
    Bullet* bullet_;                        ///< 关联的Bullet实体
};