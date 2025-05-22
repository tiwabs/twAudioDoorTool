# twAudioDoorTool

A tool for managing audio door configurations in GTA V, built with Dear ImGui.

## Overview

twAudioDoorTool is a cross-platform application that allows you to manage and configure door audio settings for GTA V. It provides a modern user interface for creating, editing, and managing door sound presets.

## Prerequisites

- Windows 10/11 or macOS 10.15+
- CMake (version 3.10 or higher)
- C++17 compatible compiler (MSYS2 MinGW for Windows, clang++ for macOS)
- Visual Studio Code (recommended)

### Windows Setup (MSYS2)

1. Install MSYS2 from https://www.msys2.org/
2. During installation, keep the default path (C:\msys64)
3. After installation, open "MSYS2 MINGW64" terminal (not the regular MSYS2 terminal)
4. Update the package database and base packages:

```bash
pacman -Syu
```

5. Close the terminal when prompted and reopen "MSYS2 MINGW64"
6. Install required packages:

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-make
pacman -S mingw-w64-x86_64-glfw
```

7. Add MSYS2 to your system's PATH:
   - Open Windows System Properties (Win + Pause/Break)
   - Click "Advanced system settings"
   - Click "Environment Variables"
   - Under "System variables", find and select "Path"
   - Click "Edit"
   - Click "New"
   - Add `C:\msys64\mingw64\bin`
   - Click "OK" on all windows
8. Restart your terminal/Visual Studio Code

### macOS Setup

Using Homebrew:

```bash
brew install cmake
brew install glfw
```

## Dependencies (automatically managed)

- Dear ImGui v1.91.9
- nlohmann/json v3.11.2
- pugixml v1.13
- ImGuiFileDialog

## Installation

1. Clone this repository:

```bash
git clone https://github.com/tiwabs/twAudioDoorTool.git
cd twAudioDoorTool
```

2. Open the project in Visual Studio Code:

```bash
code .
```

## Building and Running

### Using Visual Studio Code

1. Press `Ctrl+Shift+B` (Windows) or `Cmd+Shift+B` (macOS) to build the project
2. Press `F5` to run the application in debug mode

### Using Command Line

#### Windows (MSYS2):

```bash
mkdir build
cd build
cmake -G "MSYS Makefiles" ..
make
./twAudioDoorTool.exe
```

#### macOS:

```bash
mkdir -p build
cd build
cmake ..
make
./twAudioDoorTool
```

## Troubleshooting

### Common Issues

1. **Compiler not found error**:

   - Make sure you've followed all MSYS2 installation steps above
   - Verify that `C:\msys64\mingw64\bin` is in your system's PATH
   - Try running the commands in a new terminal session
   - Run `gcc --version` to verify the compiler is accessible

2. **CMake configuration fails**:

   - Use "MinGW Makefiles" generator: `cmake -G "MinGW Makefiles" ..`
   - Make sure you're using the MINGW64 version of CMake
   - Clear your build directory and try again
   - Check that all required packages are installed

3. **GLFW3 not found error**:

   - Verify GLFW is installed: `pacman -Qs glfw`
   - Try reinstalling: `pacman -S mingw-w64-x86_64-glfw`
   - Make sure you're using the MINGW64 terminal

4. **Build script fails**:
   - Make sure you're running build.bat from a regular Command Prompt or PowerShell
   - Verify all MSYS2 packages are up to date
   - Check that you can run gcc, cmake, and mingw32-make from the terminal

## Project Structure

- `src/` : Application source code
  - `components/` : UI components
- `assets/` : Application resources
- `libs/` : External libraries
- `build/` : Build output directory
- `CMakeLists.txt` : CMake configuration
- `build.bat` : Windows build script
- `build.sh` : macOS build script

## Features

- Modern GUI built with Dear ImGui
- Cross-platform compatibility (Windows and macOS)
- Sound preset management for door configurations
- Settings persistence between sessions
- XML format support for configurations
- Integrated file selection dialog

## Development

The application is structured with the following main components:

- Dear ImGui-based user interface
- JSON file management for settings
- XML parsing with pugixml
- File handling with ImGuiFileDialog

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Credits

Created by tiwabs
