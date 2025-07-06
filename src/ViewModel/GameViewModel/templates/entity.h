#pragma once
#include "Common/animation.h"
#include "Common/macros.h"
class Store;

// 参与主时钟更新的实体
enum class SourceType
{
    Tower,    // 塔
    Enemy,    // 敌人
    Soldier   // 士兵
};

class Entity
{
public:
    ID                     id = INVALID_ID;   // 实体ID
    Position               position;
    std::vector<Animation> animations;
    virtual ~Entity() = default;
    virtual bool Insert(Store& store);
    virtual void Update(Store& store) = 0;
    virtual void QueueViewData(Store& store);
    virtual bool Remove(Store& store) { return true; };
};