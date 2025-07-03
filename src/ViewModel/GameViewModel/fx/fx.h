#pragma once
#include "ViewModel/GameViewModel/templates/entity.h"

class Fx : public Entity
{
public:
    bool        Insert(Store& store) override { return true; }
    void        Update(Store& store) override {}
    virtual Fx* Clone() = 0;
    bool        Remove(Store& store) override { return true; }
};

enum class FxType
{
    Explosion,
    LevelFlag,
    None
};

class LevelFlag : public Fx
{
public:
    LevelFlag();
    Fx* Clone() override { return new LevelFlag(*this); }
};