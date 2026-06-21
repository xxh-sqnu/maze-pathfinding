@echo off
title Maze Pathfinding
echo.
echo ====================================
echo   Compiling Maze Pathfinding...
echo ====================================
echo.

g++ -std=c++17 -o maze.exe main.cpp 2>nul
if %errorlevel% equ 0 (
    echo [OK] Compile succeeded! Running...
    echo.
    maze.exe
    goto :end
)

echo [FAIL] g++ not found.
echo.
echo Please install MinGW-w64:
echo   https://winlibs.com/
echo.
echo Or open main.cpp in your IDE (Dev-C++ / VS Code / Visual Studio).

:end
pause
