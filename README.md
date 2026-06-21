# Maze Pathfinding v1.0

A simple C++ maze pathfinding project with BFS and DFS algorithms.

## Features

- **BFS** (Breadth-First Search) — guaranteed shortest path
- **DFS** (Depth-First Search) — depth-first exploration
- **Handcrafted Maze** — preset 8x10 maze
- **Random Maze** — auto-generated using Prim's algorithm
- **ASCII Visualization** — no encoding issues, runs on any terminal

## Compile & Run

```bash
g++ -std=c++17 -o maze.exe main.cpp
./maze.exe
```

Or double-click `run.bat` to compile and run automatically.

## Maze Legend

| Symbol | Meaning |
|--------|---------|
| `S`    | Start (top-left) |
| `E`    | End (bottom-right) |
| `**`   | Path found |
| `##`   | Wall |
| `  `   | Open path |

## File Structure

```
maze/
  main.cpp    # Main program
  run.bat     # One-click compile & run (Windows)
  README.md   # This file
```
