#include "time.h"
#include "logger.hpp"

#include <SDL3/SDL_time.h>
#include <SDL3/SDL_timer.h>

namespace engine::core {
namespace {
DECLARE_TAG(Time)
constexpr double kNSPerSec = 1000000000.0;
}  // namespace

Time::Time()
    : last_time_ns_(SDL_GetTicksNS()),
      current_frame_start_time_ns_(last_time_ns_) {
  LOGI(TAG, "Init Time: last time is {}", last_time_ns_);
}
void Time::Update() {
  current_frame_start_time_ns_ = SDL_GetTicksNS();
  auto delta_time_s =
      static_cast<double>(current_frame_start_time_ns_ - last_time_ns_) /
      kNSPerSec;
  if (target_frame_duration_s_ > 0.0) {
    LimitFrameRate(delta_time_s);
  } else {
    delta_time_s_ = delta_time_s;
  }
  last_time_ns_ = SDL_GetTicksNS();
}
double Time::GetDeltaTimeS() const {
  return time_scale_factor_ * delta_time_s_;
}
double Time::GetUnscaledDeltaTimeS() const {
  return delta_time_s_;
}
void Time::SetTimeScale(double scale) {
  if (scale < 0.0) {
    LOGW(
        TAG,
        "Time Scale can not be a negative, will be claming to 0.0. error value "
        "is ",
        scale);
    scale = 0.0;
  }
  time_scale_factor_ = scale;
}
double Time::GetTimeScale() const {
  return time_scale_factor_;
}
void Time::SetTargetFPS(int32_t fps) {
  if (fps <= 0) {
    LOGW(TAG,
         "Fps cannot be a negative. Resetting to 0 (unlimit).error value is {}",
         fps);
    target_fps_ = 0;
  } else {
    target_fps_ = fps;
    target_frame_duration_s_ = 1.0 / static_cast<double>(target_fps_);
    LOGI(TAG, "Set FPS {}, target frame duration is {}", target_fps_,
         target_frame_duration_s_);
  }
}

int32_t Time::GetTargetFPS() const {
  return target_fps_;
}

void Time::LimitFrameRate(double delta_time_s) {
  if (delta_time_s < target_frame_duration_s_) {
    auto time_to_wait_ns = static_cast<uint64_t>(
        (target_frame_duration_s_ - delta_time_s) * kNSPerSec);
    SDL_DelayNS(time_to_wait_ns);
    delta_time_s_ =
        static_cast<double>(SDL_GetTicksNS() - last_time_ns_) / kNSPerSec;
  }
}
}  // namespace engine::core