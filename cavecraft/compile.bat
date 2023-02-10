@echo off
g++ main.cpp -lSDL2_image -lmingw32 -lSDL2main -lSDL2 -o release\program.exe
upx release\program.exe
release\program.exe
pause	