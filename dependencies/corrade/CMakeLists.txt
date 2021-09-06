
FetchContent_GetProperties(corrade)
if(NOT corrade_POPULATED)
  message(STATUS "Fetching Corrade v${CORRADE_TAG}")
  FetchContent_Populate(corrade)
  add_subdirectory(${corrade_SOURCE_DIR} ${corrade_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

