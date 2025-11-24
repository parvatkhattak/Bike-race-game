#include "systems/AudioManager.h"
#include "utils/Logger.h"
#include "utils/Config.h"

AudioManager::AudioManager() :
    musicPlaying(false),
    masterVolume(1.0f),
    musicVolume(0.7f),
    soundVolume(0.8f)
{
}

AudioManager::~AudioManager() {
    // Unload all music and sounds
    for (auto& [name, music] : musicTracks) {
        UnloadMusicStream(music);
    }
    
    for (auto& [name, sound] : soundEffects) {
        UnloadSound(sound);
    }
}

void AudioManager::Initialize() {
    LOG_INFO("AudioManager initialized");
    LoadAudio();
}

void AudioManager::LoadAudio() {
    // Note: These files don't exist yet, so we'll create placeholders
    // In a real project, you'd load actual audio files
    
    // For now, we'll just log that we tried to load them
    LOG_INFO("Audio loading skipped (no audio files yet)");
    
    // Example of how to load when files exist:
    // LoadMusic("menu", Config::AUDIO_PATH + "music/menu.ogg");
    // LoadMusic("race", Config::AUDIO_PATH + "music/race.ogg");
    // LoadSound("engine", Config::AUDIO_PATH + "sfx/engine.wav");
    // LoadSound("collision", Config::AUDIO_PATH + "sfx/collision.wav");
}

void AudioManager::LoadMusic(const std::string& name, const std::string& filepath) {
    Music music = LoadMusicStream(filepath.c_str());
    if (music.frameCount > 0) {
        musicTracks[name] = music;
        LOG_INFO("Loaded music: " + name);
    } else {
        LOG_WARNING("Failed to load music: " + filepath);
    }
}

void AudioManager::LoadSound(const std::string& name, const std::string& filepath) {
    Sound sound = LoadSound(filepath.c_str());
    if (sound.frameCount > 0) {
        soundEffects[name] = sound;
        LOG_INFO("Loaded sound: " + name);
    } else {
        LOG_WARNING("Failed to load sound: " + filepath);
    }
}

void AudioManager::Update(float deltaTime) {
    // Update music streaming
    if (musicPlaying) {
        UpdateMusicStream(currentMusic);
    }
}

void AudioManager::PlayMusic(const std::string& musicName) {
    // Stop current music if playing
    if (musicPlaying) {
        StopMusicStream(currentMusic);
    }
    
    // Find and play new music
    auto it = musicTracks.find(musicName);
    if (it != musicTracks.end()) {
        currentMusic = it->second;
        SetMusicVolume(currentMusic, musicVolume * masterVolume);
        PlayMusicStream(currentMusic);
        musicPlaying = true;
        LOG_INFO("Playing music: " + musicName);
    } else {
        LOG_WARNING("Music not found: " + musicName);
    }
}

void AudioManager::StopMusic() {
    if (musicPlaying) {
        StopMusicStream(currentMusic);
        musicPlaying = false;
    }
}

void AudioManager::SetMusicVolume(float volume) {
    musicVolume = volume;
    if (musicPlaying) {
        SetMusicVolume(currentMusic, musicVolume * masterVolume);
    }
}

void AudioManager::PlaySound(const std::string& soundName) {
    auto it = soundEffects.find(soundName);
    if (it != soundEffects.end()) {
        SetSoundVolume(it->second, soundVolume * masterVolume);
        PlaySound(it->second);
    } else {
        LOG_WARNING("Sound not found: " + soundName);
    }
}

void AudioManager::SetSoundVolume(float volume) {
    soundVolume = volume;
}
