#pragma once
#include "../utils/attacks.h"
#include "../utils/damage.h"
#include "../utils/vector2.h"
#include <cstddef>
#include <memory>

class Bullet
{
public:
    std::size_t sprite_id;
    std::size_t target_id;

    Vector2                         position;
    Vector2                         velocity;
    std::shared_ptr<Damage>         damage;
    virtual void                    Insert(Store& store) noexcept = 0;
    virtual void                    Update(Store& store) noexcept = 0;
    virtual void                    OnHit(Store& store) noexcept  = 0;
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