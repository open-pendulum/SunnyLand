#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace engine::core {

class Config final {
 public:
  explicit Config(const std::string& file_name);

  Config(const Config&) = delete;
  Config& operator=(const Config&) = delete;
  Config(Config&&) = delete;
  Config& operator=(Config&&) = delete;

  bool LoadConfig(const std::string& file_name);
  bool SaveConfig(const std::string& file_name);

  const std::string& WindowTitle() const;
  const int32_t& WindowWidth() const;
  const int32_t& WindowHeight() const;
  const bool& WindowResizable() const;
  const bool& VSync() const;
  const int32_t& TargetFps() const;
  const float& MusicVolume() const;
  const float& SoundVolume() const;
  const std::unordered_map<std::string, std::vector<std::string>>& InputMappings() const;

 private:
  void FromJson(const nlohmann::json& json);
  nlohmann::json ToJson() const;

 private:
  std::string window_title_{"SunnyLand"};
  int32_t window_width_{1280};
  int32_t window_height_{720};
  bool window_resizable_{true};

  bool is_fullscreen_{false};
  float framerate_limit_{60.0f};
  bool vsync_{true};

  int32_t target_fps_{144};

  float music_volume_{0.5f};
  float sound_volume_{0.5f};

  std::unordered_map<std::string, std::vector<std::string>> input_mappings_{
      {"move_left", {"A", "Left"}}, {"move_right", {"D", "Right"}},
      {"move_up", {"W", "Up"}},     {"move_down", {"S", "Down"}},
      {"jump", {"J", "Space"}},     {"attack", {"K", "MouseLeft"}},
      {"pause", {"P", "Escape"}}};
};
}  // namespace engine::core
