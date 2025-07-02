#pragma once
#include <vector>
#include <string>
#include "Common/macros.h"

struct LevelAsset
{
    std::vector<std::string> required_textures;
    std::vector<std::string> required_sounds;
};

typedef std::pair<std::string, Position> LevelData;
