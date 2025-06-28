#pragma once
#include <SFML/System/Vector2.hpp>
#include <cstddef>

struct PathInfo
{
    int          path_id     = -1;   // 敌人所属路径 id，如果为 -1 表示无视路径
    int          subpath_id  = -1;   // 子路径 id
    std::size_t  waypoint_id = 0;    // 处于
};