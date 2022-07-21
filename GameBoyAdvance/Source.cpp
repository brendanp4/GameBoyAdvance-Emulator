#include <SDL.h>
#include "Game.h"
#include "Graphics.h"



int main(int argc, char *argv[])
{
	Game game;
	Graphics gfx;
	SDL_Event event;

	int count = 0;
	gfx.Init();


	while (1) {
		count += 1;


		game.Update(gfx);
		game.Draw(gfx);
		gfx.Render();

		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			break;
		}
	}


	SDL_Quit();

	return 0;
}