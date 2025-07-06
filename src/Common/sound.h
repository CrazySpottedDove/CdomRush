#pragma once

#include <SFML/Audio/Music.hpp>
#include <cstddef>
#include <queue>
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

// 音乐文件名，需要使用时根据文件名流式加载得到 Music
typedef std::unordered_set<std::string> MusicSet;

// ResourceLevel -> 声音组名列表
typedef std::unordered_map<ResourceLevel, std::unordered_set<std::string>> SoundGroupLevelMap;

enum class SoundGroupMode{
    Sequence, // 按照索引播放
    Random // 随机播放
};

struct SoundData{
    std::string group_name;
    SoundGroupMode mode;
    float gain = 1.0f; // 音量倍率
    // size_t sound_id = 0; // 播放的音乐在声音组中的索引
    SoundData(const std::string& group_name, const float gain = 1.0f,
              const SoundGroupMode mode = SoundGroupMode::Sequence)
        : group_name(group_name), gain(gain), mode(mode) {}
};

// SoundData 的队列
typedef std::queue<SoundData> SoundDataQueue;
