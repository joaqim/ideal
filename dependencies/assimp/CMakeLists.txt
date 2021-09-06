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

fetchcontent_getproperties(assimp)
if(NOT assimp_POPULATED)
  message(STATUS "Fetching Assimp v${ASSIMP_VERSION}")
  fetchcontent_populate(assimp)
  add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
add_library(Assimp::Assimp STATIC IMPORTED)
set_target_properties(Assimp::Assimp PROPERTIES IMPORTED_LOCATION ${assimp_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}assimp${CMAKE_STATIC_LIBRARY_SUFFIX})
set_target_properties(Assimp::Assimp PROPERTIES INTERFACE_LINK_LIBRARIES assimp::assimp)

get_target_property(_ASSIMP_INTERFACE_INCLUDE_DIRECTORIES assimp::assimp INTERFACE_INCLUDE_DIRECTORIES)
set(ASSIMP_INCLUDE_DIR ${_ASSIMP_INTERFACE_INCLUDE_DIRECTORIES})
set(ASSIMP_LIBRARY Assimp::Assimp)
#set_target_properties(Assimp::Assimp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${assimp_BINARY_DIR}/include")

#find_package(Assimp REQUIRED)
#add_library(Assimp::Assimp INTERFACE IMPORTED)
#set_target_properties(Assimp::Assimp PROPERTIES INTERFACE_LINK_LIBRARIES assimp::assimp)
#set_target_properties(Assimp::Assimp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${assimp_BINARY_DIR}/include")
