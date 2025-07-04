#pragma once
#include <cstddef>
#include "Common/type.h"
#include <vector>
#include <string>
#include <queue>
#include "ViewModel/SpriteViewModel/stringMap.h"

struct SubWave
{
    double      time;          // 在本波中的什么时间产生
    std::size_t path_id;       // 属于什么路径
    std::size_t subpath_id;    // 属于什么子路径
    std::size_t count = 1;     // 产生多少个敌人
    double      gap   = 0.0;   // 产生敌人之间的间隔时间
    EnemyType   enemy_type;    // 敌人类型
    SubWave() = default;
    SubWave(const double time, const std::size_t path_id, const std::size_t subpath_id,
            const std::size_t count, const double gap, const std::string& enemy_type):time(time),
            path_id(path_id), subpath_id(subpath_id), count(count), gap(gap)
            {
                this->enemy_type = enemy_type_str_map.at(enemy_type);
            }
};

struct Wave
{
    double duration;                  // 本波持续时间。持续时间结束后，即可准备下一波。
    double preparation_time;          // 准备时间，在准备时间未归零前，玩家可以选择提前出波获得奖励
    std::vector<SubWave> sub_waves;   // 本波的子波，默认已按时间顺序排序好
    Wave() = default;
    Wave(const double duration, const double preparation_time)
        : duration(duration)
        , preparation_time(preparation_time)
    {}
};

struct PendingEnemy
{
    EnemyType enemy_type;
    double    spawn_time;
    size_t    path_id;
    size_t    subpath_id;
    bool      operator>(const PendingEnemy& other) const { return spawn_time > other.spawn_time; }
    PendingEnemy() = default;
    PendingEnemy(const EnemyType& enemy_type, const double spawn_time, const size_t path_id,
                 const size_t subpath_id)
        : enemy_type(enemy_type)
        , spawn_time(spawn_time)
        , path_id(path_id)
        , subpath_id(subpath_id)
    {}
};

typedef std::priority_queue<PendingEnemy, std::vector<PendingEnemy>, std::greater<PendingEnemy>> PendingEnemyQueue;