#include "config.h"
#include "logger.hpp"

#include <fstream>

namespace engine::core {
namespace {
DECLARE_TAG(Config)
}  // namespace

Config::Config(const std::string& file_name) {
  LoadConfig(file_name);
}
bool Config::LoadConfig(const std::string& file_name) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    LOGE(TAG, "Failed to open config file: {}!", file_name);
    return false;
  }
  try {
    nlohmann::json j;
    file >> j;
    FromJson(j);
    LOGI(TAG, "Loaded config file: {}", file_name);
    return true;
  } catch (const nlohmann::json::exception& e) {
    LOGE(TAG, "Failed to parse config file: {}! Error: {}", file_name, e.what());
    return false;
  }
}
bool Config::SaveConfig(const std::string& file_name) {
  std::ofstream file(file_name);
  if (!file.is_open()) {
    LOGE(TAG, "Failed to open config file: {}!", file_name);
    return false;
  }

  try {
    nlohmann::json j = ToJson();
    file << j.dump(4);
    LOGI(TAG, "Saved config file: {}", file_name);
    return true;
  } catch (const nlohmann::json::exception& e) {
    LOGE(TAG, "Failed to save config file: {}! Error: {}", file_name, e.what());
    return false;
  }
}
const std::string& Config::WindowTitle() const {
  return window_title_;
}

const int32_t& Config::WindowWidth() const {
  return window_width_;
}

const int32_t& Config::WindowHeight() const {
  return window_height_;
}
const bool& Config::WindowResizable() const {
  return window_resizable_;
}
const bool& Config::VSync() const {
  return vsync_;
}

const int32_t& Config::TargetFps() const {
  return target_fps_;
}
const float& Config::MusicVolume() const {
  return music_volume_;
}
const float& Config::SoundVolume() const {
  return sound_volume_;
}

const std::unordered_map<std::string, std::vector<std::string>>& Config::InputMappings() const {
  return input_mappings_;
}

void Config::FromJson(const nlohmann::json& json) {
  if (json.contains("window")) {
    const auto& window_json = json["window"];
    if (window_json.contains("title")) {
      window_title_ = window_json["title"];
    }
    if (window_json.contains("width")) {
      window_width_ = window_json["width"];
    }
    if (window_json.contains("height")) {
      window_height_ = window_json["height"];
    }
    if (window_json.contains("resizable")) {
      window_resizable_ = window_json["resizable"];
    }
  }
  if (json.contains("graphics")) {
    const auto& graphics_json = json["graphics"];
    if (graphics_json.contains("vsync")) {
      vsync_ = graphics_json["vsync"];
    }
  }

  if (json.contains("performance")) {
    const auto& performance_json = json["performance"];
    if (performance_json.contains("fps")) {
      target_fps_ = performance_json["fps"];
      if (target_fps_ <= 0) {
        LOGE(TAG, "TargetFps must be greater than zero, 0 is unlimited");
        target_fps_ = 0;
      }
    }
  }
  if (json.contains("audio")) {
    const auto& audio_json = json["audio"];
    if (audio_json.contains("music_volume")) {
      music_volume_ = audio_json["music_volume"];
    }
    if (audio_json.contains("sound_volume")) {
      sound_volume_ = audio_json["sound_volume"];
    }
  }

  if (json.contains("input_mappings")) {
    const auto& input_mappings_json = json["input_mappings"];
    try {
      auto input_mappings = input_mappings_json.get<std::unordered_map<std::string, std::vector<std::string>>>();
      input_mappings_ = std::move(input_mappings);
      LOGI(TAG, "Loaded input mappings: {}", input_mappings_json.dump());
    } catch (const nlohmann::json::exception& e) {
      LOGE(TAG, "Failed to parse input mappings! Error: {}", e.what());
    }
  } else {
    LOGW(TAG, "No input mapping found");
  }
}
nlohmann::json Config::ToJson() const {
  return nlohmann::ordered_json{{"window",
                                 {{"title", window_title_},
                                  {"width", window_width_},
                                  {"height", window_height_},
                                  {"resizable", window_resizable_}}},
                                {"graphics", {{"vsync", vsync_}}},
                                {"performance", {{"target_fps", target_fps_}}},
                                {"audio", {{"music_volume", music_volume_}, {"sound_volume", sound_volume_}}},
                                {"input_mappings", input_mappings_}};
}
}  // namespace engine::core
