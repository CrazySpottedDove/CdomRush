#pragma once
#include "Model/components/animation.h"
class Store;

// 参与主时钟更新的实体
class Entity{
public:
    Animation animation;
    virtual ~Entity() = default;
    virtual void Insert(Store& store) = 0;
    virtual void Update(Store& store) = 0;
    virtual void Remove(Store& store) = 0;
};