#pragma once
#include "Common/macros.h"
#include <cstddef>
#include <string>
#include <vector>
#include "ViewModel/GameViewModel/components/path.h"

class PathManager
{
public:
    PathManager() = default;
    void ReadPathsFromLua(const std::string& file);
    void ClearPaths() { paths.clear(); }
    const Position& GetPathPoint(const std::size_t path_id, const std::size_t sub_path_id, const std::size_t waypoint_id) const{
        return paths[path_id][sub_path_id][waypoint_id];
    }
    const Position& GetPathPoint(const PathInfo& path_info) const {
        return GetPathPoint(path_info.path_id, path_info.subpath_id, path_info.waypoint_id);
    }
    const SubPath& GetSubPath(const PathInfo& path_info)const{
        return paths[path_info.path_id][path_info.subpath_id];
    }
    bool IsPathEnd(const PathInfo& path_info) const {
        return path_info.waypoint_id + 1 >= paths[path_info.path_id][path_info.subpath_id].size();
    }
    int GetPointsRemained(const PathInfo& path_info) const {
        return paths[path_info.path_id][path_info.subpath_id].size() - path_info.waypoint_id - 1;
    }
private:
    Paths paths;
};