#include "waveManager.h"
#include "Manager/store/store.h"
#include "Model/enemies/enemies.h"
#include "Common/macros.h"
#include "utils/readLua.h"
#include <sol/table.hpp>
#include <string>
#include <unordered_map>

SubWave::SubWave(const double time, const std::size_t path_id, const std::size_t subpath_id,
                 const std::size_t count, const double gap, const std::string& enemy_type)
    : time(time)
    , path_id(path_id)
    , subpath_id(subpath_id)
    , count(count)
    , gap(gap)
{
    static const std::unordered_map<std::string, EnemyType> enemy_type_map = {
        {"ForestTroll", EnemyType::ForestTroll},
    {"OrcArmored", EnemyType::orc_armored}};
    auto it = enemy_type_map.find(enemy_type);
    this->enemy_type = it->second;
    DEBUG_CODE(
        if (it != enemy_type_map.end()) {
            std::cout << "Set EnemyType to " << enemy_type << std::endl;
         } else {
            std::cerr << "Error: Unknown enemy type '" << enemy_type << "'." << std::endl;
        })
}

void WaveManager::LoadWavesFromLua(const std::string& path)
{
    const sol::table waves_table = ReadLua(path);
    for (const auto& [key, value] : waves_table) {
        sol::table wave_table = value.as<sol::table>();
        Wave       wave(wave_table["duration"].get<double>(),
                  wave_table["preparation_time"].get<double>());
        sol::table sub_waves_table = wave_table["sub_waves"].get<sol::table>();
        for (const auto& [sub_key, sub_value] : sub_waves_table) {
            sol::table sub_wave_table = sub_value.as<sol::table>();
            ;
            wave.sub_waves.emplace_back(SubWave(sub_wave_table["time"].get<double>(),
                                                sub_wave_table["path_id"].get<std::size_t>(),
                                                sub_wave_table["subpath_id"].get<std::size_t>(),
                                                sub_wave_table["count"].get<std::size_t>(),
                                                sub_wave_table["gap"].get<double>(),
                                                sub_wave_table["enemy_type"].get<std::string>()));
        }
        waves.push_back(std::move(wave));
    }
}

void WaveManager::Update(Store& store)
{
    current_wave_time += FRAME_LENGTH;

    if (IsFinished()) {
        return;   // 没有更多的波次了
    }

    // 还在等待
    if (preparing) {
        if (current_wave_time >= waves[current_wave_index].preparation_time) {
            preparing         = false;
            current_wave_time = 0;
        }
        return;
    }

    // 出掉还在等待的怪
    while (pending_enemies.size() > 0 && pending_enemies.top().spawn_time <= current_wave_time) {
        const auto& pending_enemy    = pending_enemies.top();
        Enemy*      new_enemy        = store.template_manager.CreateEnemy(pending_enemy.enemy_type);
        new_enemy->path_info.path_id = pending_enemy.path_id;
        new_enemy->path_info.subpath_id  = pending_enemy.subpath_id;
        new_enemy->path_info.waypoint_id = 0;
        new_enemy->position = store.path_manager.GetPathPoint(pending_enemy.path_id,
                                                            pending_enemy.subpath_id, 0);
        store.QueueEnemy(new_enemy);
        pending_enemies.pop();
    }

    // 检查当前波次持续时间是否已经结束
    if (current_wave_time >= waves[current_wave_index].duration) {
        ++current_wave_index;
        current_wave_time     = 0;
        preparing             = true;
        current_subwave_index = 0;
        return;
    }

    // 检查当前波次的子波是否需要触发
    while (waves[current_wave_index].sub_waves.size() > current_subwave_index) {
        const auto& sub_wave = waves[current_wave_index].sub_waves[current_subwave_index];
        if (current_wave_time >= sub_wave.time) {
            // 生成子波的敌人
            for (std::size_t i = 0; i < sub_wave.count; ++i) {
                pending_enemies.emplace(PendingEnemy(sub_wave.enemy_type,
                                                     current_wave_time + i * sub_wave.gap,
                                                     sub_wave.path_id,
                                                     sub_wave.subpath_id));
            }
            ++current_subwave_index;
        }else{
            break;
        }
    }
}