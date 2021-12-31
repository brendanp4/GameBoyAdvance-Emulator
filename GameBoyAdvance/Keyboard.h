#pragma once
#include "SDL.h"

class Keyboard
{
private:
	SDL_Event event;
public:
	void ProcessInput();
	bool key_W = false;
	bool key_A = false;
	bool key_S = false;
	bool key_D = false;
	bool key_R = false;
	bool key_T = false;
	bool key_UP	= false;
	bool key_DOWN = false;
	bool key_LEFT = false;
	bool key_RIGHT = false;

};

