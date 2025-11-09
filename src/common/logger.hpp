#include <spdlog/spdlog.h>
#include <format>

#define DECLARE_TAG(tag) constexpr char TAG[] = #tag;

#define LOG_FORMAT_STR(fmt, ...) std::format(fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_PREFIX_STR(tag) \
  LOG_FORMAT_STR("[{}][{}][{}][{}]", __FILE__, __LINE__, tag, __FUNCTION__)

#define LOGT(tag, msg, ...)                   \
  spdlog::trace("{} {}", LOG_PREFIX_STR(tag), \
                LOG_FORMAT_STR(msg __VA_OPT__(, ) __VA_ARGS__))
#define LOGD(tag, msg, ...)                   \
  spdlog::debug("{} {}", LOG_PREFIX_STR(tag), \
                LOG_FORMAT_STR(msg __VA_OPT__(, ) __VA_ARGS__))
#define LOGI(tag, msg, ...)                  \
  spdlog::info("{} {}", LOG_PREFIX_STR(tag), \
               LOG_FORMAT_STR(msg __VA_OPT__(, ) __VA_ARGS__))
#define LOGW(tag, msg, ...)                  \
  spdlog::warn("{} {}", LOG_PREFIX_STR(tag), \
               LOG_FORMAT_STR(msg __VA_OPT__(, ) __VA_ARGS__))
#define LOGE(tag, msg, ...)                   \
  spdlog::error("{} {}", LOG_PREFIX_STR(tag), \
                LOG_FORMAT_STR(msg __VA_OPT__(, ) __VA_ARGS__))
#define LOGC(tag, msg, ...)                      \
  spdlog::critical("{} {}", LOG_PREFIX_STR(tag), \
                   LOG_FORMAT_STR(msg __VA_OPT__(, ) __VA_ARGS__))

namespace engine::utils {
class Tracer {
 public:
  explicit Tracer(const std::string& file_name, int line,
                  const std::string& func_name, const std::string& tag)
      : log_prefix_(
            std::format("[{}][{}][{}][{}]", file_name, line, tag, func_name)) {
    spdlog::info("{} Enter", log_prefix_);
  }
  ~Tracer() {
    spdlog::info("{} Exit", log_prefix_);
  }
  Tracer(const Tracer&) = delete;
  Tracer& operator=(const Tracer&) = delete;
  Tracer(Tracer&&) = delete;
  Tracer& operator=(Tracer&&) = delete;

 private:
  std::string log_prefix_;
};
}  // namespace engine::utils

#define TRACEI(tag) \
  engine::utils::Tracer tracer(__FILE__, __LINE__, __func__, tag)
