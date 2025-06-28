#pragma once
#include "utils/macros.h"
#include <cstddef>
#include <string>
#include <vector>
typedef std::vector<Position> SubPath;
typedef std::vector<SubPath>  Path;
typedef std::vector<Path> Paths;
class PathManager
{
public:
    PathManager() = default;
    void ReadPathsFromLua(const std::string& file);
    void ClearPaths() { paths.clear(); }
    const Position& GetPathPoint(const std::size_t path_id, const std::size_t sub_path_id, const std::size_t waypoint_id) const{
        return paths[path_id][sub_path_id][waypoint_id];
    }
private:
    Paths paths;
};