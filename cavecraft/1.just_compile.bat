@echo off

mkdir releasex64
"gcc-15.2.0-gdb-16.3.90.20250511-binutils-2.45-mingw-w64-v13.0.0-ucrt\bin\g++.exe" main.cpp -I"sdl2\include" -L"sdl2\lib" -lSDL2_image -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows -o releasex64\program.exe


pause	
