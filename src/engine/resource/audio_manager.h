#pragma once

namespace engine::resource {



class AudioManager {
 public:
  AudioManager();

  AudioManager(const AudioManager& other) = delete;
  AudioManager& operator=(const AudioManager& other) = delete;
  AudioManager(AudioManager&& other) = delete;
  AudioManager& operator=(AudioManager&& other) = delete;

  ~AudioManager();
};
}  // namespace engine::resource
