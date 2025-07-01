#include "waveManager.h"
#include "Model/enemies/enemies.h"
#include "utils/readLua.h"
#include <sol/table.hpp>
#include <string>
#include <unordered_map>

SubWave::SubWave(const double time, const std::size_t path_id, const std::size_t subpath_id,
               const std::size_t count, const double gap, const std::string& enemy_type)
    : time(time), path_id(path_id), subpath_id(subpath_id), count(count), gap(gap), {
        static constexpr std::unordered_map<std::string, EnemyType> enemy_type_map = {
            {"ForestTroll",EnemyType::ForestTroll}
        };
    }

void WaveManager::LoadWavesFromLua(const std::string& path){
    const sol::table waves_table = ReadLua(path);
    for(const auto&[key, value]: waves_table){
        sol::table wave_table = value.as<sol::table>();
        Wave wave(
            wave_table["duration"].get<double>(),
            wave_table["preparation_time"].get<double>()
        );
        sol::table sub_waves_table = wave_table["sub_waves"].get<sol::table>();
        for(const auto&[sub_key, sub_value]: sub_waves_table){
            sol::table sub_wave_table = sub_value.as<sol::table>();
            SubWave sub_wave(
                sub_wave_table["enemy_type"].get<EnemyType>(),
                sub_wave_table["count"].get<int>(),
                sub_wave_table["interval"].get<double>()
            );
            wave.sub_waves.push_back(sub_wave);
        }
    }
}