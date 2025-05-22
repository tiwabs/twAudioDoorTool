@echo off
setlocal enabledelayedexpansion

:: Check if MSYS2 is installed
if not exist "C:\msys64" (
    echo Error: MSYS2 is not installed in C:\msys64
    echo Please install MSYS2 from https://www.msys2.org/
    exit /b 1
)

:: Set explicit paths for tools
set "MINGW_PATH=C:\msys64\mingw64"
set "PATH=%MINGW_PATH%\bin;%PATH%"
set "CC=%MINGW_PATH%\bin\gcc.exe"
set "CXX=%MINGW_PATH%\bin\g++.exe"
set "CMAKE=%MINGW_PATH%\bin\cmake.exe"
set "MAKE=%MINGW_PATH%\bin\mingw32-make.exe"

:: Verify tools exist
if not exist "%CC%" (
    echo Error: GCC not found at %CC%
    echo Please run these commands in MSYS2 MinGW 64-bit:
    echo pacman -Syu
    echo pacman -S mingw-w64-x86_64-gcc
    exit /b 1
)

if not exist "%CMAKE%" (
    echo Error: CMake not found at %CMAKE%
    echo Please run this command in MSYS2 MinGW 64-bit:
    echo pacman -S mingw-w64-x86_64-cmake
    exit /b 1
)

if not exist "%MAKE%" (
    echo Error: MinGW32-make not found at %MAKE%
    echo Please run this command in MSYS2 MinGW 64-bit:
    echo pacman -S mingw-w64-x86_64-make
    exit /b 1
)

echo Cleaning build directory...
if exist build rd /s /q build
mkdir build
cd build

echo Configuring with CMake...
echo Using GCC: %CC%
echo Using G++: %CXX%
echo Using CMake: %CMAKE%
echo Using Make: %MAKE%

"%CMAKE%" -G "MinGW Makefiles" ^
    -DCMAKE_C_COMPILER="%CC%" ^
    -DCMAKE_CXX_COMPILER="%CXX%" ^
    -DCMAKE_MAKE_PROGRAM="%MAKE%" ^
    -DCMAKE_PREFIX_PATH="%MINGW_PATH%" ^
    -DCMAKE_BUILD_TYPE=Debug ^
    ..

if %ERRORLEVEL% neq 0 (
    echo.
    echo CMake configuration failed! Full error log:
    if exist CMakeFiles\CMakeError.log (
        type CMakeFiles\CMakeError.log
    ) else (
        echo CMakeError.log not found
    )
    cd ..
    exit /b 1
)

echo Building...
"%MAKE%" VERBOSE=1
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    cd ..
    exit /b 1
)

echo Build completed successfully!
cd ..

if exist "build\twAudioDoorTool.exe" (
    echo Running twAudioDoorTool...
    build\twAudioDoorTool.exe
) else (
    echo Warning: Executable not found in build directory
)