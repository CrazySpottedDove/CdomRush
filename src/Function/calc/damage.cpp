#include "Function/calc/damage.h"
#include "Function/calc/buff.h"
#include "Function/calc/hp.h"
#include "Manager/store/store.h"
#include "Model/components/buff.h"
#include "Model/components/damage.h"
#include "Model/enemies/enemies.h"
#include "Model/templates/unit.h"
#include <SFML/System/Vector2.hpp>
#include <vector>

void calc::enforce_damage(const DamageEvent& damage_event)
{
    const auto& data       = damage_event.data;
    auto&       target     = *damage_event.target;
    double      value      = data.value;
    double      protection = 0.0;
    switch (data.type) {
    case DamageType::True: break;
    case DamageType::Physical:
        protection = target.armor.physical + target.buff.physical_armor_inc - data.ignore_armor;
        break;
    case DamageType::Magical:
        protection = target.armor.magical + target.buff.magical_armor_inc - data.ignore_armor;
        break;
    case DamageType::Explosion:
        protection =
            (target.armor.physical + target.buff.physical_armor_inc - data.ignore_armor) * 0.5;
        break;
    case DamageType::Poison: protection = target.armor.poison - data.ignore_armor; break;
    case DamageType::Fire: protection = target.armor.fire - data.ignore_armor; break;
    case DamageType::Electrical:
        protection =
            (target.armor.physical + target.buff.physical_armor_inc - data.ignore_armor) * 0.5 +
            target.armor.electrical;
        break;
    case DamageType::Tear:
        if (target.armor.physical + target.buff.physical_armor_inc <= 0.0) {
            value *= 2.0;
        }
        break;
    case DamageType::Blood: protection = target.armor.blood - data.ignore_armor; break;
    case DamageType::Physical_Armor: calc::physical_armor_dec(target, data.value); break;
    case DamageType::Magical_Armor: calc::magical_armor_dec(target, data.value); break;
    }

    if (protection > 1.0) {
        protection = 1.0;
    }

    if (damage_event.source != nullptr) {
        calc::update_damage_value(damage_event.source->buff, value);
    }

    target.health.hp -= value * (1.0 - protection);
}

void calc::update_damage_value(const Buff& buff, double& value)
{
    value = (value + buff.damage_inc) * buff.damage_factor;
}

Enemy* calc::find_foremost_enemy(const Store& store, const Position& position, const double range,
                                 const bool require_active)
{
    const std::vector<Enemy*>& enemies = store.GetEnemies();
    if (enemies.empty()) {
        return nullptr;
    }

    Enemy* foremost_enemy      = nullptr;
    int    min_points_remained = std::numeric_limits<int>::max();
    for (const auto& enemy : enemies) {
        // 死亡的敌人，不索敌
        if (calc::is_dead(*enemy)) {
            continue;
        }

        // 被动的敌人，不索敌
        if (require_active && enemy->type == EnemyType::PassiveEnemy) {
            continue;
        }

        // 超出范围的敌人，不索敌
        const sf::Vector2f displacement = enemy->position - position;
        if (displacement.lengthSquared() > range * range) {
            continue;
        }

        const int points_remained = store.path_manager.GetPointsRemained(enemy->path_info);
        if (points_remained < min_points_remained) {
            min_points_remained = points_remained;
            foremost_enemy      = enemy;
        }
    }
    return foremost_enemy;
}

Soldier* find_nearest_soldier(const Store& store, const Position& position, const double range)
{
    const std::vector<Soldier*>& soldiers = store.GetSoldiers();
    if (soldiers.empty()) {
        return nullptr;
    }

    Soldier* nearest_soldier      = nullptr;
    double   min_distance_squared = std::numeric_limits<double>::max();
    for (const auto& soldier : soldiers) {
        // 超出范围的士兵，不索敌
        const sf::Vector2f displacement     = soldier->position - position;
        const double       distance_squared = displacement.lengthSquared();
        if (distance_squared > range * range) {
            continue;
        }

        if (distance_squared < min_distance_squared) {
            min_distance_squared = distance_squared;
            nearest_soldier      = soldier;
        }
    }
    return nearest_soldier;
}

std::vector<Enemy*> calc::find_enemies_in_range(const Store& store, const Position& position,
                                                const double range)
{
    std::vector<Enemy*> enemies_in_range;
    const std::vector<Enemy*>& enemies = store.GetEnemies();
    for (const auto& enemy : enemies) {
        // 死亡的敌人，不索敌
        if (calc::is_dead(*enemy)) {
            continue;
        }

        // 超出范围的敌人，不索敌
        const sf::Vector2f displacement = enemy->position - position;
        if (displacement.lengthSquared() > range * range) {
            continue;
        }

        enemies_in_range.push_back(enemy);
    }
    return enemies_in_range;
}

std::vector<Soldier*> calc::find_soldiers_in_range(const Store& store, const Position& position,
                                                    const double range)
{
    std::vector<Soldier*> soldiers_in_range;
    const std::vector<Soldier*>& soldiers = store.GetSoldiers();
    for (const auto& soldier : soldiers) {
        // 超出范围的士兵，不索敌
        const sf::Vector2f displacement     = soldier->position - position;
        const double       distance_squared = displacement.lengthSquared();
        if (distance_squared > range * range) {
            continue;
        }

        soldiers_in_range.push_back(soldier);
    }
    return soldiers_in_range;
}