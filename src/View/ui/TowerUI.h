#pragma once
#include "BaseUI.h"
#include "Model/towers/towers.h"
#include "View/animation/animationPlayer.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

/**
 * @brief TowerUI类 - 处理Tower实体的多图层UI渲染和动画管理
 *
 * 1. 支持Tower的多图层渲染系统
 * 2. 每个图层拥有独立的AnimationContext，避免状态冲突
 * 3. 遍历tower_->Layers，为每层创建独立渲染调用
 * 4. 使用layer.offset计算正确的渲染位置
 * 5. 使用layer.animation的前缀和状态进行精确控制
 */
class TowerUI : public BaseUI
{
public:
    /**
     * @brief 构造函数
     * @param tower Tower实体指针
     * @param animation_player AnimationPlayer引用
     */
    explicit TowerUI(Tower* tower)
        : tower_(tower)
    {}

    /**
     * @brief 渲染Tower的所有图层
     * @param window 渲染窗口
     * @param scale 全局缩放比例
     */
    void Render(AnimationPlayer* animation_player, sf::RenderWindow& window,
                const Position& position, Animation& animation,
                const sf::Vector2f& scale = {1.0f, 1.0f}) override;

    /**
     * @brief 获取关联的Tower指针
     * @return Tower*
     */
    Tower* GetTower() const { return tower_; }

    void OnClick(Store& store, sf::RenderWindow& window) override;

private:
    Tower* tower_;   ///< 关联的Tower实体
};