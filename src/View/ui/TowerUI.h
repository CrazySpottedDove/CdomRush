#pragma once
#include "BaseUI.h"
#include "View/animation/animationPlayer.h"
#include "Model/towers/towers.h"
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
    explicit TowerUI(Tower* tower, AnimationPlayer& animation_player);
    
    /**
     * @brief 渲染Tower的所有图层
     * @param window 渲染窗口
     * @param scale 全局缩放比例
     */
    void Render(sf::RenderWindow& window,const Position& position, Animation& animation, const sf::Vector2f& scale = {1.0f, 1.0f}) override;
    //void Render(sf::RenderWindow& window, const sf::Vector2f& scale = {1.0f, 1.0f}) override;
    
    /**
     * @brief 获取关联的Tower指针
     * @return Tower* 
     */
    Tower* GetTower() const { return tower_; }

private:
    Tower* tower_;                                      ///< 关联的Tower实体
    AnimationPlayer& animation_player_;                 ///< AnimationPlayer引用
    std::vector<AnimationContext> layer_contexts_;     ///< 每个图层的独立动画上下文
    //std::vector<State> last_layer_states_;             ///< 每个图层上次的状态，用于检测状态变化
    //bool initialized_;                                  ///< 是否已初始化图层动画
    
    // /**
    //  * @brief 初始化图层动画系统（内部使用）
    //  * 为每个图层创建AnimationContext并设置初始状态
    //  */
    // void InitializeLayerAnimations();
    
    // /**
    //  * @brief 更新所有图层的动画状态（内部使用）
    //  * 检测每个图层的状态变化并自动切换动画
    //  */
    // void UpdateLayerAnimations();
    
    // /**
    //  * @brief 渲染单个图层（内部使用）
    //  * @param window 渲染窗口
    //  * @param layer_index 图层索引
    //  * @param scale 缩放比例
    //  */
    // void RenderLayer(sf::RenderWindow& window, size_t layer_index, const sf::Vector2f& scale);
    
    // /**
    //  * @brief 检查指定图层是否需要更新动画（内部使用）
    //  * @param layer_index 图层索引
    //  * @return true 如果该图层状态发生变化
    //  */
    // bool LayerNeedsAnimationUpdate(size_t layer_index) const;
    
    // /**
    //  * @brief 同步图层状态缓存（内部使用）
    //  * 更新last_layer_states_数组
    //  */
    // void SyncLayerStates();
    
    // /**
    //  * @brief 创建虚拟Entity对象（内部使用）
    //  * 组合Tower位置和图层偏移，生成用于AnimationPlayer的Entity对象
    //  * @param layer_index 图层索引
    //  * @return 虚拟Entity对象
    //  */
    //Entity CreateVirtualEntity(size_t layer_index) const;
}; 