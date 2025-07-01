#pragma once
#include "Model/templates/entity.h"

class fx : public Entity
{
public:
    bool Insert(Store& store) override { return true; }
    void Update(Store& store) override {}
    bool Remove(Store& store) override { return true; }
};

enum class FxType
{
    Explosion,
    None
};