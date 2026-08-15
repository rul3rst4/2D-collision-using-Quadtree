# AGENTS.md

## Cursor Cloud specific instructions

This is a single-file C++ project (`main.cpp`) that renders a 2D AABB collision
simulation of ~1000 particles using a quadtree, drawn with SFML 3. CMake
fetches and builds SFML 3.1.0 (Graphics/Window/System only).

### Dependencies
- `g++` (C++17) and `cmake` 3.28+ — preinstalled.
- SFML 3 Linux headers: `libxrandr-dev`, `libxcursor-dev`, `libxi-dev`,
  `libudev-dev`, `libgl1-mesa-dev`, `libfreetype-dev`, `libharfbuzz-dev`.
  `ninja-build` is optional but faster than Make.

Ubuntu 24.04's `libsfml-dev` package is still SFML 2.6, so do not use apt for
SFML itself.

### Build
```
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
The two `-Wnarrowing` warnings about `mt19937` seeding are harmless (existing code).

### Run (GUI)
This is a GUI app and needs an X display. The cloud VM exposes one on `:1`:
```
DISPLAY=:1 ./build/quadtree
```
It opens a "Quadtree collision" window and prints instantaneous FPS to stdout each
frame. The loop runs until the window is closed, so run it in the background /
a tmux session when you need the shell back. There are no automated tests or lint
config in this repo.
