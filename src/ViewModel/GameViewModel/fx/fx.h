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

class Map : public Fx{
    public:
    Map();
    Map(const std::string& prefix);
    Fx* Clone() override { return new Map(*this); }
};

class LevelFlag : public Fx
{
public:
    LevelFlag();
    Fx* Clone() override { return new LevelFlag(*this); }
};

class CommonUpgradeIcon: public Fx{
public:
    CommonUpgradeIcon();
    Fx* Clone() override { return new CommonUpgradeIcon(*this); }
};