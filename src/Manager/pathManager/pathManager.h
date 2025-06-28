#pragma once
#include "utils/macros.h"
#include <string>
#include <vector>
typedef std::vector<Position> Path;
typedef std::vector<Path>     Paths;
class PathManager
{
public:
    PathManager() = default;
    void ReadPathsFromLua(const std::string& file);
    void ClearPaths() { paths.clear(); }
    
private:
    Paths paths;
};