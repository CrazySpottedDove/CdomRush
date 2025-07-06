#include "View/soundManager.h"
#include "Common/macros.h"
#include "Common/random.h"
#include "Common/sound.h"
#include <SFML/Audio/Sound.hpp>

SoundManager::SoundManager()
{
    sound_pool.reserve(16);
}

void SoundManager::SetSoundBufferMap(SoundBufferMap* map)
{
    sound_buffer_map = map;
}

void SoundManager::SetSoundGroupMap(SoundGroupMap* map)
{
    sound_group_map = map;
}

void SoundManager::SetMusicSet(MusicSet* set)
{
    music_set = set;
}

void SoundManager::SetSoundDataQueue(SoundDataQueue* queue)
{
    sound_data_queue = queue;
}

void SoundManager::Play(const SoundData& sound_data)
{
    auto it = sound_group_map->find(sound_data.group_name);
    DEBUG_CODE(if (it == sound_group_map->end()) {
        WARNING("Sound group not found: " << sound_data.group_name);
        return;
    })
    auto& sound_group = it->second;
    size_t sound_id;
    switch (sound_data.mode) {
    case SoundGroupMode::Random:
    {
        sound_id = randInt(0, sound_group.sound_files.size() - 1);
        break;
    }
    case SoundGroupMode::Sequence:
    {
        sound_id = sound_group.next_sound_id;
        ++sound_group.next_sound_id;   // 更新下一个要播放的声音索引
        if (sound_group.next_sound_id >= sound_group.sound_files.size()) {
            sound_group.next_sound_id = 0;   // 重置索引
        }
        break;
    }
    }
    const std::string& sound_file = sound_group.sound_files[sound_id];

    if (sound_group.is_stream) {
        auto music_it = music_set->find(sound_file);
        DEBUG_CODE(if (music_it == music_set->end()) {
            WARNING("Music not found: " << sound_data.group_name);
            return;
        })
        // 如果音乐已经在播放，停止当前音乐
        if (current_music.getStatus() == sf::Music::Status::Playing) {
            current_music.stop();
        }
        const bool open_success = current_music.openFromFile(SOUNDS_PATH + *music_it);
        DEBUG_CODE(if (!open_success) {
            WARNING("Failed to open music file: " << SOUNDS_PATH + *music_it);
            return;
        })
        current_music.setVolume(sound_group.gain);
        current_music.play();
    }
    else {
        auto buffer_it = sound_buffer_map->find(sound_file);

        if (buffer_it == sound_buffer_map->end()) {
            WARNING("Sound buffer not found: " << sound_data.group_name);
            return;
        }
        const auto& sound_buffer = buffer_it->second;
        if (sound_pool.size() == sound_pool.capacity()) {
            INFO("Sound pool is full, skip sound: "
                 << sound_group.sound_files[sound_group.next_sound_id]);
            return;
        }
        sf::Sound* sound = new sf::Sound(sound_buffer);
        sound->setVolume(sound_group.gain);
        sound->play();
        sound_pool.push_back(sound);
    }
}

void SoundManager::ClearPlayedSounds()
{
    for (auto it = sound_pool.begin(); it != sound_pool.end();) {
        if ((*it)->getStatus() == sf::Sound::Status::Stopped) {
            delete *it;  // 删除已停止的声音
            it = sound_pool.erase(it);  // 从池中移除
        }
        else {
            ++it;  // 继续检查下一个声音
        }
    }
}

void SoundManager::PlayAll()
{
    DEBUG_CODE(if (sound_data_queue == nullptr) {
        WARNING("SoundDataQueue is not set, cannot play sounds.");
        return;
    })
    while (!sound_data_queue->empty()) {
        Play(sound_data_queue->front());
        sound_data_queue->pop();
    }
    ClearPlayedSounds();
}