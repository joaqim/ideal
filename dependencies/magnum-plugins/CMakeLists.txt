set(WITH_STBTRUETYPEFONT ON CACHE BOOL "" FORCE)
set(WITH_TINYGLTFIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_STBIMAGEIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_ASSIMPIMPORTER ON CACHE BOOL "" FORCE)
 set(CMAKE_DISABLE_FIND_PACKAGE_assimp OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(assimp-src)
if(NOT assimp-src_POPULATED)
  FetchContent_Populate(assimp-src)
  add_subdirectory(${assimp-src_SOURCE_DIR} ${assimp-src_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(ASSIMP_LIBRARY "assimp")
set(ASSIMP_INCLUDE_DIR ${assimp-src_SOURCE_DIR}/include)
# add_library(${ASSIMP_LIBRARY} SHARED IMPORTED)
set_target_properties(${ASSIMP_LIBRARY} PROPERTIES IMPORTED_LOCATION "${assimp-src_BINARY_DIR}/libassimp.so")

FetchContent_GetProperties(magnum-plugins)
if(NOT magnum-plugins_POPULATED)
  message(STATUS "Fetching Magnum Plugins ${MAGNUM_TAG}")
  FetchContent_Populate(magnum-plugins)
  add_subdirectory(${magnum-plugins_SOURCE_DIR} ${magnum-plugins_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

