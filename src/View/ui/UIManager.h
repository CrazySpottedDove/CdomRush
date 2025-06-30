#pragma once
#include <unordered_map>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Manager/store/store.h"
#include "View/animation/animationPlayer.h"
#include "EnemyUI.h"
#include "SoldierUI.h"
#include "BulletUI.h"
#include "TowerUI.h"

/**
 * @brief UIManager类 - 顶层UI管理器
 * 
 * 功能：
 * 1. 维护四个映射容器：EnemyUIs、SoldierUIs、BulletUIs、TowerUIs
 * 2. 与Store联系由实体指针获取实体数据
 * 3. 与AnimationPlayer联系获取美术资源
 * 4. 管理UI对象的生命周期，实体指针到UI对象的映射
 * 5. 提供统一的渲染接口
 */
class UIManager
{
public:
    /**
     * @brief 构造函数
     * @param store Store引用，提供实体数据
     */
    explicit UIManager(Store& store);
    
    /**
     * @brief 析构函数
     */
    ~UIManager() = default;
    
    /**
     * @brief 更新UI管理器
     * 
     * 职责：
     * 1. 同步Store中的实体状态
     * 2. 创建新实体的UI对象
     * 3. 清理已死亡/销毁实体的UI对象
     */
    void Update();
    
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

private:
    Store& store_;                                                      ///< Store引用
    std::unique_ptr<AnimationPlayer> animation_player_;                 ///< AnimationPlayer实例
    
    // 四个映射容器：实体指针 -> UI对象
    std::unordered_map<Enemy*, std::unique_ptr<EnemyUI>> enemy_uis_;    ///< Enemy映射
    std::unordered_map<Soldier*, std::unique_ptr<SoldierUI>> soldier_uis_; ///< Soldier映射
    std::unordered_map<Bullet*, std::unique_ptr<BulletUI>> bullet_uis_; ///< Bullet映射
    std::unordered_map<Tower*, std::unique_ptr<TowerUI>> tower_uis_;    ///< Tower映射
    
    // ===============================
    // 地图状态管理
    // ===============================
    std::string current_map_prefix_;                                      ///< 当前地图前缀
    sf::Vector2f current_map_position_;                                   ///< 当前地图位置
    sf::Vector2f current_map_scale_;                                      ///< 当前地图缩放
    
    /**
     * @brief 同步Enemy列表（内部使用）
     * 检查Store中的Enemy列表，创建新UI或清理旧UI
     */
    void SyncEnemyUIs();
    
    /**
     * @brief 同步Soldier列表（内部使用）
     */
    void SyncSoldierUIs();
    
    /**
     * @brief 同步Bullet列表（内部使用）
     */
    void SyncBulletUIs();
    
    /**
     * @brief 同步Tower列表（内部使用）
     */
    void SyncTowerUIs();
    
    /**
     * @brief 清理死亡实体的UI（内部使用）
     * @tparam EntityType 实体类型
     * @tparam UIType UI类型
     * @param ui_map UI映射容器
     * @param entities Store中的实体列表
     */
    template<typename EntityType, typename UIType>
    void CleanupDeadUIs(std::unordered_map<EntityType*, std::unique_ptr<UIType>>& ui_map,
                        const std::vector<EntityType*>& entities);
}; 