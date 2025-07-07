#pragma once
#include "Common/macros.h"
#include "Common/type.h"
#include "ViewModel/GameViewModel/templates/entity.h"
#include <cstddef>

class Fx : public Entity
{
public:
    bool        Insert(Store& store) override { return true; }
    void        Update(Store& store) override {}
    virtual Fx* Clone() = 0;
    void        QueueViewData(Store& store) override;
    bool        Remove(Store& store) override { return true; }
};

class ActionFx : public Fx
{
public:
    ID                source_id;
    void              QueueViewData(Store& store) override;
    virtual ActionFx* Clone() override = 0;
};

class Map : public Fx
{
public:
    Map();
    void QueueViewData(Store& store) override;
    Fx*  Clone() override { return new Map(*this); }
};

class LevelFlag : public Fx
{
public:
    LevelFlag();
    Fx* Clone() override { return new LevelFlag(*this); }
};

class CommonUpgradeButton : public ActionFx
{
public:
    std::size_t price = 0;
    CommonUpgradeButton(const UpgradeTowerParams& params);
    ActionFx* Clone() override { return new CommonUpgradeButton(*this); }
    void      Update(Store& store) override;
};

class Explosion : public Fx
{
public:
    Explosion();
    Fx* Clone() override { return new Explosion(*this); }
};

class BloodSplat : public Fx
{
public:
    BloodSplat();
    Fx* Clone() override { return new BloodSplat(*this); }
};

class UpgradeToArcherButton : public ActionFx
{
public:
    UpgradeToArcherButton(const UpgradeTowerParams& params);
    ActionFx* Clone() override { return new UpgradeToArcherButton(*this); }
    void      Update(Store& store) override;
};

class UpgradeToMageButton : public ActionFx
{
public:
    UpgradeToMageButton(const UpgradeTowerParams& params);
    ActionFx* Clone() override { return new UpgradeToMageButton(*this); }
    void      Update(Store& store) override;
};

class UpgradeToEngineerButton : public ActionFx
{
public:
    UpgradeToEngineerButton(const UpgradeTowerParams& params);
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

class SellTowerButton : public ActionFx
{
public:
    SellTowerButton(const SellTowerParams& params);
    ActionFx* Clone() override { return new SellTowerButton(*this); }
};

class TextFx : public Fx
{
public:
    virtual std::string Stringfy(Store& store) = 0;
    void                QueueViewData(Store& store) override;
};

class GoldStat : public TextFx
{
public:
    GoldStat();
    Fx*         Clone() override { return new GoldStat(*this); }
    std::string Stringfy(Store& store) override;
};

class LifeStat : public TextFx
{
public:
    LifeStat();
    Fx*         Clone() override { return new LifeStat(*this); }
    std::string Stringfy(Store& store) override;
};

class WaveStat : public TextFx
{
public:
    WaveStat();
    Fx*         Clone() override { return new WaveStat(*this); }
    std::string Stringfy(Store& store) override;
};

class TopLeft : public Fx
{
public:
    TopLeft();
    Fx*  Clone() override { return new TopLeft(*this); }
    void QueueViewData(Store& store) override;
};

class ActionTextFx : public ActionFx
{
public:
    virtual std::string Stringfy(Store& store) = 0;
    void                QueueViewData(Store& store) override;
};

class PriceTagText: public ActionTextFx
{
public:
    PriceTagText(const std::string& text);
    ActionFx* Clone() override { return new PriceTagText(*this); }
    std::string text;
    std::string Stringfy(Store& store) override {
        return text;
    };
};