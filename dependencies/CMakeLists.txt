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

set(ENTT_VERSION 3.8.1)
set(IMGUI_VERSION 1.76)
set(ASSIMP_VERSION 5.0.1)
set(SDL_VERSION 2.0.16)
set(CORRADE_TAG 2020.06)
set(MAGNUM_TAG 2020.06)

# SDL

if(NOT IDEAL_USE_SYSTEM_SDL)
  add_subdirectory(sdl)
endif()

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

# EnTT

fetchcontent_declare(
  entt
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG dd6863f71da1b360ec09c25912617a3423f08149 # v3.8.1
)
add_subdirectory(entt)


# Glfw

#
# FetchContent_Declare(
#   glfw
#   GIT_REPOSITORY https://github.com/glfw/glfw
#   GIT_TAG 814b7929c5add4b0541ccad26fb81f28b71dc4d8 # v3.3.4
#   )
#
#
# FetchContent_GetProperties(glfw)
# if(NOT glfw_POPULATED)
#   FetchContent_Populate(glfw)
#
#   set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#   set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
#   set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
#   # These two will be off-by-default when GLFW 3.4 gets released
#   set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
#   set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#
#   add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
#

# ImGui

fetchcontent_declare(
  imgui-src
  URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)

add_subdirectory(imgui EXCLUDE_FROM_ALL)

# Assimp, required by Magnum::Trade::AssimpImporter

# fetchcontent_declare(
#   assimp
#   GIT_REPOSITORY https://github.com/assimp/assimp
#   GIT_TAG 8f0c6b04b2257a520aaab38421b2e090204b69df # v5.0.1
# )
# 
# add_subdirectory(assimp EXCLUDE_FROM_ALL)

# Corrade

fetchcontent_declare(
  corrade
  GIT_REPOSITORY https://github.com/mosra/corrade
  GIT_TAG 61d1b58cbcb159837bca506b5336a810da67f0a7 # 2020.06
)
add_subdirectory(corrade)

# Magnum

fetchcontent_declare(
  magnum
  GIT_REPOSITORY https://github.com/mosra/magnum
  GIT_TAG fac6f4da261930c7035bf51f66091e350905720b # 2020.06
)

add_subdirectory(magnum)

# Magnum Plugins

#find_package(Assimp REQUIRED)
# message(${ASSIMP_FOUND})
# message(${ASSIMP_INCLUDE_DIR})
# message(${ASSIMP_LIBRARY})

fetchcontent_declare(
  magnum-plugins
  GIT_REPOSITORY https://github.com/mosra/magnum-plugins
  GIT_TAG b0f2818b781eb0fb6a8a76095a6529fc85d442f3 # 2020.06
)

add_subdirectory(magnum-plugins)

# Magnum Integration

fetchcontent_declare(
  magnum-integration
  GIT_REPOSITORY https://github.com/mosra/magnum-integration
  GIT_TAG a6c469de651032746a965c5aa83c243ca6a097a6 # 2020.06
)

add_subdirectory(magnum-integration)

message(STATUS "Fetching thirdparty libraries done")
