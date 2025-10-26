#pragma once
#include <cstdint>

namespace engine::core {
class Time {
 public:
  Time();

  Time(const Time& other) = delete;
  Time& operator=(const Time& other) = delete;
  Time(Time&& other) = delete;
  Time& operator=(Time&& other) = delete;

  void Update();

  [[nodiscard]] double GetDeltaTimeS() const;

  [[nodiscard]] double GetUnscaledDeltaTimeS() const;

  void SetTimeScale(double scale);

  [[nodiscard]] double GetTimeScale() const;

  void SetTargetFPS(int32_t fps);
  [[nodiscard]] int32_t GetTargetFPS() const;

 private:
  void LimitFrameRate(double delta_time_s);

 private:
  uint64_t last_time_ns_{0};
  uint64_t current_frame_start_time_ns_{0};
  double delta_time_s_{0.0};
  double time_scale_factor_{1.0};

  int32_t target_fps_{0};
  double target_frame_duration_s_{0.0};
};
}  // namespace engine::core