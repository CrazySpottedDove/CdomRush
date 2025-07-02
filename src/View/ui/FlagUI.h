// #pragma once
// #include "BaseUI.h"
// #include "View/animation/animationPlayer.h"
// #include "Manager/resourceManager/animationManager.h"
// #include <SFML/Graphics.hpp>
// #include <functional>
// #include "FxUI.h"

// /**
//  * @brief 关卡状态枚举
//  */
// enum class LevelStatus {
//     Available,  /// 可选
//     Selected,   ///< 选中状态
//     Completed   ///< 完成状态
// };

// /**
//  * @brief FlagUI类 - 关卡选择旗子UI组件
//  *
//  * 简化设计原则：
//  * 1. 所有状态都使用相同的动画资源（map_flag）
//  * 2. 始终使用State::Idle状态播放飘扬动画
//  * 3. 通过sprite颜色叠加实现悬停高亮效果
//  * 4. 状态差异仅体现在交互响应上，不体现在视觉上
//  */
// class FlagUI : public FxUI
// {
// public:
//     /**
//      * @brief 构造函数
//      * @param level_id 关卡ID
//      * @param position 旗子固定位置（世界坐标）
//      * @param animation_player AnimationPlayer引用
//      * @param animation_manager AnimationManager引用（用于自渲染）
//      * @param initial_status 初始关卡状态（默认可选择）
//      */
//     explicit FlagUI(std::string level_name,
//                     const sf::Vector2f& position,
//                     AnimationPlayer& animation_player,
//                     const AnimationManager& animation_manager,
//                     LevelStatus initial_status = LevelStatus::Available);

//     /**
//      * @brief 渲染旗子（实现高亮）
//      * @param window 渲染窗口
//      * @param scale 全局缩放比例
//      */
//     void Render(sf::RenderWindow& window, const Position& position, Animation& animation,const sf::Vector2f& scale = {1.0f, 1.0f}) override;

//     /**
//      * @brief 处理鼠标事件
//      * @param event SFML事件
//      * @param mouse_world_pos 鼠标世界坐标位置
//      * @return true 如果事件被处理
//      */
//     bool HandleMouseEvent(const sf::Event& event, const sf::Vector2f& mouse_world_pos);

//     /**
//      * @brief 设置关卡状态
//      * @param status 新状态
//      */
//     void SetLevelStatus(LevelStatus status);

//     /**
//      * @brief 获取关卡状态
//      * @return 当前状态
//      */
//     LevelStatus GetLevelStatus() const { return level_status_; }

//     /**
//      * @brief 获取关卡ID
//      * @return 关卡ID
//      */
//     std::string GetLevelName() const { return level_name_; }

//     /**
//      * @brief 获取旗子位置
//      * @return 旗子固定位置
//      */
//     sf::Vector2f GetPosition() const { return position_; }

//     /**
//      * @brief 检查鼠标是否在旗子附近的接收区域
//      * @param mouse_pos 鼠标位置（世界坐标）
//      * @return true 如果在点击区域内
//      */
//     bool IsMouseOver(const sf::Vector2f& mouse_pos) const;


// private:
//     std::string level_name_;                ///< 关卡名称
//     sf::Vector2f position_;                 ///< 旗子固定位置
//     LevelStatus level_status_;              ///< 关卡状态
//     bool is_hovered_;                       ///< 是否鼠标悬停
//     bool initialized_;                      ///< 是否已初始化动画

//     AnimationPlayer& animation_player_;     ///< AnimationPlayer引用
//     const AnimationManager& animation_manager_; ///< AnimationManager引用（用于自渲染）
//     AnimationContext animation_context_;    ///< 独立的动画上下文


//     /**
//      * @brief 初始化旗子动画（内部使用）
//      * 所有旗子都使用相同的map_flag动画
//      */
//     void InitializeAnimation();

//     /**
//      * @brief 更新动画帧（内部使用）
//      * 自动更新动画，无需状态切换
//      */
//     void UpdateAnimation();

//     /**
//      * @brief 获取当前动画帧（内部使用）
//      * 直接从AnimationManager获取帧数据
//      * @return 当前帧的sprite，用于自定义渲染
//      */
//     sf::Sprite GetCurrentFrameSprite(const sf::Vector2f& position,
//                                       Animation& animation,
//                                       //const AnimationContext& context,
//                                       const sf::Vector2f& scale) const;

//     /**
//      * @brief 获取旗子的点击区域（内部使用）
//      * @return 点击区域的矩形
//      */
//     sf::FloatRect GetClickBounds() const;

//     /**
//      * @brief 处理鼠标悬停逻辑（内部使用）
//      * @param mouse_pos 鼠标位置
//      */
//     void UpdateHoverState(const sf::Vector2f& mouse_pos);
// };