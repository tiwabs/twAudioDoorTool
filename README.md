# twAudioDoorTool

Application utilisant Raylib et RayGui pour macOS.

## Prérequis

- macOS
- CMake (version 3.10 ou supérieure)
- Compilateur C++ compatible C++17 (clang++ recommandé)
- VSCode (recommandé)

## Installation

1. Clonez ce dépôt :
```
git clone https://github.com/votre-username/twAudioDoorTool.git
cd twAudioDoorTool
```

2. Ouvrez le projet dans VSCode :
```
code .
```

## Compilation et exécution

### Avec VSCode

1. Appuyez sur `Cmd+Shift+B` pour compiler le projet
2. Appuyez sur `F5` pour lancer l'application en mode debug

### En ligne de commande

```
mkdir -p build
cd build
cmake ..
make
./twAudioDoorTool
```

## Structure du projet

- `src/` : Code source de l'application
- `.vscode/` : Configuration VSCode
- `CMakeLists.txt` : Configuration CMake

## Fonctionnalités

- Interface graphique avec Raylib et RayGui
- Compatible macOS
- Téléchargement automatique des dépendances lors de la compilation 