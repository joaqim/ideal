# Disable Assimp tests, tools and exporter functionality
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
# If you won't be accessing Assimp outside of the plugin, build it as static to
# have the plugin binary self-contained
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

# The following is important to avoid Assimp appending `d` to all your
# binaries. You need Assimp >= 5.0.0 for this to work, also note that after
# 5.0.1 this option is prefixed with ASSIMP_, so better set both variants.
set(INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(assimp-src)
if(NOT assimp-src_POPULATED)
  message(STATUS "Fetching Assimp v${ASSIMP_VERSION}")
  FetchContent_Populate(assimp-src)
  add_subdirectory(${assimp-src_SOURCE_DIR} ${assimp-src_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
