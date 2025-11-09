#include "engine/core/game_app.h"

#include "logger.hpp"
int main(int, char**) {
  spdlog::set_level(spdlog::level::trace);
  engine::core::GameApp game;
  game.Run();
}
