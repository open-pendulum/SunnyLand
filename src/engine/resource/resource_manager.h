#pragma once

#include <glm/glm.hpp>
#include <memory>

struct SDL_Renderer;
struct SDL_Texture;
struct Mix_Chunk;
struct Mix_Music;
struct TTF_Font;

namespace engine::resource {
class TextureManager;
class AudioManager;
class FontManager;

class ResourceManager final {
 public:
  explicit ResourceManager(SDL_Renderer* renderer);
  ~ResourceManager();
  void Clear();

  ResourceManager(const ResourceManager& other) = delete;
  ResourceManager& operator=(const ResourceManager& other) = delete;
  ResourceManager(ResourceManager&& other) = delete;
  ResourceManager& operator=(ResourceManager&& other) = delete;

  SDL_Texture* LoadTexture(const std::string& name) const;
  SDL_Texture* GetTexture(const std::string& name) const;
  void UnloadTexture(const std::string& name);
  glm::vec2 GetTextureSize(const std::string& name) const;
  void ClearTextures() const;

  Mix_Chunk* LoadSound(const std::string& name) const;
  Mix_Chunk* GetSound(const std::string& name) const;
  void UnloadSound(const std::string& name);
  void ClearSounds() const;

  Mix_Music* LoadMusic(const std::string& name) const;
  Mix_Music* GetMusic(const std::string& name) const;
  void UnloadMusic(const std::string& name);
  void ClearMusic() const;

  TTF_Font* LoadTTF(const std::string& name) const;
  TTF_Font* GetTTF(const std::string& name) const;
  void UnloadTTF(const std::string& name);
  void ClearTTFs() const;

 private:
  std::unique_ptr<TextureManager> texture_manager_{nullptr};
  std::unique_ptr<AudioManager> audio_manager_{nullptr};
  std::unique_ptr<FontManager> font_manager_{nullptr};
};
}  // namespace engine::resource
