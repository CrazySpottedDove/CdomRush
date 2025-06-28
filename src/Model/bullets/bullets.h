#pragma once
#include <memory>
#include "Model/components/damage.h"
#include "Model/templates/entity.h"
#include "utils/macros.h"
#include "Manager/store/store.h"
class Bullet: public Entity
{
public:
    Position position;
    DamageEvent damage_event;
    virtual std::shared_ptr<Bullet> Clone() const noexcept        = 0;
};

// TODO: 箭矢
// 采用抛物线轨迹
class Arrow : public Bullet
{};

// TODO: 法球
// 采用直线追踪轨迹
class Bolt : public Bullet
{};

// TODO: 炸弹
// OnHit() 需要添加爆炸效果
// 采用抛物线轨迹
class Bomb : public Bullet
{};