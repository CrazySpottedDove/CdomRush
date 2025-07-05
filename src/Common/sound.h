#pragma once

#include <SFML/Audio/Music.hpp>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <unordered_set>
#include <vector>
#include "Common/resourceLevel.h"

// 声音组中有多个声音文件，依据一定的规则进行播放。is_stream 表示是否为流式音乐。如果为 true，则在 SoundBufferMap 中查找资源；如果为 false，则在 MusicMap 中查找资源。
struct SoundGroup
{
    std::vector<std::string> sound_files; // 同属于声音组的声音文件
    size_t                   next_sound_id = 0;   // 下次播放的音乐在声音组中的索引
    bool is_stream = false;
};

// 声音组名->声音组
typedef std::unordered_map<std::string, SoundGroup> SoundGroupMap;

// 声音文件名->SoundBuffer
typedef std::unordered_map<std::string, sf::SoundBuffer> SoundBufferMap;

// 声音文件名->Music
typedef std::unordered_map<std::string, sf::Music> MusicMap;

// ResourceLevel -> 声音组名列表
typedef std::unordered_map<ResourceLevel, std::unordered_set<std::string>> SoundGroupLevelMap;

enum class SoundGroupMode{
    Sequence, // 按照索引播放
    Random // 随机播放
};

struct SoundData{
    std::string* group_name;
    SoundGroupMode mode;
    float gain = 1.0f; // 音量倍率
    size_t sound_id = 0; // 播放的音乐在声音组中的索引
};
