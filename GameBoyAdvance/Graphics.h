#pragma once
#include "SDL.h"
#include "imgui.h"
//#include "imgui_impl_sdl.h"
//#include "imgui_impl_sdlrenderer.h"
#include <cstdint>


class Graphics
{
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Texture *texture;

	//uint8_t screen[200 * 200 * 4];
	//bool show_demo_window = true;
	//bool show_another_window = true;
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



	//uint32_t *textureBuffer = new uint32_t[720 * 480];
	//
	//
	//void *address = &textureBuffer;
	//
	//int row = 2880;

	uint32_t *textureBuffer = new uint32_t[480 * 320];


	void *address = &textureBuffer;

	int row = 1920;


	int count = 0;
	int color = 50;

public:
	void PutPixel(int x, int y, int r, int g, int b);
	//void imguiInit();
	//void imguiFrame();
	int Init();
	void Exit();
	void Render();
};

