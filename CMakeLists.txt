cmake_minimum_required(VERSION 3.10)

# Set minimum policy version
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(twAudioDoorTool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Include FetchContent for downloading dependencies
include(FetchContent)

# Download and configure Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9
)
FetchContent_MakeAvailable(imgui)

# Fetch nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Fetch pugixml
FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.13
)
FetchContent_MakeAvailable(pugixml)

# Add source files
file(GLOB SOURCES
    "src/*.cpp"
    "src/components/*.cpp"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
    "libs/ImGuiFileDialog/ImGuiFileDialog.cpp"
)

# Add header files
file(GLOB HEADERS
    "src/*.h"
    "src/components/*.h"
    "${imgui_SOURCE_DIR}/imgui.h"
    "${imgui_SOURCE_DIR}/imgui_internal.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h"
    "libs/ImGuiFileDialog/ImGuiFileDialog.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Create assets directory in build folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Copy settings.json to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/assets/settings.json
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/settings.json
    COMMENT "Copying settings.json to build directory"
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json
    pugixml
    glfw
    ${OPENGL_LIBRARIES}
)

# Configure include paths
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${pugixml_SOURCE_DIR}/src
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}
)

# macOS specific configuration
if(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        "-framework Cocoa" 
        "-framework IOKit" 
        "-framework CoreAudio" 
        "-framework CoreVideo"
    )
endif()

# Windows specific configuration
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        winmm
        gdi32
        opengl32
    )
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()