#pragma once
#include "ViewModel/GameViewModel/enemies/enemies.h"
#include <cstddef>
#include <functional>
#include <queue>
#include <vector>
class Store;
struct SubWave{
    double time; // 在本波中的什么时间产生
    std::size_t path_id; // 属于什么路径
    std::size_t subpath_id; // 属于什么子路径
    std::size_t count = 1; // 产生多少个敌人
    double gap = 0.0; // 产生敌人之间的间隔时间
    EnemyType enemy_type; // 敌人类型
    SubWave() = default;
    SubWave(const double time, const std::size_t path_id, const std::size_t subpath_id,
            const std::size_t count, const double gap, const std::string& enemy_type);
};

struct Wave{
    double duration; // 本波持续时间。持续时间结束后，即可准备下一波。
    double preparation_time; // 准备时间，在准备时间未归零前，玩家可以选择提前出波获得奖励
    std::vector<SubWave> sub_waves; // 本波的子波，默认已按时间顺序排序好
    Wave() = default;
    Wave(const double duration, const double preparation_time)
        : duration(duration), preparation_time(preparation_time) {}
};

struct PendingEnemy{
    EnemyType enemy_type;
    double spawn_time;
    size_t path_id;
    size_t subpath_id;
    bool      operator>(const PendingEnemy& other) const { return spawn_time > other.spawn_time; }
    PendingEnemy() = default;
    PendingEnemy(const EnemyType& enemy_type, const double spawn_time, const size_t path_id, const size_t subpath_id)
        : enemy_type(enemy_type), spawn_time(spawn_time), path_id(path_id), subpath_id(subpath_id) {}
};

class WaveManager{
public:
    std::size_t current_wave_index = 0;
    std::size_t current_subwave_index = 0;
    double current_wave_time = 0;
    bool preparing = true;
    WaveManager() = default;

    /**
     * @brief 根据时间，更新波次，衍生敌人
     *
     * @param store
     */
    void Update(Store& store);

    /**
     * @brief 在进入一个关卡时，初始化 WaveManager
     *
     * @param file
     */
    void Init(const std::string& file){
        current_wave_index = 0;
        current_wave_time = 0;
        current_subwave_index = 0;
        preparing = true;
        ClearWaves();
        LoadWavesFromLua(file);
    }

    /**
     * @brief 反映还有没有波次
     *
     * @return true
     * @return false
     */
    bool IsFinished(){
        return current_wave_index >= waves.size();
    }
private:
    std::vector<Wave> waves;
    std::priority_queue<PendingEnemy, std::vector<PendingEnemy>, std::greater<PendingEnemy>> pending_enemies; // 等待生成的敌人
    void ClearWaves()
    {
        waves.clear();
        while (!pending_enemies.empty()) {
            pending_enemies.pop();
        }
    }
    void LoadWavesFromLua(const std::string& file);
};