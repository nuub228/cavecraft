@echo off

mkdir releasex64
g++ main.cpp -I"sdl2\include" -L"sdl2\lib" -lSDL2_image -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows -o releasex64\program.exe


pause	


