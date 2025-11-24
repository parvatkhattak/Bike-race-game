#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "raylib.h"
#include <string>
#include <map>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void Initialize();
    void Update(float deltaTime);

    // Music
    void PlayMusic(const std::string& musicName);
    void StopMusic();
    void SetMusicVolume(float volume);

    // Sound effects
    void PlaySound(const std::string& soundName);
    void SetSoundVolume(float volume);

private:
    void LoadAudio();
    void LoadMusic(const std::string& name, const std::string& filepath);
    void LoadSound(const std::string& name, const std::string& filepath);

    std::map<std::string, Music> musicTracks;
    std::map<std::string, Sound> soundEffects;

    Music currentMusic;
    bool musicPlaying;
    float masterVolume;
    float musicVolume;
    float soundVolume;
};

#endif // AUDIOMANAGER_H
