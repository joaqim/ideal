include(FetchContent)

option(USE_SYSTEM_DEPS CACHE OFF)
option(LINK_DEPS_STATIC CACHE OFF)
option(OpenGL_GL_PREFERENCE "GLVND")

# Don't build shared libs if we want to link to deps statically
if(LINK_DEPS_STATIC)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

message(STATUS "Fetching third party libraries...")

set(IMGUI_VERSION 1.76)

# EnTT

FetchContent_Declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG dd6863f71da1b360ec09c25912617a3423f08149 # v3.8.1
  )

#FetchContent_MakeAvailable(entt)
FetchContent_GetProperties(entt)
if(NOT entt_POPULATED)
  FetchContent_Populate(entt)
  add_subdirectory(${entt_SOURCE_DIR} ${entt_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Corrade

FetchContent_Declare(
  corrade
  GIT_REPOSITORY https://github.com/mosra/corrade
  GIT_TAG 61d1b58cbcb159837bca506b5336a810da67f0a7 # 2020.06
  )

  FetchContent_GetProperties(corrade)
if(NOT corrade_POPULATED)
  FetchContent_Populate(corrade)
  add_subdirectory(${corrade_SOURCE_DIR} ${corrade_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Glfw

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 814b7929c5add4b0541ccad26fb81f28b71dc4d8 # v3.3.4
  )


FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)

  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
  # These two will be off-by-default when GLFW 3.4 gets released
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Sdl2
if(NOT IDEAL_USE_SYSTEM_SDL)
  # This is the most minimal set of features which still make Sdl2Application
  # work. If you need something from these, remove the setting. The SDL_AUDIO
  # option (and SDL_DLOPEN) should not be needed either as Magnum doesn't use it,
  # but if it's disabled it causes linker errors. Needs further investigation.
  SET(SDL_ATOMIC     OFF CACHE BOOL "" FORCE)
  SET(SDL_CPUINFO    OFF CACHE BOOL "" FORCE)
  SET(SDL_FILE       ON  CACHE BOOL "" FORCE) # Needed for Mac OSX
  SET(SDL_FILESYSTEM OFF CACHE BOOL "" FORCE)
  SET(SDL_HAPTIC     OFF CACHE BOOL "" FORCE)
  SET(SDL_LOCALE     OFF CACHE BOOL "" FORCE)
  SET(SDL_POWER      OFF CACHE BOOL "" FORCE)
  SET(SDL_RENDER     OFF CACHE BOOL "" FORCE)
  SET(SDL_SENSOR     OFF CACHE BOOL "" FORCE)
  SET(SDL_THREADS    OFF CACHE BOOL "" FORCE)
  SET(SDL_TIMERS     OFF CACHE BOOL "" FORCE)

  SET(SDL_AUDIO         ON  CACHE BOOL "" FORCE)
  SET(PULSEAUDIO        ON  CACHE BOOL "" FORCE)
  SET(PULSEAUDIO_SHARED OFF CACHE BOOL "" FORCE)
  SET(ALSA              OFF CACHE BOOL "" FORCE)
  SET(ALSA_SHARED       OFF CACHE BOOL "" FORCE)
  SET(JACK              OFF CACHE BOOL "" FORCE)
  SET(JACK_SHARED       OFF CACHE BOOL "" FORCE)
  SET(DISKAUDIO         OFF CACHE BOOL "" FORCE)
  SET(DISKAUDIO_SHARED  OFF CACHE BOOL "" FORCE)
  SET(SNDIO             OFF CACHE BOOL "" FORCE)
  SET(SNDIO_SHARED      OFF CACHE BOOL "" FORCE)
  SET(DUMMYAUDIO        OFF CACHE BOOL "" FORCE)
  SET(DUMMYAUDIO_SHARED OFF CACHE BOOL "" FORCE)
  SET(ESD               OFF CACHE BOOL "" FORCE)
  SET(ESD_SHARED        OFF CACHE BOOL "" FORCE)
  SET(ARTS              OFF CACHE BOOL "" FORCE)
  SET(ARTS_SHARED       OFF CACHE BOOL "" FORCE)
  SET(NAS               OFF CACHE BOOL "" FORCE)
  SET(NAS_SHARED        OFF CACHE BOOL "" FORCE)
  SET(OSS               OFF CACHE BOOL "" FORCE)
  SET(OSS_SHARED        OFF CACHE BOOL "" FORCE)

  SET(SDL_JOYSTICK     ON  CACHE BOOL "" FORCE)
  SET(JOYSTICK_VIRTUAL ON  CACHE BOOL "" FORCE)

  SET(SDL_LOADSO             ON  CACHE BOOL "" FORCE)
  SET(SDL_VIDEO              ON  CACHE BOOL "" FORCE)
  SET(VIDEO_VULKAN           OFF CACHE BOOL "" FORCE)
  SET(VIDEO_DUMMY            OFF CACHE BOOL "" FORCE)
  SET(VIDEO_OPENGL           ON  CACHE BOOL "" FORCE)
  SET(VIDEO_OPENGLES         OFF CACHE BOOL "" FORCE)
  SET(VIDEO_WAYLAND          OFF CACHE BOOL "" FORCE)
  SET(VIDEO_WAYLAND_QT_TOUCH OFF CACHE BOOL "" FORCE)
  SET(VIDEO_KMSDRM           OFF CACHE BOOL "" FORCE)
  SET(KMSDRM_SHARED          OFF CACHE BOOL "" FORCE)
  SET(VIDEO_RPI              OFF CACHE BOOL "" FORCE)
  SET(VIDEO_VIVANTE          OFF CACHE BOOL "" FORCE)

  SET(LIBSAMPLERATE          ON CACHE BOOL "" FORCE)
  SET(LIBSAMPLERATE_SHARED   ON CACHE BOOL "" FORCE)

  SET(SDL_EVENTS     ON  CACHE BOOL "" FORCE)

  # This assumes you want to have SDL as a static library. If not, set SDL_STATIC
  # to OFF instead.
  SET(SDL_DLOPEN ON  CACHE BOOL "" FORCE)
  SET(SDL_SHARED OFF  CACHE BOOL "" FORCE)
  SET(SDL_STATIC ON CACHE BOOL "" FORCE)

  #ADD_SUBDIRECTORY(SDL2 EXCLUDE_FROM_ALL)
  FetchContent_Declare(
    sdl
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG 25f9ed87ff6947d9576fc9d79dee0784e638ac58 # v2.0.16
    )
  #FetchContent_MakeAvailable(SDL)
  FetchContent_GetProperties(sdl)
  if(NOT sdl_POPULATED)
    FetchContent_Populate(sdl)
    add_subdirectory(${sdl_SOURCE_DIR} ${sdl_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endif() # NOT IDEAL_USE_SYSTEM_SDL

# if(WIN32)
#   FetchContent_Declare(
#     SDL
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL
#     GIT_TAG 25f9ed87ff6947d9576fc9d79dee0784e638ac58 # v2.0.16
#     )
#   if (LINK_DEPS_STATIC)
#     set(SDL_STATIC ON CACHE BOOL "Build SDL2 as shared lib")
#   else()
#     set(SDL_SHARED ON CACHE BOOL "Build SDL2 as shared lib")
#   endif()
#   FetchContent_MakeAvailable(SDL)
# 
#   # Define alias for what we used
#   if (LINK_DEPS_STATIC)
#     add_library(SDL2::SDL2 ALIAS SDL2-static)
#   else()
#     add_library(SDL2::SDL2 ALIAS SDL2)
#   endif()
# endif()
# 
# 
# FetchContent_Declare(
#   SDL2
#   GIT_REPOSITORY https://github.com/libsdl-org/SDL
#   GIT_TAG 25f9ed87ff6947d9576fc9d79dee0784e638ac58 # v2.0.16
#   )
# 
# if (LINK_DEPS_STATIC)
#   set(SDL_STATIC ON CACHE BOOL "Build SDL2 as shared lib")
# else()
#   set(SDL_SHARED ON CACHE BOOL "Build SDL2 as shared lib")
# endif()
# FetchContent_MakeAvailable(SDL2)
# 
# # Define alias for what we used
# if (LINK_DEPS_STATIC)
#   add_library(SDL2::SDL2 ALIAS SDL2-static)
# else()
#   add_library(SDL2::SDL2 ALIAS SDL2)
# endif()
# 
# ImGui

FetchContent_Declare(
  imgui-src
  URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
  )

add_subdirectory(imgui EXCLUDE_FROM_ALL)

# Magnum

FetchContent_Declare(
  magnum
  GIT_REPOSITORY https://github.com/mosra/magnum
  GIT_TAG fac6f4da261930c7035bf51f66091e350905720b # 2020.06
  )

set(BUILD_PLUGINS_STATIC  ON  CACHE BOOL "" FORCE)
set(WITH_SDL2APPLICATION  ON  CACHE BOOL "" FORCE)
set(WITH_ANYIMAGEIMPORTER ON  CACHE BOOL "" FORCE)
set(WITH_TESTSUITE        OFF CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(magnum)
FetchContent_GetProperties(magnum)
if(NOT magnum_POPULATED)
  FetchContent_Populate(magnum)
  add_subdirectory(${magnum_SOURCE_DIR} ${magnum_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Magnum Plugins


FetchContent_Declare(
  magnum-plugins
  GIT_REPOSITORY https://github.com/mosra/magnum-plugins
  GIT_TAG b0f2818b781eb0fb6a8a76095a6529fc85d442f3 # 2020.06
  )

set(WITH_STBTRUETYPEFONT ON CACHE BOOL "" FORCE)
set(WITH_TINYGLTFIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_STBIMAGEIMPORTER ON CACHE BOOL "" FORCE)

FetchContent_GetProperties(magnum-plugins)
if(NOT magnum-plugins_POPULATED)
  FetchContent_Populate(magnum-plugins)
  add_subdirectory(${magnum-plugins_SOURCE_DIR} ${magnum-plugins_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()


# Magnum Integration

FetchContent_Declare(
  magnum-integration
  GIT_REPOSITORY https://github.com/mosra/magnum-integration
  GIT_TAG a6c469de651032746a965c5aa83c243ca6a097a6 # 2020.06
  )

# Fetch ImGui if needed
FetchContent_GetProperties(imgui-src)
if (NOT imgui-src_POPULATED)
  FetchContent_Populate(imgui-src)
endif()

set(IMGUI_DIR ${imgui-src_SOURCE_DIR})
set(WITH_IMGUI ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(magnum-integration)

FetchContent_GetProperties(magnum-integration)
if(NOT magnum-integration_POPULATED)
  FetchContent_Populate(magnum-integration)
  add_subdirectory(${magnum-integration_SOURCE_DIR} ${magnum-integration_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

message(STATUS "Fetching thirdparty libraries done")
