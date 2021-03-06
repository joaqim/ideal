find_package(Corrade REQUIRED Utility PluginManager)

if(CORRADE_TARGET_ANDROID)
  find_package(Magnum REQUIRED AndroidApplication)
elseif(CORRADE_TARGET_EMSCRIPTEN)
  find_package(Magnum REQUIRED EmscriptenApplication)
else()
  find_package(Magnum REQUIRED Sdl2Application)
endif()


set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

# Resources/Assets
#corrade_add_resource(Resources assets/assets.conf)


# Modules

set(MODULES_SRCS TestModule.cpp modules/ImGuiStyleModule.cpp modules/GameModule.cpp)
foreach(MOD_SRC IN LISTS MODULES_SRCS)
  get_filename_component(MOD_NAME ${MOD_SRC} NAME_WE)
  corrade_add_plugin(${MOD_NAME} ${CMAKE_CURRENT_BINARY_DIR} "" "" ${MOD_SRC})
  set_target_properties(${MOD_NAME} PROPERTIES COMPILE_FLAGS "-Wno-return-local-addr")
  set(MODULE_DEPS Corrade::PluginManager Corrade::Utility)

  # set(IMGUI_DEP off)
  string(COMPARE EQUAL ${MOD_NAME} "ImGuiStyleModule" IMGUI_DEP)
  if(IMGUI_DEP)
    list(APPEND MODULE_DEPS MagnumIntegration::ImGui)
  endif()
  target_link_libraries(${MOD_NAME} ${MODULE_DEPS})
endforeach()

find_package(
  Magnum REQUIRED
  GL
  Shaders
  MeshTools
  Primitives
  #SceneGraph
  Trade
  Sdl2Application
  AnyImageImporter
  TextureTools
  Text
)

find_package(
  MagnumPlugins REQUIRED
  TinyGltfImporter
  StbImageImporter
)

find_package(MagnumIntegration REQUIRED ImGui)


find_package(Threads)

add_library(ideal-deps STATIC)
target_sources(ideal-deps PRIVATE static_library_dummy_file.cpp)

target_link_libraries(
  ideal-deps PUBLIC
  ${CMAKE_THREAD_LIBS_INIT}
  EnTT::EnTT
  Corrade::Main
  Corrade::Utility
  Corrade::PluginManager
  Magnum::Application
  Magnum::GL
  Magnum::Magnum
  Magnum::MeshTools
  Magnum::Primitives
  #Magnum::SceneGraph
  Magnum::Shaders
  Magnum::Trade
  Magnum::AnyImageImporter
  Magnum::TextureTools
  Magnum::Text
  MagnumPlugins::TinyGltfImporter
  MagnumPlugins::StbImageImporter
  MagnumPlugins::StbTrueTypeFont
  MagnumIntegration::ImGui
  efsw
)

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/src/assets")
file(
  GLOB_RECURSE ASSETS_CONF_LIST CONFIGURE_DEPENDS
  "${ASSETS_DIR}/*.conf"
)

foreach(ASSETS_CONF IN LISTS ASSETS_CONF_LIST)
  corrade_add_resource(CORRADE_RESOURCES "${ASSETS_CONF}")
endforeach()

# Gather paths to OSP headers and sources
# add_executable(ideal ${CORRADE_RESOURCES})
add_executable(ideal)

# Link it to the interface library that describes the dependencies
target_link_libraries(ideal PRIVATE ideal-deps)

file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE HPP_FILES CONFIGURE_DEPENDS *.hpp)
file(GLOB_RECURSE H_FILES CONFIGURE_DEPENDS *.h)

list(FILTER CPP_FILES EXCLUDE REGEX ".*(MyApplication|\/modules\/.*)\.cpp$")
list(FILTER HPP_FILES EXCLUDE REGEX ".*(ImGuiStyle)\.hpp$")

target_sources(ideal PRIVATE "${CPP_FILES}" "${HPP_FILES}" "${H_FILES}" "${CORRADE_RESOURCES}")

# For some reason, source_group() requires all files to be added to the executable
set(SHADERS_DIR "${CMAKE_SOURCE_DIR}/src/assets/shaders")
file(
  GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS
  "${SHADERS_DIR}/*.vert"
  "${SHADERS_DIR}/*.frag"
  "${SHADERS_DIR}/*.comp"
)


target_sources(ideal PRIVATE "${SHADER_FILES}")

# Set ideal as startup project
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ideal)

target_include_directories(ideal PRIVATE .)

# MSVC quality of life improvements for Visual Studio

# Set the MSVC debug working directory
set_property(TARGET ideal PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enforce conformance mode for ideal
target_compile_options(ideal PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/permissive->)

# Segregate headers, shaders into filters
source_group("Shader Files" FILES ${SHADER_FILES})
source_group("Header Files" FILES ${H_FILES})

# Put executable in the bin folder
#set_target_properties(ideal PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(ideal PROPERTIES EXPORT_COMPILE_COMMANDS TRUE)

# Copy src/modules/*.conf to build/lib/
add_custom_command(
  TARGET ideal 
  POST_BUILD COMMAND 
  ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/src/modules/*.conf 
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set_target_properties(ideal PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
set_target_properties(ideal PROPERTIES INSTALL_RPATH "../lib;./lib")

#set(LIB_DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
#install(TARGETS ideal-deps LIBRARY DESTINATION ${LIB_DESTINATION} ARCHIVE DESTINATION ${LIB_DESTINATION} RUNTIME DESTINATION ${LIB_DESTINATION})
#install(TARGETS ideal RUNTIME DESTINATION bin)
#install(TARGETS ideal DESTINATION ${CMAKE_BINARY_DIR}/bin)

# add_executable(MyApplication
#   MyApplication.cpp
#   ${Resources}
#   )
#
# target_link_libraries(MyApplication PRIVATE
#   Magnum::Application
#   Magnum::GL
#   Magnum::Magnum
#   Magnum::Shaders
#   Magnum::SceneGraph
#   Magnum::Text
#   MagnumIntegration::ImGui
#
#   Corrade::Utility
#   Corrade::PluginManager
#
#   EnTT::EnTT
#   )
#
# if(WIN32)
#   add_custom_command(TARGET MyApplication POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     $<TARGET_FILE:SDL2>
#     $<TARGET_FILE:Magnum::Application>
#     $<TARGET_FILE:Magnum::GL>
#     $<TARGET_FILE:Magnum::Magnum>
#     $<TARGET_FILE:Magnum::Shaders>
#     $<TARGET_FILE:Magnum::SceneGraph>
#     $<TARGET_FILE:Magnum::Text>
#     $<TARGET_FILE:MagnumIntegration::ImGui>
#
#     $<TARGET_FILE:Corrade::Utility>
#     $<TARGET_FILE:Corrade::PluginManager>
#
#     $<TARGET_FILE:EnTT::EnTT>
#     )
# endif()

# add_custom_target(copy_assets COMMAND ${CMAKE_COMMAND} -P ${CMAKE_MODULE_PATH}/copy-assets.cmake)
# add_dependencies(MyApplication copy_assets)
#install(TARGETS MyApplication DESTINATION ${CMAKE_BINARY_DIR}/bin)


#set_target_properties(MyApplication PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set_target_properties(MyApplication PROPERTIES EXPORT_COMPILE_COMMANDS TRUE)

# Make the executable a default target to build & run in Visual Studio
#set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyApplication)
