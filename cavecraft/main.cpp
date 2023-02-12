#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>


#include "classmain.cpp"

#define X 255
#define Y 127


	

int main(int argc, char* argv[])
{

	Main _vars;
    _vars.loadtexture();
    _vars.world.seed.ssrand(6645);
    _vars.newworld();
    _vars.velx = 2;
    _vars.acc = 1;
    _vars.x = 2048;
    _vars.y = _vars.world.getply();
	
    while (_vars.gamerunning) {
		_vars.proc_fps();
		_vars.keyhandler();
		_vars.phyhandler();
		_vars.game();
		
        _vars.draw();//доделать!
        _vars.maxfps();
		SDL_UpdateWindowSurface(_vars.window);
        SDL_Delay(1);
		
    }

    SDL_DestroyWindow(_vars.window);
    return 0;
};