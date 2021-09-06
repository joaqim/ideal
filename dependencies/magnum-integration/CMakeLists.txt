
  # Fetch ImGui if needed
FetchContent_GetProperties(imgui-src)
if (NOT imgui-src_POPULATED)
  FetchContent_Populate(imgui-src)
endif()

set(IMGUI_DIR ${imgui-src_SOURCE_DIR})
set(WITH_IMGUI ON CACHE BOOL "" FORCE)

FetchContent_GetProperties(magnum-integration)
if(NOT magnum-integration_POPULATED)
  message(STATUS "Fetching Magnum Integration ${MAGNUM_TAG}")
  FetchContent_Populate(magnum-integration)
  add_subdirectory(${magnum-integration_SOURCE_DIR} ${magnum-integration_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

