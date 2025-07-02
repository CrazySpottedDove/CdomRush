#pragma once
#include "Manager/resourceManager/animationManager.h"
#include "Model/components/animation.h"
#include "Model/components/state.h"
#include <SFML/Graphics.hpp>

/**
 * @brief 动画上下文 - 存储每个UI独立的动画状态
 *
 * 这个结构体解决了AnimationPlayer全局状态冲突的问题。
 * 每个UI类（EnemyUI、BulletUI等）都会拥有自己的AnimationContext实例，
 * 从而实现动画状态的完全隔离。
 */
struct AnimationContext
{
    bool loop_enabled = true;    // 是否循环播放
    bool is_paused    = false;   // 是否暂停

    /**
     * @brief 检查是否需要状态切换（基于animation）
     * @param animation 动画对象
     * @return true 如果需要切换状态
     */
    bool needs_state_change(const Animation& animation) const
    {
        return animation.last_state != animation.state;
    }

    /**
     * @brief 重置上下文到初始状态
     */
    void reset()
    {
        loop_enabled = true;
        is_paused    = false;
    }
};

/**
 * @brief 动画播放器类 - 帧驱动的动画控制，支持Entity和Unit
 *
 * 重构后的设计：
 * 1. AnimationPlayer不再维护全局状态，完全无状态
 * 2. 所有状态都通过AnimationContext参数传递
 * 3. 每个UI对象拥有自己的AnimationContext，实现状态隔离
 * 4. 支持多线程安全操作
 *
 * 使用方法：
 * 1. 每个UI类创建自己的AnimationContext实例
 * 2. 调用AnimationPlayer方法时传递context引用
 * 3. AnimationPlayer基于context和entity/unit进行动画处理
 */
class AnimationPlayer
{
public:
    /**
     * @brief 构造函数
     * @param animation_manager AnimationManager的引用，用于获取动画数据
     */
    explicit AnimationPlayer(const AnimationManager& animation_manager);

    /*
     * @brief 统一渲染接口
     * @param window 渲染窗口
     * @param position 渲染位置
     * @param animation 动画数据
     * @param context 动画上下文引用
     * @param scale 缩放比例（默认1.0，不缩放）
     */

    void RenderAnimation(sf::RenderWindow& window, const sf::Vector2f& position,
                         Animation& animation, const sf::Vector2f& scale = {1.0f, 1.0f});

    // /**
    // * @brief 统一更新接口，同一状态的下一帧渲染
    // * @param animation 动画数据
    // * @param context 动画上下文引用
    // */
    // void UpdateAnimation(Animation& animation, AnimationContext& context);

    // /**
    //  * @brief 统一开始播放接口
    //  * @param animation 动画数据
    //  * @param context 动画上下文引用
    //  * @param state 要播放的动画状态
    //  * @param loop 是否循环播放（默认true？）
    //  */
    // void PlayAnimationState(Animation& animation, AnimationContext& context,bool loop = true);

    /**
     * @brief 渲染地图背景
     * @param window 渲染窗口
     * @param map_prefix 地图资源前缀（"screen_map"）
     * @param position 地图渲染位置（默认0,0）
     * @param scale 地图缩放比例（默认1.0）
     *
     * Note：地图相当于静态动画，状态固定为Idle，from 1 to 1
     */
    void RenderMap(sf::RenderWindow& window, const std::string& map_prefix,
                   const sf::Vector2f& position = {0.0f, 0.0f},
                   const sf::Vector2f& scale    = {1.0f, 1.0f}) const;


    // /**
    // * @brief 检查动画是否播放完成
    // */
    // bool IsAnimationFinished(Animation& animation, const AnimationContext& context) const;

private:
    const AnimationManager& animation_manager_;

    // /**
    //  * @brief 计算动画的实际帧索引（加上动画组的起始偏移）
    //  * @param animation 动画组件
    //  * @param state 当前状态
    //  * @return 实际的帧索引
    //  */
    // std::size_t CalculateActualFrameIndex(const Animation& animation, State state) const;

    // /**
    //  * @brief 获取指定状态的动画总帧数
    //  * @param prefix 动画前缀
    //  * @param state 状态
    //  * @return 总帧数
    //  */
    // std::size_t GetAnimationFrameCount(const std::string& prefix, State state) const;

    // /**
    //  * @brief 检查指定的帧ID是否有效
    //  * @param prefix 动画前缀
    //  * @param state 状态
    //  * @param frame_id 帧ID
    //  * @return true 如果帧ID有效
    //  */
    // bool IsValidFrameId(const std::string& prefix, State state, std::size_t frame_id) const;

    // ===============================
    // Unit专用
    // ===============================


    // /**
    //  * @brief 检查是否需要水平翻转（仅Unit使用）
    //  * @param unit_state Unit的状态
    //  * @param heading Unit的方向
    //  * @return true 如果需要翻转
    //  */
    // bool ShouldFlipHorizontally(State unit_state, Heading heading) const;

    // ===============================
    // 通用
    // ===============================
};
