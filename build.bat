@echo off
echo Nettoyage du dossier build...
rd /s /q build
echo Creation du dossier build...
mkdir build
cd build
echo Configuration avec CMake...
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe ..
echo Compilation...
mingw32-make
echo.
if %ERRORLEVEL% == 0 (
    echo Build termine avec succes !
    echo L'executable se trouve dans le dossier build/
) else (
    echo Erreur pendant la compilation !
)
cd ..

cd build
twAudioDoorTool.exe