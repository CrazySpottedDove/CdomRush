#pragma once
#include "Common/macros.h"
#include "ViewModel/GameViewModel/templates/entity.h"

class Fx : public Entity
{
public:
    bool        Insert(Store& store) override { return true; }
    void        Update(Store& store) override {}
    virtual Fx* Clone() = 0;
    bool        Remove(Store& store) override { return true; }
};

class Map : public Fx
{
public:
    Map();
    Fx* Clone() override { return new Map(*this); }
};

class LevelFlag : public Fx
{
public:
    LevelFlag();
    Fx* Clone() override { return new LevelFlag(*this); }
};

class CommonUpgradeIcon : public Fx
{
public:
    CommonUpgradeIcon();
    Fx* Clone() override { return new CommonUpgradeIcon(*this); }
};

class Explosion : public Fx
{
public:
    Explosion();
    Fx* Clone() override { return new Explosion(*this); }
};

class ActionFx: public Fx
{
public:
    ID source_id;
    virtual ActionFx* Clone() = 0;
};

class UpgradeToArcherButton : public ActionFx
{
public:
    UpgradeToArcherButton();
    ActionFx* Clone() override { return new UpgradeToArcherButton(*this); }
    void Update(Store& store)override;
};

class UpgradeToMageButton : public ActionFx
{
public:
    UpgradeToMageButton();
    ActionFx* Clone() override { return new UpgradeToMageButton(*this); }
    void      Update(Store& store) override;
};

class UpgradeToEngineerButton : public ActionFx
{
public:
    UpgradeToEngineerButton();
    ActionFx* Clone() override { return new UpgradeToEngineerButton(*this); }
    void      Update(Store& store) override;
};

// class UpgradeToBarrackButton : public ActionFx
// {
// public:
//     UpgradeToBarrackButton();
//     ActionFx* Clone() override { return new UpgradeToBarrackButton(*this); }
//     void      Update(Store& store) override;
// };