#pragma once


#include "Model/components/path.h"
#include "Model/templates/unit.h"
class Enemy : public Unit
{
public:
    int gold;
    int life_cost;
    PathInfo path_info;
};