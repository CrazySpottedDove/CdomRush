#pragma once
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <functional>

class Store;
#include "View/animation/animationPlayer.h"
#include "EnemyUI.h"
#include "SoldierUI.h"
#include "BulletUI.h"
#include "TowerUI.h"
// #include "FlagUI.h"
#include "FxUI.h"
class Fx;
/**
 * @brief UIManager类 - 顶层UI管理器
 *
 * 功能：
 * 1. 维护四个映射容器：EnemyUIs、SoldierUIs、BulletUIs、TowerUIs
 * 2. 维护关卡选择旗子容器：FlagUIs
 * 3. 与Store联系由实体指针获取实体数据
 * 4. 与AnimationPlayer联系获取美术资源
 * 5. 管理UI对象的生命周期，实体指针到UI对象的映射
 * 6. 提供统一的渲染接口和关卡选择支持
 */
class UIManager
{
public:
    /**
     * @brief 构造函数
     * @param animation_manager AnimationManager引用，用于FlagUI自渲染
     */
    explicit UIManager();

    /**
     * @brief 析构函数
     */
    ~UIManager() = default;

    // /**
    //  * @brief 更新UI管理器
    //  *
    //  * 职责：
    //  * 1. 同步Store中的实体状态
    //  * 2. 创建新实体的UI对象
    //  * 3. 清理已死亡/销毁实体的UI对象
    //  */
    // void Update();

    /**
     * @brief 渲染所有UI
     * @param window 渲染窗口
     * @param scale 全局缩放比例
     */
    void RenderAll(sf::RenderWindow& window, const sf::Vector2f& scale = {1.0f, 1.0f});

    /**
     * @brief 单独渲染地图
     * @param window 渲染窗口
     * @param map_prefix 地图资源前缀（如"screen_map"）
     * @param position 地图位置
     * @param scale 地图缩放比例
     */
    void RenderMap(sf::RenderWindow& window, const std::string& map_prefix,
                   const sf::Vector2f& position = {0.0f, 0.0f},
                   const sf::Vector2f& scale = {1.0f, 1.0f});

    /**
     * @brief 获取统计信息
     */
    struct UIStats {
        size_t enemy_count;
        size_t soldier_count;
        size_t bullet_count;
        size_t tower_count;
        size_t flag_count;
    };
    UIStats GetStats() const;

    // ===============================
    // 地图管理功能
    // ===============================

    /**
     * @brief 设置当前地图
     * @param map_prefix 地图资源前缀（如"screen_map", "menu_background"等）
     * @param position 地图位置（默认0,0）
     * @param scale 地图缩放（默认1.0）
     */
    void SetCurrentMap(const std::string& map_prefix,
                       const sf::Vector2f& position = {0.0f, 0.0f},
                       const sf::Vector2f& scale = {1.0f, 1.0f});

    /**
     * @brief 清除当前地图（不再渲染地图背景）
     */
    void ClearCurrentMap();

    /**
     * @brief 检查是否有地图在显示
     * @return true 如果当前有地图
     */
    bool HasCurrentMap() const { return !current_map_prefix_.empty(); }

    /**
     * @brief 获取当前地图前缀
     * @return 当前地图前缀，如果没有地图则返回空字符串
     */
    const std::string& GetCurrentMapPrefix() const { return current_map_prefix_; }

    void QueueEnemyUI(Enemy* enemy);
    void QueueSoldierUI(Soldier* soldier);
    void QueueBulletUI(Bullet* bullet);
    void QueueTowerUI(Tower* tower);
    void QueueFxUI(Fx* fx);
    void DeQueueEnemyUI(Enemy* enemy);
    void DeQueueSoldierUI(Soldier* soldier);
    void DeQueueBulletUI(Bullet* bullet);
    void DeQueueTowerUI(Tower* tower);
    void DeQueueFxUI(Fx* fx);
    void RenderEnemyUI(sf::RenderWindow& window, Enemy* enemy, const sf::Vector2f& scale = {1.0f, 1.0f});
    void RenderSoldierUI(sf::RenderWindow& window, Soldier* soldier, const sf::Vector2f& scale = {1.0f, 1.0f});
    void RenderBulletUI(sf::RenderWindow& window, Bullet* bullet, const sf::Vector2f& scale = {1.0f, 1.0f});
    void RenderTowerUI(sf::RenderWindow& window, Tower* tower, const sf::Vector2f& scale = {1.0f, 1.0f});
    void RenderFxUI(sf::RenderWindow& window, Fx* fx, const sf::Vector2f& scale = {1.0f, 1.0f});
    /**
     * @brief 通用渲染接口 - 接受Position和Animation(实际上与各个UI中的逻辑一样？)
     * @param window 渲染窗口
     * @param position 渲染位置
     * @param animation 动画数据（会被修改）
     * @param context 动画上下文（调用者管理）
     * @param scale 缩放比例
     */
    void RenderAnimationAtPosition(sf::RenderWindow& window, const Position& position,
                                  Animation& animation, AnimationContext& context,
                                  const sf::Vector2f& scale = {1.0f, 1.0f});

    // ===============================
    // 关卡选择功能
    // ===============================

    /**
     * @brief 处理鼠标事件（用于旗子交互）
     * @param event SFML事件
     * @param window 窗口引用（用于坐标转换）
     * @return true 如果事件被处理
     */
    bool HandleMouseEvent(const sf::Event& event, const sf::RenderWindow& window);

    /**
     * @brief 设置全局关卡选择回调函数（已简化，推荐使用InitializeLevelFlags的内置回调）
     * @param callback 关卡被选中时调用的回调函数
     * @deprecated 当前设计使用直接GameState切换，此方法可选
     */
    void SetGlobalLevelSelectionCallback(std::function<void(int)> callback);

    /**
     * @brief 清除所有旗子的选中状态
     */
    void ClearAllFlagSelections();

    /**
     * @brief 获取当前选中的关卡ID
     * @return 选中的关卡ID，如果没有选中则返回-1
     */
    int GetSelectedLevelId() const { return selected_level_id_; }

    // 五个映射容器：实体指针 -> UI对象
    std::unordered_map<Enemy*, std::unique_ptr<EnemyUI>>     enemy_uis_;     ///< Enemy映射
    std::unordered_map<Soldier*, std::unique_ptr<SoldierUI>> soldier_uis_;   ///< Soldier映射
    std::unordered_map<Bullet*, std::unique_ptr<BulletUI>>   bullet_uis_;    ///< Bullet映射
    std::unordered_map<Tower*, std::unique_ptr<TowerUI>>     tower_uis_;     ///< Tower映射
    std::unordered_map<Fx*, std::unique_ptr<FxUI>>         fx_uis_;      ///< 关卡旗子映射（level_id -> FlagUI）

    Store*                                                   store_;         ///< Store指针
    std::unique_ptr<AnimationPlayer>                         animation_player_;   ///< AnimationPlayer实例
    // const AnimationManager&                                  animation_manager_;  ///< AnimationManager引用（用于FlagUI自渲染）

    // ===============================
    // 通用渲染用的UI实例
    // ===============================
    // std::unique_ptr<EnemyUI>   universal_enemy_ui_;      ///< 通用Enemy渲染用的UI实例
    // std::unique_ptr<SoldierUI> universal_soldier_ui_;    ///< 通用Soldier渲染用的UI实例
    // std::unique_ptr<BulletUI>  universal_bullet_ui_;     ///< 通用Bullet渲染用的UI实例
    // std::unique_ptr<TowerUI>   universal_tower_ui_;      ///< 通用Tower渲染用的UI实例

private:

    // ===============================
    // 地图状态管理
    // ===============================
    std::string current_map_prefix_;                                      ///< 当前地图前缀
    sf::Vector2f current_map_position_;                                   ///< 当前地图位置
    sf::Vector2f current_map_scale_;                                      ///< 当前地图缩放

    // ===============================
    // 关卡选择状态管理
    // ===============================
    int selected_level_id_;                                               ///< 当前选中的关卡ID（-1表示未选中）
    std::function<void(int)> global_level_selection_callback_;            ///< 全局关卡选择回调函数

    /**
     * @brief 处理旗子点击事件（内部使用）
     * @param level_id 被点击的关卡ID
     */
    void OnFlagClicked(int level_id);

    /**
     * @brief 确保通用UI实例已初始化（内部使用）
     */
    void EnsureUniversalUIInstancesInitialized();

    // /**
    //  * @brief 同步Enemy列表（内部使用）
    //  * 检查Store中的Enemy列表，创建新UI或清理旧UI
    //  */
    // void SyncEnemyUIs();

    // /**
    //  * @brief 同步Soldier列表（内部使用）
    //  */
    // void SyncSoldierUIs();

    // /**
    //  * @brief 同步Bullet列表（内部使用）
    //  */
    // void SyncBulletUIs();

    // /**
    //  * @brief 同步Tower列表（内部使用）
    //  */
    // void SyncTowerUIs();

    // /**
    //  * @brief 清理死亡实体的UI（内部使用）
    //  * @tparam EntityType 实体类型
    //  * @tparam UIType UI类型
    //  * @param ui_map UI映射容器
    //  * @param entities Store中的实体列表
    //  */
    // template<typename EntityType, typename UIType>
    // void CleanupDeadUIs(std::unordered_map<EntityType*, std::unique_ptr<UIType>>& ui_map,
    //                     const std::vector<EntityType*>& entities);
};