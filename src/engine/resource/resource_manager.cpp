#include "resource_manager.h"
#include "audio_manager.h"
#include "font_manager.h"
#include "logger.hpp"
#include "texture_manager.h"

namespace engine::resource {
namespace {
DECLARE_TAG(ResourceManager)
}  // namespace

ResourceManager::ResourceManager(SDL_Renderer* renderer)
    : texture_manager_(std::make_unique<TextureManager>(renderer)),
      audio_manager_(std::make_unique<AudioManager>()),
      font_manager_(std::make_unique<FontManager>()) {
  TRACEI(TAG);
}
ResourceManager::~ResourceManager() {
  TRACEI(TAG);
}
void ResourceManager::Clear() {
  font_manager_->ClearFonts();
  audio_manager_->ClearAudio();
  texture_manager_->ClearTextures();
}
SDL_Texture* ResourceManager::LoadTexture(const std::string& name) const {
  return texture_manager_->LoadTexture(name);
}
SDL_Texture* ResourceManager::GetTexture(const std::string& name) const {
  return texture_manager_->GetTexture(name);
}
void ResourceManager::UnloadTexture(const std::string& name) {
  texture_manager_->UnloadTexture(name);
}
glm::vec2 ResourceManager::GetTextureSize(const std::string& name) const {
  return texture_manager_->GetTextureSize(name);
}
void ResourceManager::ClearTextures() const {
  texture_manager_->ClearTextures();
}
Mix_Chunk* ResourceManager::LoadSound(const std::string& name) const {
  return audio_manager_->LoadSound(name);
}
Mix_Chunk* ResourceManager::GetSound(const std::string& name) const {
  return audio_manager_->GetSound(name);
}
void ResourceManager::UnloadSound(const std::string& name) {
  audio_manager_->UnloadSound(name);
}
void ResourceManager::ClearSounds() const {
  audio_manager_->ClearSounds();
}
Mix_Music* ResourceManager::LoadMusic(const std::string& name) const {
  return audio_manager_->LoadMusic(name);
}
Mix_Music* ResourceManager::GetMusic(const std::string& name) const {
  return audio_manager_->GetMusic(name);
}
void ResourceManager::UnloadMusic(const std::string& name) {
  audio_manager_->UnloadMusic(name);
}
void ResourceManager::ClearMusic() const {
  audio_manager_->ClearMusic();
}
TTF_Font* ResourceManager::LoadFont(const std::string& name, int32_t point_size) const {
  return font_manager_->LoadFont(name, point_size);
}
TTF_Font* ResourceManager::GetFont(const std::string& name, int32_t point_size) const {
  return font_manager_->GetFont(name, point_size);
}
void ResourceManager::UnloadFont(const std::string& name, int32_t point_size) {
  font_manager_->UnloadFont(name, point_size);
}
void ResourceManager::ClearFonts() const {
  font_manager_->ClearFonts();
}
}  // namespace engine::resource
