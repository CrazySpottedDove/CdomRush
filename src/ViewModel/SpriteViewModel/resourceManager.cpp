#include "ViewModel/SpriteViewModel/resourceManager.h"
#include "Common/animation.h"
#include "Common/macros.h"
#include "Common/resourceLevel.h"
#include "Common/sound.h"
#include "Common/towerEssential.h"
#include "ViewModel/SpriteViewModel/readLua.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <vector>

ResourceManager::ResourceManager()
    : font(DEFAULT_FONT_FILE)
{
    const sol::table level_map = ReadLua(LEVEL_MAP_FILE);
    for (const auto& [key, value] : level_map) {
        const sol::table level_data = value.as<sol::table>();
        levels.emplace_back(level_data["name"].get<std::string>(),
                            Position{level_data["position"]["x"].get<float>(),
                                     level_data["position"]["y"].get<float>()});
    }
    for (const auto& entry : std::filesystem::directory_iterator("assets/images/common")) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();

            // 只处理 .lua 文件
            if (path.extension() == ".lua") {
                try {
                    LoadTexturesAndSpriteFrameDatas(path.string());
                }
                catch (const std::exception& e) {
                    std::cerr << "  Failed to load " << path.filename() << ": " << e.what()
                              << std::endl;
                }
            }
        }
    }

    for (const auto& entry : std::filesystem::directory_iterator("assets/sounds/common")) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();

            // 只处理 .lua 文件
            if (path.extension() == ".lua") {
                try {
                    LoadSoundGroups(path.string());
                }
                catch (const std::exception& e) {
                    std::cerr << "  Failed to load " << path.filename() << ": " << e.what()
                              << std::endl;
                }
            }
        }
    }

    LoadAnimationGroups();
}

ResourceManager::~ResourceManager()
{
    texture_map.clear();
    sprite_frame_data_map.clear();
    sound_group_map.clear();
    sound_buffer_map.clear();
    music_set.clear();
}

void ResourceManager::LoadTexture(const std::string& file_name, ResourceLevel level)
{
    if (texture_map.find(file_name) == texture_map.end()) {
        // INFO("Loading texture: " << file_name);
        const bool load_success = texture_map[file_name].loadFromFile(file_name);
        DEBUG_CODE(
            if (!load_success) { ERROR("Failed to load texture: " << file_name); } else {
                SUCCESS("Successfully loaded texture: " << file_name);
            })
        texture_level_map[level].insert(file_name);
    }
}

void ResourceManager::UnloadSpecificTexturesAndSpriteFrameDatas()
{
    for (auto it = prefix_level_map[ResourceLevel::Specific].begin();
         it != prefix_level_map[ResourceLevel::Specific].end();
         ++it) {
        sprite_frame_data_map.erase(*it);
    }
    for (auto it = texture_level_map[ResourceLevel::Specific].begin();
         it != texture_level_map[ResourceLevel::Specific].end();
         ++it) {
        texture_map.erase(*it);
    }
    prefix_level_map[ResourceLevel::Specific].clear();
    texture_level_map[ResourceLevel::Specific].clear();
}

void ResourceManager::UnloadSpecificSoundGroups()
{
    for (const auto& sound_group_name : sound_group_level_map[ResourceLevel::Specific]) {
        auto it = sound_group_map.find(sound_group_name);
        DEBUG_CODE(
            if (it == sound_group_map.end()) {
                ERROR("Try to unload not exist sound group: " << sound_group_name);
            } else {
                SUCCESS("Unloading sound group: " << sound_group_name);
            }
        )
        if(it->second.is_stream){
            for(const auto& sound_file : it->second.sound_files) {
                auto music_it = music_set.find(sound_file);
                DEBUG_CODE(
                    if (music_it == music_set.end()) {
                        ERROR("Try to unload not exist music: " << sound_file);
                    } else {
                        SUCCESS("Unloading music: " << sound_file);
                    }
                )
                music_set.erase(music_it);
            }
        }else{
            for (const auto& sound_file : it->second.sound_files) {
                auto sound_it = sound_buffer_map.find(sound_file);
                DEBUG_CODE(
                    if (sound_it == sound_buffer_map.end()) {
                        ERROR("Try to unload not exist sound: " << sound_file);
                    } else {
                        SUCCESS("Unloading sound: " << sound_file);
                    }
                )
                sound_buffer_map.erase(sound_it);
            }
        }
        sound_group_map.erase(sound_group_name);
    }
    sound_group_level_map[ResourceLevel::Specific].clear();
}

void ResourceManager::ParseSpriteFrameData(const sol::table& frame_data_unparsed,
                                           SpriteFrameData&  frame_data)
{
    frame_data.textureName        = frame_data_unparsed["a_name"].get<std::string>();
    const sol::table   f_quad     = frame_data_unparsed["f_quad"];
    const sol::table   size_table = frame_data_unparsed["size"];
    const sol::table   trim_table = frame_data_unparsed["trim"];
    const sf::Vector2i pos        = {f_quad[1].get<int>(), f_quad[2].get<int>()};
    const sf::Vector2i size       = {f_quad[3].get<int>(), f_quad[4].get<int>()};
    frame_data.frameRect          = sf::IntRect(pos, size);
    frame_data.displaySize        = {size_table[1].get<int>(), size_table[2].get<int>()};
    frame_data.trim_left          = trim_table[1].get<int>();
    frame_data.trim_top           = trim_table[2].get<int>();
    frame_data.trim_right         = trim_table[3].get<int>();
    frame_data.trim_bottom        = trim_table[4].get<int>();
}

void ResourceManager::LoadTexturesAndSpriteFrameDatas(const std::string&  file_name,
                                                      const ResourceLevel level)
{
    const sol::table& sprite_frames_table = ReadLua(file_name);
    for (const auto& pair : sprite_frames_table) {
        const std::string prefix       = pair.first.as<std::string>();
        const sol::table& frame_tables = pair.second.as<sol::table>();
        for (const auto& frame_table : frame_tables) {
            const sol::table frame_data_unparsed = frame_table.second.as<sol::table>();
            SpriteFrameData  frame_data;
            ParseSpriteFrameData(frame_data_unparsed, frame_data);
            LoadTexture(IMAGES_PATH + frame_data.textureName, level);
            sprite_frame_data_map[prefix].push_back(frame_data);
            prefix_level_map[level].insert(prefix);
        }
    }
}

void ResourceManager::LoadSoundGroups(const std::string& file_path, const ResourceLevel level)
{
    const sol::table& sound_groups_table = ReadLua(file_path);
    for (const auto& pair : sound_groups_table) {
        const std::string sound_group_name = pair.first.as<std::string>();
        const sol::table  group            = pair.second.as<sol::table>();

        sound_group_map[sound_group_name] = SoundGroup{
            group["files"].get<std::vector<std::string>>(), 0, group["gain"].get_or(1.0f) * 100.0f, group["stream"].get_or(false)};
        sound_group_level_map[level].insert(sound_group_name);
        INFO("Loading sound group: " << sound_group_name);
        for (const auto& sound_file : sound_group_map[sound_group_name].sound_files) {
            const std::string full_path = SOUNDS_PATH + sound_file;
            if (sound_group_map[sound_group_name].is_stream) {
                if (music_set.find(sound_file) == music_set.end()) {
                    music_set.insert(sound_file);
                    SUCCESS("Successfully tracked music: " << full_path);
                }
                else {
                    WARNING("Music already tracked: " << full_path);
                }
            }
            else {
                if (sound_buffer_map.find(full_path) == sound_buffer_map.end()) {
                    sound_buffer_map[sound_file] = sf::SoundBuffer(full_path);
                    SUCCESS("Successfully loaded sound: " << full_path);
                }
                else {
                    WARNING("Sound already loaded: " << full_path);
                }
            }
        }
    }
}


void ResourceManager::LoadAnimationGroups()
{
    const sol::table& animation_groups_table = ReadLua("assets/images/animation_groups.lua");

    for (const auto& pair : animation_groups_table) {
        const std::string prefix      = pair.first.as<std::string>();
        const sol::table  state_group = pair.second.as<sol::table>();

        for (const auto& state_pair : state_group) {
            INFO("Loading animation group: " << prefix << " - "
                                             << state_pair.first.as<std::string>());
            const State      state        = state_str_map.at(state_pair.first.as<std::string>());
            const sol::table frames_table = state_pair.second.as<sol::table>();
            animation_group_map[prefix][state] = AnimationGroup(
                frames_table["from"].get<std::size_t>(), frames_table["to"].get<std::size_t>());
        }
    }
};

void ResourceManager::LoadLevelAssets(const std::string& level_name, std::string& level_prepare_music, std::string& level_fight_music, double& gold)
{
    const std::string file_path         = std::string(LEVEL_DATA_PATH) + level_name + ".lua";
    const sol::table  level_data_map    = ReadLua(file_path);
    const sol::table  required_textures = level_data_map["required_textures"];
    for (const auto& [key, value] : required_textures) {
        const std::string& required_texture = IMAGES_PATH + value.as<std::string>();
        LoadTexturesAndSpriteFrameDatas(required_texture, ResourceLevel::Specific);
    }

    level_prepare_music = level_data_map["prepare_music"].get<std::string>();
    level_fight_music   = level_data_map["fight_music"].get<std::string>();
    gold                = level_data_map["gold"].get<double>();
};

void ResourceManager::LoadTowerEssentials(const std::string& file_name)
{
    const sol::table tower_positions_map = ReadLua(file_name);
    for (const auto& [key, value] : tower_positions_map) {
        const sol::table tower_position_table = value.as<sol::table>();
        const TowerType  type =
            tower_type_str_map.at(tower_position_table["type"].get<std::string>());
        Position position{tower_position_table["pos"]["x"].get<float>(),
                          tower_position_table["pos"]["y"].get<float>()};

        Position rally_point{tower_position_table["rally_point"]["x"].get<float>(),
                             tower_position_table["rally_point"]["y"].get<float>()};

        tower_essentials.emplace_back(TowerEssential{position, type, rally_point});
    }
}

void ResourceManager::LoadPaths(const std::string& file_name)
{
    const sol::table paths_table = ReadLua(file_name);
    // 遍历每个路径
    for (const auto& path_pair : paths_table) {

        const sol::table path_table = path_pair.second.as<sol::table>();

        Path current_path;

        // 遍历路径中的每个子路径
        for (const auto& subpath_pair : path_table) {
            const sol::table subpath_table = subpath_pair.second.as<sol::table>();

            SubPath current_subpath;

            // 遍历子路径中的每个路径点
            for (const auto& waypoint_pair : subpath_table) {
                const sol::table waypoint_table = waypoint_pair.second.as<sol::table>();
                // 解析Position
                Position waypoint(waypoint_table["x"].get<double>(),
                                  waypoint_table["y"].get<double>());
                // MapPosition(waypoint);
                current_subpath.push_back(waypoint);
            }

            current_path.push_back(current_subpath);
        }

        paths.push_back(current_path);
    }
}

void ResourceManager::LoadWaves(const std::string& file_name)
{
    const sol::table waves_table = ReadLua(file_name);
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
};



void ResourceManager::LoadLevelResources(const std::string& level_name,
                                         std::string&       level_prepare_music,
                                         std::string&       level_fight_music, double& gold)
{
    LoadLevelAssets(level_name, level_prepare_music, level_fight_music, gold);

    LoadSoundGroups(LEVEL_SPECIFIC_SOUNDS_PATH + level_name + ".lua",ResourceLevel::Specific);
    LoadPaths(PATH_PATH + level_name + ".lua");
    LoadWaves(WAVE_PATH + level_name + ".lua");
    LoadTowerEssentials(TOWER_PATH + level_name + ".lua");
}



