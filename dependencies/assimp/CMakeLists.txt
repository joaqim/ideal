FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp
  GIT_TAG 8f0c6b04b2257a520aaab38421b2e090204b69df # v5.0.1
  )

# Disable Assimp tests, tools and exporter functionality
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
# If you won't be accessing Assimp outside of the plugin, build it as static to
# have the plugin binary self-contained
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# The following is important to avoid Assimp appending `d` to all your
# binaries. You need Assimp >= 5.0.0 for this to work, also note that after
# 5.0.1 this option is prefixed with ASSIMP_, so better set both variants.
set(INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(assimp)
if(NOT assimp_POPULATED)
  FetchContent_Populate(assimp)
  add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
