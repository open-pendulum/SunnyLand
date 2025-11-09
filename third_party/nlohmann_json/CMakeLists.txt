# 启用 FetchContent 模块（CMake 3.11+ 支持，若版本较低需升级 CMake）
include(FetchContent)

# 声明要下载的 nlohmann_json 版本（可替换为你需要的版本，如 v3.11.2、v3.10.5 等）
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0  # 推荐使用稳定版本，避免直接用 main 分支
  GIT_SHALLOW ON   # 只下载最新提交，加快速度
  GIT_PROGRESS TRUE
)

# 自动下载、配置、编译（头文件-only 库不会编译二进制，速度很快）
FetchContent_MakeAvailable(nlohmann_json)