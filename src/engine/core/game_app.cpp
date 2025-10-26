#include "game_app.h"
#include "logger.hpp"
#include "time.h"

#include <SDL3/SDL.h>

namespace engine::core {
namespace {
DECLARE_TAG(GameApp)
}  // namespace
GameApp::GameApp() : time_(std::make_unique<Time>()) {
  TRACEI(TAG);
}
GameApp::~GameApp() {
  TRACEI(TAG);
  if (is_running_) {
    Close();
  }
}
void GameApp::Run() {
  TRACEI(TAG);
  if (!Init()) {
    LOGE(TAG, "Failed to initialize!");
    return;
  }
  LOGI(TAG, "Running...");
  time_->SetTargetFPS(144);
  while (is_running_) {
    time_->Update();
    HandleEvents();
    Update(time_->GetDeltaTimeS());
    Render();
  }
}
bool GameApp::Init() {
  TRACEI(TAG);
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    LOGE(TAG, "Failed to initialize! SDL Error: {}", SDL_GetError());
    return false;
  }

  window_ = SDL_CreateWindow("SunnyLand", 1280, 720, SDL_WINDOW_RESIZABLE);
  if (!window_) {
    LOGE(TAG, "Failed to create window! SDL Error: {}", SDL_GetError());
    return false;
  }

  renderer_ = SDL_CreateRenderer(window_, nullptr);
  if (!renderer_) {
    LOGE(TAG, "Failed to create renderer! SDL Error: %s", SDL_GetError());
    return false;
  }
  is_running_ = true;
  return true;
}

void GameApp::Update(double delta_time_s) {
}

void GameApp::Render() {
  LOGI(TAG, "Updating...");
}
void GameApp::HandleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      is_running_ = false;
    }
  }
}
void GameApp::Close() {
  TRACEI(TAG);
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }

  if (window_) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
  SDL_Quit();
  is_running_ = false;
}
}  // namespace engine::core
