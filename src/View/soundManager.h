#pragma once

#include "Common/sound.h"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <vector>

typedef std::vector<sf::Sound*> SoundPool;

class SoundManager{
public:
    SoundManager();
    void SetSoundBufferMap(SoundBufferMap* map);
    void SetSoundGroupMap(SoundGroupMap* map);
    void SetMusicSet(MusicSet* set);
    void SetSoundDataQueue(SoundDataQueue* queue);
    void PlayAll();
private:
    SoundBufferMap* sound_buffer_map;
    SoundGroupMap* sound_group_map;
    MusicSet* music_set;
    sf::Music current_music;
    SoundPool sound_pool;
    SoundDataQueue* sound_data_queue;
    void Play(const SoundData& sound_data);
    void ClearPlayedSounds();
};