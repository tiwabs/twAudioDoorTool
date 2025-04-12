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

# Ajouter les fichiers sources
add_executable(${PROJECT_NAME} src/main.cpp)

# Lier les bibliothèques
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Configurer les chemins d'inclusion
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${raylib_SOURCE_DIR}/src
    ${raygui_SOURCE_DIR}/src
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