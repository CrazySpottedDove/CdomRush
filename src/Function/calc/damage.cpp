#include "Function/calc/damage.h"
#include "Function/calc/buff.h"
#include "Function/calc/hp.h"
#include "Manager/store/store.h"
#include "Model/components/buff.h"
#include "Model/components/damage.h"
#include "Model/enemies/enemies.h"
#include "Model/soldiers/soldiers.h"
#include "Model/templates/unit.h"
#include "utils/macros.h"
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <vector>

void calc::enforce_damage(Store& store, const DamageEvent& damage_event)
{
    const auto& data       = damage_event.data;
    Unit*       target     = store.GetEnemy(damage_event.target);
    Unit* source = store.GetSoldier(damage_event.source);
    if (target == nullptr){
        target = store.GetSoldier(damage_event.target);
        source = store.GetEnemy(damage_event.source);
    }
    if (target == nullptr) {
        return; // 目标不存在
    }
    double      value      = data.value;
    double      protection = 0.0;
    switch (data.type) {
    case DamageType::True: break;
    case DamageType::Physical:
        protection = target->armor.physical + target->buff.physical_armor_inc - data.ignore_armor;
        break;
    case DamageType::Magical:
        protection = target->armor.magical + target->buff.magical_armor_inc - data.ignore_armor;
        break;
    case DamageType::Explosion:
        protection =
            (target->armor.physical + target->buff.physical_armor_inc - data.ignore_armor) * 0.5;
        break;
    case DamageType::Poison: protection = target->armor.poison - data.ignore_armor; break;
    case DamageType::Fire: protection = target->armor.fire - data.ignore_armor; break;
    case DamageType::Electrical:
        protection =
            (target->armor.physical + target->buff.physical_armor_inc - data.ignore_armor) * 0.5 +
            target->armor.electrical;
        break;
    case DamageType::Tear:
        if (target->armor.physical + target->buff.physical_armor_inc <= 0.0) {
            value *= 2.0;
        }
        break;
    case DamageType::Blood: protection = target->armor.blood - data.ignore_armor; break;
    case DamageType::Physical_Armor: calc::physical_armor_dec(*target, data.value); break;
    case DamageType::Magical_Armor: calc::magical_armor_dec(*target, data.value); break;
    }

    if (protection > 1.0) {
        protection = 1.0;
    }

    if (source != nullptr) {
        calc::update_damage_value(source->buff, value);
    }

    target->health.hp -= value * (1.0 - protection);
}

void calc::update_damage_value(const Buff& buff, double& value)
{
    value = (value + buff.damage_inc) * buff.damage_factor;
}

ID calc::find_foremost_enemy(const Store& store, const Position& position, const double range,
                                 const bool require_active)
{
    const std::unordered_map<ID, Enemy*>& enemies = store.GetEnemies();
    if (enemies.empty()) {
        return INVALID_ID;
    }

    int    min_points_remained = std::numeric_limits<int>::max();
    ID   foremost_enemy_id      = INVALID_ID;
    for (const auto& [enemy_id, enemy] : enemies) {
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
            foremost_enemy_id = enemy_id;
        }
    }
    return foremost_enemy_id;
}

ID find_nearest_soldier(const Store& store, const Position& position, const double range)
{
    const std::unordered_map<ID, Soldier*>& soldiers = store.GetSoldiers();
    if (soldiers.empty()) {
        return INVALID_ID;
    }

    ID nearest_soldier_id      = INVALID_ID;
    double   min_distance_squared = std::numeric_limits<double>::max();
    for (const auto& [soldier_id, soldier] : soldiers) {
        // 超出范围的士兵，不索敌
        const sf::Vector2f displacement     = soldier->position - position;
        const double       distance_squared = displacement.lengthSquared();
        if (distance_squared > range * range) {
            continue;
        }

        if (distance_squared < min_distance_squared) {
            min_distance_squared = distance_squared;
            nearest_soldier_id = soldier_id;
        }
    }
    return nearest_soldier_id;
}

std::vector<ID> calc::find_enemies_in_range(const Store& store, const Position& position,
                                                const double range)
{
    std::vector<ID> enemy_ids_in_range;
    const std::unordered_map<ID, Enemy*>& enemies = store.GetEnemies();
    for (const auto& [enemy_id, enemy] : enemies) {
        // 死亡的敌人，不索敌
        if (calc::is_dead(*enemy)) {
            continue;
        }

        // 超出范围的敌人，不索敌
        const sf::Vector2f displacement = enemy->position - position;
        if (displacement.lengthSquared() > range * range) {
            continue;
        }

        enemy_ids_in_range.push_back(enemy_id);
    }
    return enemy_ids_in_range;
}

std::vector<ID> calc::find_soldiers_in_range(const Store& store, const Position& position,
                                                    const double range)
{
    std::vector<ID> soldier_ids_in_range;
    const std::unordered_map<ID, Soldier*>& soldiers = store.GetSoldiers();
    for (const auto& [soldier_id, soldier] : soldiers) {
        // 超出范围的士兵，不索敌
        const sf::Vector2f displacement     = soldier->position - position;
        const double       distance_squared = displacement.lengthSquared();
        if (distance_squared > range * range) {
            continue;
        }

        soldier_ids_in_range.push_back(soldier_id);
    }
    return soldier_ids_in_range;
}