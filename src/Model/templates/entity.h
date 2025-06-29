#pragma once
#include "Model/components/animation.h"
#include "utils/macros.h"
class Store;

// 参与主时钟更新的实体

class Entity{
public:
    Position position;
    Animation animation;
    virtual ~Entity() = default;
    virtual bool Insert(Store& store) = 0;
    virtual void Update(Store& store) = 0;
    virtual bool Remove(Store& store) = 0;
};