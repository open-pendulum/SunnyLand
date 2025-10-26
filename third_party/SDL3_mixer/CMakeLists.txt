include(FetchContent)
FetchContent_Declare(
        SDL3_mixer
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL_mixer.git"
        GIT_TAG "30c1301055a35ee87b8679279b6fc88e10d28fa3"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL3_mixer)

