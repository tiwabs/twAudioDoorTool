# twAudioDoorTool

A tool for managing audio door configurations in GTA V, built with Raylib and RayGui.

## Overview

twAudioDoorTool is a cross-platform application that allows you to manage and configure door audio settings for GTA V. It provides a user-friendly interface for creating, editing, and managing door sound presets.

## Prerequisites

- Windows 10/11 or macOS 10.15+
- CMake (version 3.10 or higher)
- C++17 compatible compiler (MSYS2 MinGW for Windows, clang++ for macOS)
- Visual Studio Code (recommended)

## Installation

1. Clone this repository:
```
git clone https://github.com/your-username/twAudioDoorTool.git
cd twAudioDoorTool
```

2. Open the project in Visual Studio Code:
```
code .
```

## Building and Running

### Using Visual Studio Code

1. Press `Ctrl+Shift+B` (Windows) or `Cmd+Shift+B` (macOS) to build the project
2. Press `F5` to run the application in debug mode

### Using Command Line

#### Windows (MSYS2):
```
mkdir build
cd build
cmake -G "MSYS Makefiles" ..
make
./twAudioDoorTool.exe
```

#### macOS:
```
mkdir -p build
cd build
cmake ..
make
./twAudioDoorTool
```

## Project Structure

- `src/` : Application source code
- `assets/` : Application assets and resources
- `build/` : Build output directory
- `CMakeLists.txt` : CMake configuration
- `build.bat` : Windows build script
- `build.sh` : macOS build script

## Features

- Modern GUI built with Raylib and RayGui
- Cross-platform compatibility (Windows and macOS)
- Theme customization with multiple built-in themes
- Sound preset management for door configurations
- Automatic dependency download during build process
- Settings persistence between sessions

## Development

The application is structured with the following main components:

- `main.cpp`: Main application entry point and UI
- `settings_manager.h/cpp`: Manages application settings and sound presets
- `themes.h/cpp`: Handles UI theme management

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Credits

Created by tiwabs 