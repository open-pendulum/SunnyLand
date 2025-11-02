#include "audio_manager.h"
#include "logger.hpp"

namespace engine::resource {
namespace {
DECLARE_TAG(AudioManager);
}
AudioManager::AudioManager() {
  TRACEI(TAG);
  MIX_InitFlags flags = MIX_INIT_OGG | MIX_INIT_MP3;
  if ((Mix_Init(flags) & flags) != flags) {
    LOGE(TAG, "Failed to initialize SDL_mixer with flags: %d", flags);
    throw std::runtime_error("Failed to initialize SDL_mixer with flags: " +
                             std::to_string(flags));
  }

  if (!Mix_OpenAudio(0, nullptr)) {
    LOGE(TAG, "Failed to open audio device");
    throw std::runtime_error("Failed to open audio device");
  }
}

AudioManager::~AudioManager() {
  TRACEI(TAG);
  Mix_HaltChannel(-1);
  Mix_HaltMusic();

  ClearSounds();
  ClearMusic();

  Mix_CloseAudio();
  Mix_Quit();
}

Mix_Chunk* AudioManager::LoadSound(const std::string& file_path) {
  if (sounds_.contains(file_path)) {
    return sounds_.at(file_path).get();
  }

  Mix_Chunk* raw_chunk = Mix_LoadWAV(file_path.c_str());
  if (raw_chunk == nullptr) {
    LOGE(TAG, "Failed to load sound: {}", file_path);
    return nullptr;
  }
  sounds_.emplace(file_path,
                  std::unique_ptr<Mix_Chunk, SDLMixChunkDeleter>(raw_chunk));
  LOGI(TAG, "Loaded sound: {}", file_path);
  return raw_chunk;
}
Mix_Chunk* AudioManager::GetSound(const std::string& file_path) {
  if (sounds_.contains(file_path)) {
    return sounds_.at(file_path).get();
  }
  LOGW(TAG, "Sound not found: {}, try to load it", file_path);
  return LoadSound(file_path);
}
void AudioManager::UnloadSound(const std::string& file_path) {
  if (sounds_.contains(file_path)) {
    sounds_.erase(file_path);
    LOGI(TAG, "Unloaded sound: {}", file_path);
  } else {
    LOGW(TAG, "Sound not found: {}, cannot unload", file_path);
  }
}
void AudioManager::ClearSounds() {
  TRACEI(TAG);
  sounds_.clear();
  LOGI(TAG, "Cleared all sounds");
}
Mix_Music* AudioManager::LoadMusic(const std::string& file_path) {
  TRACEI(TAG);
  if (musics_.contains(file_path)) {
    return musics_.at(file_path).get();
  }

  Mix_Music* raw_music = Mix_LoadMUS(file_path.c_str());
  if (raw_music == nullptr) {
    LOGE(TAG, "Failed to load music: {}", file_path);
    return nullptr;
  }
  musics_.emplace(file_path,
                  std::unique_ptr<Mix_Music, SDLMixMusicDeleter>(raw_music));
  LOGI(TAG, "Loaded music: {}", file_path);
  return raw_music;
}
Mix_Music* AudioManager::GetMusic(const std::string& file_path) {
  if (musics_.contains(file_path)) {
    return musics_.at(file_path).get();
  }
  LOGW(TAG, "Music not found: {}, try to load it", file_path);
  return LoadMusic(file_path);
}
void AudioManager::UnloadMusic(const std::string& file_path) {
  if (musics_.contains(file_path)) {
    musics_.erase(file_path);
    LOGI(TAG, "Unloaded music: {}", file_path);
  } else {
    LOGW(TAG, "Music not found: {}, cannot unload", file_path);
  }
}
void AudioManager::ClearMusic() {
  TRACEI(TAG);
  musics_.clear();
  LOGI(TAG, "Cleared all music");
}
void AudioManager::ClearAudio() {
  TRACEI(TAG);
  ClearSounds();
  ClearMusic();
}
}  // namespace engine::resource
