#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace engine::resource {

class AudioManager final {
 public:
  AudioManager();

  AudioManager(const AudioManager& other) = delete;
  AudioManager& operator=(const AudioManager& other) = delete;
  AudioManager(AudioManager&& other) = delete;
  AudioManager& operator=(AudioManager&& other) = delete;

  ~AudioManager();

 public:
  Mix_Chunk* LoadSound(const std::string& file_path);
  Mix_Chunk* GetSound(const std::string& file_path);
  void UnloadSound(const std::string& file_path);
  void ClearSounds();

  Mix_Music* LoadMusic(const std::string& file_path);
  Mix_Music* GetMusic(const std::string& file_path);
  void UnloadMusic(const std::string& file_path);
  void ClearMusic();

  void ClearAudio();

 private:
  struct SDLMixChunkDeleter {
    void operator()(Mix_Chunk* chunk) const {
      if (chunk) {
        Mix_FreeChunk(chunk);
      }
    }
  };
  std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, SDLMixChunkDeleter>> sounds_;

  struct SDLMixMusicDeleter {
    void operator()(Mix_Music* music) const {
      if (music) {
        Mix_FreeMusic(music);
      }
    }
  };
  std::unordered_map<std::string, std::unique_ptr<Mix_Music, SDLMixMusicDeleter>> musics_;
};
}  // namespace engine::resource
