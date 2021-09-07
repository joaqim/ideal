set(WITH_STBTRUETYPEFONT ON CACHE BOOL "" FORCE)
set(WITH_TINYGLTFIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_STBIMAGEIMPORTER ON CACHE BOOL "" FORCE)
set(WITH_ASSIMPIMPORTER OFF CACHE BOOL "" FORCE)
# 
# set(CMAKE_DISABLE_FIND_PACKAGE_assimp ON CACHE BOOL "" FORCE)
# 
# 
# fetchcontent_getproperties(assimp)
# if(NOT assimp_POPULATED)
#   fetchcontent_populate(assimp)
#   add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()
# 
#set(ASSIMP_ROOT_DIR ${assimp_SOURCE_DIR})
#find_package(Assimp REQUIRED)
#message(${ASSIMP_LIBRARIES})
#message(${ASSIMP_FOUND})
#message(${ASSIMP_INCLUDE_DIR})

# set(CMAKE_PREFIX_PATH ${assimp_BINARY_DIR})

fetchcontent_getproperties(magnum-plugins)
if(NOT magnum-plugins_POPULATED)
  message(STATUS "Fetching Magnum Plugins ${MAGNUM_TAG}")
  fetchcontent_populate(magnum-plugins)
  add_subdirectory(${magnum-plugins_SOURCE_DIR} ${magnum-plugins_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
