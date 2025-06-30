#pragma once
#include <SFML/Graphics.hpp>
#include "Manager/resourceManager/animationManager.h"
#include "Model/components/animation.h"
#include "Model/components/state.h"
#include "Model/templates/entity.h"
#include "Model/templates/unit.h"

/**
 * @brief 动画上下文 - 存储每个UI独立的动画状态
 * 
 * 这个结构体解决了AnimationPlayer全局状态冲突的问题。
 * 每个UI类（EnemyUI、BulletUI等）都会拥有自己的AnimationContext实例，
 * 从而实现动画状态的完全隔离。
 */
struct AnimationContext {
    State current_state = State::Idle;      // 当前播放的动画状态
    bool loop_enabled = true;               // 是否循环播放
    bool is_paused = false;                 // 是否暂停
    
    /**
     * @brief 检查是否需要状态切换
     * @param target_state 目标状态
     * @return true 如果需要切换状态
     */
    bool needs_state_change(State target_state) const {
        return current_state != target_state;
    }
    
    /**
     * @brief 更新当前状态
     * @param new_state 新的状态
     */
    void update_state(State new_state) {
        current_state = new_state;
    }
    
    /**
     * @brief 重置上下文到初始状态
     */
    void reset() {
        current_state = State::Idle;
        loop_enabled = true;
        is_paused = false;
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
    
    // ===============================
    // Entity接口 - 基础动画功能
    // ===============================
    
    /**
     * @brief 播放指定状态的动画（Entity版本 - 基础）
     * @param entity Entity对象引用
     * @param context 动画上下文引用
     * @param state 要播放的动画状态
     * @param loop 是否循环播放（默认true）
     */
    void PlayAnimation(Entity& entity, AnimationContext& context, State state, bool loop = true);
    
    /**
     * @brief 手动前进到下一帧（Entity版）
     * @param entity Entity对象引用
     * @param context 动画上下文引用
     * @return true 如果成功前进到下一帧，false 如果动画已结束
     */
    bool NextFrame(Entity& entity, AnimationContext& context);
    
    /**
     * @brief 自动更新动画（Entity版本）
     * @param entity Entity对象引用
     * @param context 动画上下文引用
     */
    void Update(Entity& entity, AnimationContext& context);
    
    /**
     * @brief 渲染当前帧到指定位置（Entity版本）
     * @param window 渲染窗口
     * @param entity Entity对象
     * @param context 动画上下文引用
     * @param scale 缩放比例（默认1.0，不缩放）
     * 
     * Note：Entity版本使用entity.position作为渲染位置
     */
    void Render(sf::RenderWindow& window, const Entity& entity, const AnimationContext& context,
               const sf::Vector2f& scale = {1.0f, 1.0f});
    
    /**
     * @brief 停止当前动画（Entity版本）
     * @param entity Entity对象引用
     * @param context 动画上下文引用
     */
    void StopAnimation(Entity& entity, AnimationContext& context);
    
    /**
     * @brief 重置动画到第一帧（Entity版本）
     * @param entity Entity对象引用
     * @param context 动画上下文引用
     */
    void ResetAnimation(Entity& entity, AnimationContext& context);
    
    /**
     * @brief 检查动画是否播放完成（Entity版本）
     * @param entity Entity对象
     * @param context 动画上下文引用
     * @return true 如果动画播放完成
     */
    bool IsAnimationFinished(const Entity& entity, const AnimationContext& context) const;
    
    /**
     * @brief 暂停/恢复动画（Entity版本）
     * @param entity Entity对象引用
     * @param context 动画上下文引用
     * @param paused true=暂停，false=恢复
     */
    void SetPaused(Entity& entity, AnimationContext& context, bool paused);
    
    /**
     * @brief 跳转到指定帧（Entity版本）
     * @param entity Entity对象引用
     * @param context 动画上下文引用
     * @param frame_id 目标帧ID（相对于动画组的起始帧）
     * @return true 如果跳转成功
     */
    bool JumpToFrame(Entity& entity, AnimationContext& context, std::size_t frame_id);
    
    // ===============================
    // Unit接口 - 状态驱动+翻转
    // ===============================
    
    /**
     * @brief 开始播放动画（Unit版本 - 状态驱动+翻转）
     * @param unit Unit对象引用（包含state、heading、animation等信息）
     * @param context 动画上下文引用
     * @param loop 是否循环播放（默认true）
     * 
     * 根据unit.state自动选择合适的动画
     * 根据unit.heading决定是否翻转
     */
    void PlayAnimation(Unit& unit, AnimationContext& context, bool loop = true);
    
    /**
     * @brief 手动前进到下一帧（Unit版本）
     * @param unit Unit对象引用
     * @param context 动画上下文引用
     * @return true 如果成功前进到下一帧，false 如果动画已结束
     */
    bool NextFrame(Unit& unit, AnimationContext& context);
    
    /**
     * @brief 自动更新动画（Unit版本）
     * @param unit Unit对象引用
     * @param context 动画上下文引用
     * 
     * 会自动检测Unit状态变化并切换动画
     */
    void Update(Unit& unit, AnimationContext& context);
    
    /**
     * @brief 渲染当前帧到指定位置（Unit版本 - 支持翻转）
     * @param window 窗口
     * @param unit Unit对象（包含动画信息和方向信息）
     * @param context 动画上下文引用
     * @param scale 缩放比例（默认1.0，不缩放）
     * 
     * 注意：Unit版本使用unit.position作为渲染位置，并支持翻转
     */
    void Render(sf::RenderWindow& window, const Unit& unit, const AnimationContext& context,
               const sf::Vector2f& scale = {1.0f, 1.0f});
    
    /**
     * @brief 停止当前动画（Unit版本）
     * @param unit Unit对象引用
     * @param context 动画上下文引用
     */
    void StopAnimation(Unit& unit, AnimationContext& context);
    
    /**
     * @brief 重置动画到第一帧（Unit版本）
     * @param unit Unit对象引用
     * @param context 动画上下文引用
     */
    void ResetAnimation(Unit& unit, AnimationContext& context);
    
    /**
     * @brief 检查动画是否播放完成（Unit版本）
     * @param unit Unit对象
     * @param context 动画上下文引用
     * @return true 如果动画播放完成
     */
    bool IsAnimationFinished(const Unit& unit, const AnimationContext& context) const;
    
    /**
     * @brief 暂停/恢复动画（Unit版本）
     * @param unit Unit对象引用
     * @param context 动画上下文引用
     * @param paused true=暂停，false=恢复
     */
    void SetPaused(Unit& unit, AnimationContext& context, bool paused);
    
    /**
     * @brief 跳转到指定帧（Unit版本）
     * @param unit Unit对象引用
     * @param context 动画上下文引用
     * @param frame_id 目标帧ID（相对于动画组的起始帧）
     * @return true 如果跳转成功
     */
    bool JumpToFrame(Unit& unit, AnimationContext& context, std::size_t frame_id);
    
    /**
     * @brief 渲染地图背景
     * @param window 渲染窗口
     * @param map_prefix 地图资源前缀（"screen_map"）
     * @param position 地图渲染位置（默认0,0）
     * @param scale 地图缩放比例（默认1.0）
     * 
     * Note：地图相当于静态动画，状态固定为Idle，from 1 to 1
     */
    void RenderMap(sf::RenderWindow& window, 
                   const std::string& map_prefix,
                   const sf::Vector2f& position = {0.0f, 0.0f},
                   const sf::Vector2f& scale = {1.0f, 1.0f}) const;

private:
    const AnimationManager& animation_manager_;  
    
    // ===============================
    // Unit专用
    // ===============================
    
    
    /**
     * @brief 检查是否需要水平翻转（仅Unit使用）
     * @param unit_state Unit的状态
     * @param heading Unit的方向
     * @return true 如果需要翻转
     */
    bool ShouldFlipHorizontally(State unit_state, Heading heading) const;
    
    // ===============================
    // 通用
    // ===============================
    
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
