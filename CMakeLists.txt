cmake_minimum_required(VERSION 3.16)
project(BounceTales VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Binaries path")
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)
set(TGUI_BACKEND SFML_GRAPHICS CACHE STRING "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
  src/include
  ${DEPS_DIR}/TGUI/include
  ${DEPS_DIR}/SFML/include
  ${DEPS_DIR}/json/include
)

add_subdirectory(${DEPS_DIR}/SFML)
add_subdirectory(${DEPS_DIR}/TGUI)
add_subdirectory(${DEPS_DIR}/json)

target_link_libraries(${PROJECT_NAME} PRIVATE
  sfml-graphics
  sfml-audio
  sfml-window
  sfml-system
  TGUI::TGUI
  nlohmann_json::nlohmann_json
)

add_custom_command(
  TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
  ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
  ${CMAKE_SOURCE_DIR}/extra ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMENT "Copying assets"
)

if (WIN32)
  add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${DEPS_DIR}/SFML/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMENT "Copying OpenAL DLL"
  )
endif ()
