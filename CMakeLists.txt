cmake_minimum_required(VERSION 3.10)

# Définir la politique de version minimale
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(twAudioDoorTool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Définir les options de build
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Inclure FetchContent pour télécharger les dépendances
include(FetchContent)

# Télécharger et configurer Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# Télécharger RayGui (header-only)
FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG 3.5
)
FetchContent_MakeAvailable(raygui)

# Fetch TinyXML2
FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG 9.0.0
)
FetchContent_MakeAvailable(tinyxml2)

# Add source files
set(SOURCES
    src/main.cpp
    src/themes.cpp
)

# Add header files
set(HEADERS
    src/themes.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    raylib
    tinyxml2
)

# Configurer les chemins d'inclusion
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${raylib_SOURCE_DIR}/src
    ${raygui_SOURCE_DIR}/src
    ${tinyxml2_SOURCE_DIR}
    ${raylib_BINARY_DIR}
)

# Configuration spécifique pour macOS
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        "-framework Cocoa" 
        "-framework IOKit" 
        "-framework CoreAudio" 
        "-framework CoreVideo"
    )
endif()