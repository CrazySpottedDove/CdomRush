#pragma once
#include "BaseUI.h"
#include "Model/enemies/enemies.h"
#include <SFML/Graphics/RenderWindow.hpp>

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
    explicit EnemyUI(Enemy* enemy)
        : enemy_(enemy)
    {}

    /**
     * @brief 获取关联的Enemy指针
     * @return Enemy*
     */
    Enemy* GetEnemy() const { return enemy_; }

    void OnClick(Store& store, sf::RenderWindow& window) override;
private:
    Enemy* enemy_;   ///< 关联的Enemy实体
};