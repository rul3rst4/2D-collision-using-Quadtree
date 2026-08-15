# AGENTS.md

## Cursor Cloud specific instructions

This is a single-file C++ project (`main.cpp`) that renders a 2D AABB collision
simulation of ~1000 particles using a quadtree, drawn with SFML. There is no
build system (no Makefile/CMake) and no package manager.

### Dependencies
- `g++` (C++17) — preinstalled.
- SFML dev library (`libsfml-dev`, provides `sfml-graphics`/`sfml-window`/`sfml-system`).
  The startup update script installs it via apt; it is the only external dependency.

### Build
```
g++ -std=c++17 main.cpp -o quadtree -lsfml-graphics -lsfml-window -lsfml-system
```
The two `-Wnarrowing` warnings about `mt19937` seeding are harmless (existing code).

### Run (GUI)
This is a GUI app and needs an X display. The cloud VM exposes one on `:1`:
```
DISPLAY=:1 ./quadtree
```
It opens a "Quadtree collision" window and prints instantaneous FPS to stdout each
frame. The loop runs until the window is closed, so run it in the background /
a tmux session when you need the shell back. There are no automated tests or lint
config in this repo.
