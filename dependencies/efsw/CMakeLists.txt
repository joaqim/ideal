
set(EFSW_VERBOSE OFF CACHE BOOL "Build efsw with verbose mode." FORCE)
# set(EFSW_STATIC_LIB OFF CACHE BOOL "Build efsw as a static library" FORCE)
set(EFSW_BUILD_TEST_APP OFF CACHE BOOL "Build the test app" FORCE)

if(BUILD_SHARED_LIBS)
  set(EFSW_STATIC_LIB FALSE)
else()
  set(EFSW_STATIC_LIB TRUE)
endif()

fetchcontent_getproperties(efsw)
if(NOT efsw_POPULATED)
  message(STATUS "Fetching efsw")
  fetchcontent_populate(efsw)
  add_subdirectory(${efsw_SOURCE_DIR} ${efsw_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
set_target_properties(efsw PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${efsw_SOURCE_DIR}/include")

# 
# if (EFSW_VERBOSE)
# 	add_definitions(-DEFSW_VERBOSE)
# endif()
# 
# set(SRCS
# 	src/efsw/Debug.cpp
# 	src/efsw/DirectorySnapshot.cpp
# 	src/efsw/DirectorySnapshotDiff.cpp
# 	src/efsw/DirWatcherGeneric.cpp
# 	src/efsw/FileInfo.cpp
# 	src/efsw/FileSystem.cpp
# 	src/efsw/FileWatcher.cpp
# 	src/efsw/FileWatcherCWrapper.cpp
# 	src/efsw/FileWatcherFSEvents.cpp
# 	src/efsw/FileWatcherGeneric.cpp
# 	src/efsw/FileWatcherImpl.cpp
# 	src/efsw/FileWatcherInotify.cpp
# 	src/efsw/FileWatcherKqueue.cpp
# 	src/efsw/FileWatcherWin32.cpp
# 	src/efsw/Log.cpp
# 	src/efsw/Mutex.cpp
# 	src/efsw/String.cpp
# 	src/efsw/System.cpp
# 	src/efsw/Thread.cpp
# 	src/efsw/Watcher.cpp
# 	src/efsw/WatcherFSEvents.cpp
# 	src/efsw/WatcherGeneric.cpp
# 	src/efsw/WatcherInotify.cpp
# 	src/efsw/WatcherKqueue.cpp
# 	src/efsw/WatcherWin32.cpp)
# 
# include_directories(include src)	
# 
# if (WIN32)
# 	list (APPEND SRCS
# 		  src/efsw/platform/win/FileSystemImpl.cpp
# 		  src/efsw/platform/win/MutexImpl.cpp
# 		  src/efsw/platform/win/SystemImpl.cpp
# 		  src/efsw/platform/win/ThreadImpl.cpp)
# else ()
#     list (APPEND SRCS
#           src/efsw/platform/posix/FileSystemImpl.cpp
#           src/efsw/platform/posix/MutexImpl.cpp
#           src/efsw/platform/posix/SystemImpl.cpp
#           src/efsw/platform/posix/ThreadImpl.cpp)
# endif()          
# 
# if (APPLE)
# 	list (REMOVE_ITEM SRCS
# 		  "src/efsw/WatcherInotify.cpp"
# 		  "src/efsw/WatcherWin32.cpp"
# 		  "src/efsw/FileWatcherInotify.cpp"
# 		  "src/efsw/FileWatcherWin32.cpp")
# 
# 	exec_program(uname ARGS -v  OUTPUT_VARIABLE OSX_VERSION)
#    	string(REGEX MATCH "[0-9]+" OSX_VERSION ${OSX_VERSION})
#    	if (NOT OSX_VERSION GREATER 9)
#     	add_definitions(-DEFSW_FSEVENTS_NOT_SUPPORTED)
#    	endif()  
# elseif (WIN32)
# 	list (REMOVE_ITEM SRCS 
# 		  "src/efsw/WatcherKqueue.cpp" 
# 		  "src/efsw/WatcherFSEvents.cpp" 
# 		  "src/efsw/WatcherInotify.cpp"
# 		  "src/efsw/FileWatcherKqueue.cpp"
# 		  "src/efsw/FileWatcherInotify.cpp"
# 		  "src/efsw/FileWatcherFSEvents.cpp")
# elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# 	list (REMOVE_ITEM SRCS
# 		  "src/efsw/WatcherKqueue.cpp"
# 		  "src/efsw/WatcherFSEvents.cpp"
# 		  "src/efsw/WatcherWin32.cpp"
# 		  "src/efsw/FileWatcherKqueue.cpp"
# 		  "src/efsw/FileWatcherWin32.cpp"
# 		  "src/efsw/FileWatcherFSEvents.cpp")
# 
# 	if (NOT EXISTS "/usr/include/sys/inotify.h" AND NOT EXISTS "/usr/local/include/sys/inotify.h")
# 		add_definitions(-DEFSW_INOTIFY_NOSYS)
# 	endif()	  
# elseif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
# 	list (REMOVE_ITEM SRCS
# 		  "src/efsw/WatcherInotify.cpp"
# 		  "src/efsw/WatcherWin32.cpp"
# 		  "src/efsw/WatcherFSEvents.cpp"
# 		  "src/efsw/FileWatcherInotify.cpp"
# 		  "src/efsw/FileWatcherWin32.cpp"
# 		  "src/efsw/FileWatcherFSEvents.cpp")		  
# endif()
# 
# if (MSVC)
# 	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
# else ()
# 	add_definitions(-Wall -Wno-long-long)
# endif()
# 
# if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
# 	add_definitions(-DDEBUG)
# elseif (${CMAKE_BUILD_TYPE} MATCHES "Release")
# 	add_definitions(-DNDEBUG)
# endif()
# 
# if (EFSW_STATIC_LIB)
# 	add_library(efsw STATIC ${SRCS})
# else()
# 	add_library(efsw SHARED ${SRCS})
# endif()
# 
# if (APPLE)
# 	set(MAC_LIBS "-framework CoreFoundation" "-framework CoreServices")
# 	target_link_libraries(efsw ${MAC_LIBS})
# elseif (NOT (${CMAKE_SYSTEM_NAME} MATCHES "Haiku") AND NOT WIN32)
# 	target_link_libraries(efsw pthread)
# endif()	
# 
#fetchcontent_getproperties(efsw)
#if(NOT assimp_POPULATED)
#message(STATUS "Fetching Assimp v${ASSIMP_VERSION}")
#fetchcontent_populate(assimp)
#add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR} EXCLUDE_FROM_ALL)
#endif()
#add_library(Assimp::Assimp STATIC IMPORTED)
#set_target_properties(Assimp::Assimp PROPERTIES IMPORTED_LOCATION ${assimp_BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}assimp${CMAKE_STATIC_LIBRARY_SUFFIX})
# set_target_properties(Assimp::Assimp PROPERTIES INTERFACE_LINK_LIBRARIES assimp::assimp)
# 
# get_target_property(_ASSIMP_INTERFACE_INCLUDE_DIRECTORIES assimp::assimp INTERFACE_INCLUDE_DIRECTORIES)
# set(ASSIMP_INCLUDE_DIR ${_ASSIMP_INTERFACE_INCLUDE_DIRECTORIES})
# set(ASSIMP_LIBRARY Assimp::Assimp)
