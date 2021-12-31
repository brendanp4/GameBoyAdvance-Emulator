#include "Keyboard.h"

void Keyboard::ProcessInput()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_w:
			key_W = true;
			break;
		case SDLK_a:
			key_A = true;
			break;
		case SDLK_s:
			key_S = true;
			break;
		case SDLK_d:
			key_D = true;
			break;
		case SDLK_r:
			key_R = true;
			break;
		case SDLK_t:
			key_T = true;
			break;
		case SDLK_LEFT:
			key_LEFT = true;
			break;
		case SDLK_RIGHT:
			key_RIGHT = true;
			break;
		case SDLK_UP:
			key_UP = true;
			break;
		case SDLK_DOWN:
			key_DOWN = true;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym)
		{
		case SDLK_w:
			key_W = false;
			break;
		case SDLK_a:
			key_A = false;
			break;
		case SDLK_s:
			key_S = false;
			break;
		case SDLK_d:
			key_D = false;
			break;
		case SDLK_r:
			key_R = false;
			break;
		case SDLK_t:
			key_T = false;
			break;
		case SDLK_LEFT:
			key_LEFT = false;
			break;
		case SDLK_RIGHT:
			key_RIGHT = false;
			break;
		case SDLK_UP:
			key_UP = false;
			break;
		case SDLK_DOWN:
			key_DOWN = false;
			break;
		default:
			break;
		}
		break;
	}
}
