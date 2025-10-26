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
}
SDL_Texture* ResourceManager::LoadTexture(const std::string& name) const {
}
SDL_Texture* ResourceManager::GetTexture(const std::string& name) const {
}
void ResourceManager::UnloadTexture(const std::string& name) {
}
glm::vec2 ResourceManager::GetTextureSize(const std::string& name) const {
}
void ResourceManager::ClearTextures() const {
}
Mix_Chunk* ResourceManager::LoadSound(const std::string& name) const {
}
Mix_Chunk* ResourceManager::GetSound(const std::string& name) const {
}
void ResourceManager::UnloadSound(const std::string& name) {
}
void ResourceManager::ClearSounds() const {
}
Mix_Music* ResourceManager::LoadMusic(const std::string& name) const {
}
Mix_Music* ResourceManager::GetMusic(const std::string& name) const {
}
void ResourceManager::UnloadMusic(const std::string& name) {
}
void ResourceManager::ClearMusic() const {
}
TTF_Font* ResourceManager::LoadTTF(const std::string& name) const {
}
TTF_Font* ResourceManager::GetTTF(const std::string& name) const {
}
void ResourceManager::UnloadTTF(const std::string& name) {
}
void ResourceManager::ClearTTFs() const {
}
}  // namespace engine::resource
