cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(BounceTales VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(TGUI_BACKEND SFML_GRAPHICS)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Download libraries
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
  TGUI
  GIT_REPOSITORY https://github.com/texus/TGUI
  GIT_TAG v1.7.0  
)
FetchContent_MakeAvailable(TGUI)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2)
FetchContent_MakeAvailable(json)

file(GLOB_RECURSE sources src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${sources})
target_include_directories(${PROJECT_NAME} PRIVATE src/include)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio sfml-window sfml-system)
target_link_libraries(${PROJECT_NAME} PRIVATE TGUI::TGUI)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()

add_custom_target(copy-assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    ${CMAKE_SOURCE_DIR}/extra ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMENT "Copying assets"
)


add_dependencies(${PROJECT_NAME} copy-assets)
# install(TARGETS ${PROJECT_NAME})
