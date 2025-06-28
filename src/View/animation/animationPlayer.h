#pragma once
#include <SFML/Graphics.hpp>
#include "Manager/resourceManager/animationManager.h"
#include "Model/components/animation.h"
#include "Model/components/state.h"

/**
 * @brief 动画播放器类 - 基于帧驱动的动画控制
 * 
 * 这个类负责：
 * 1. 管理动画的播放状态
 * 2. 控制动画帧的前进
 * 3. 将动画渲染到屏幕上
 * 
 * 使用方法：
 * 1. 创建AnimationPlayer实例
 * 2. 调用PlayAnimation开始播放动画
 * 3. 调用NextFrame()手动前进到下一帧，或者调用Update()自动前进
 * 4. 调用Render渲染当前帧
 */
class AnimationPlayer
{
public:
    /**
     * @brief 构造函数
     * @param animation_manager AnimationManager的引用，用于获取动画数据
     */
    explicit AnimationPlayer(const AnimationManager& animation_manager);
    
    /**
     * @brief 开始播放指定状态的动画
     * @param animation 动画组件引用
     * @param state 要播放的状态
     * @param loop 是否循环播放（默认true）
     * 
     * 例子：
     * animation_player.PlayAnimation(my_animation, State::Walk, true);
     */
    void PlayAnimation(Animation& animation, State state, bool loop = true);
    
    /**
     * @brief 手动前进到下一帧
     * @param animation 动画组件引用
     * @return true 如果成功前进到下一帧，false 如果动画已结束
     * 
     * 例子：
     * if (animation_player.NextFrame(my_animation)) {
     *     // 成功前进到下一帧
     * }
     */
    bool NextFrame(Animation& animation);
    
    /**
     * @brief 自动更新动画（根据内部条件决定是否前进到下一帧）
     * @param animation 动画组件引用
     * 
     * 例子：
     * animation_player.Update(my_animation); // 在游戏主循环中调用
     */
    void Update(Animation& animation);
    
    /**
     * @brief 渲染当前帧到指定位置
     * @param window 渲染窗口
     * @param animation 动画组件
     * @param position 渲染位置（世界坐标）
     * @param scale 缩放比例（默认1.0，不缩放）
     * 
     * 例子：
     * animation_player.Render(window, my_animation, sf::Vector2f(100, 200));
     */
    void Render(sf::RenderWindow& window, const Animation& animation, 
               const sf::Vector2f& position, const sf::Vector2f& scale = {1.0f, 1.0f});
    
    /**
     * @brief 停止当前动画
     * @param animation 动画组件引用
     */
    void StopAnimation(Animation& animation);
    
    /**
     * @brief 重置动画到第一帧
     * @param animation 动画组件引用
     */
    void ResetAnimation(Animation& animation);
    
    /**
     * @brief 检查动画是否播放完成（仅对非循环动画有意义）
     * @param animation 动画组件
     * @return true 如果动画播放完成
     */
    bool IsAnimationFinished(const Animation& animation) const;
    
    /**
     * @brief 暂停/恢复动画
     * @param animation 动画组件引用
     * @param paused true=暂停，false=恢复
     */
    void SetPaused(Animation& animation, bool paused);
    
    /**
     * @brief 跳转到指定帧
     * @param animation 动画组件引用
     * @param frame_id 目标帧ID（相对于动画组的起始帧）
     * @return true 如果跳转成功
     */
    bool JumpToFrame(Animation& animation, std::size_t frame_id);

private:
    const AnimationManager& animation_manager_;  // AnimationManager引用
    
    // 当前播放状态
    State current_state_;           // 当前播放的状态
    bool loop_enabled_;             // 是否循环播放
    bool is_paused_;                // 是否暂停
    
    /**
     * @brief 计算动画的实际帧索引（加上动画组的起始偏移）
     * @param animation 动画组件
     * @param state 当前状态
     * @return 实际的帧索引
     */
    std::size_t CalculateActualFrameIndex(const Animation& animation, State state) const;
    
    /**
     * @brief 获取指定状态的动画总帧数
     * @param prefix 动画前缀
     * @param state 状态
     * @return 总帧数
     */
    std::size_t GetAnimationFrameCount(const std::string& prefix, State state) const;
    
    /**
     * @brief 检查指定的帧ID是否有效
     * @param prefix 动画前缀
     * @param state 状态
     * @param frame_id 帧ID
     * @return true 如果帧ID有效
     */
    bool IsValidFrameId(const std::string& prefix, State state, std::size_t frame_id) const;
};
