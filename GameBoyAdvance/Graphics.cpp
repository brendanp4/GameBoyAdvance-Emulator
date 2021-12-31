#include "Graphics.h"

//void Graphics::imguiInit()
//{
//	// Setup Dear ImGui context
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//	
//	// Setup Dear ImGui style
//	ImGui::StyleColorsDark();
//	//ImGui::StyleColorsClassic();
//	
//	// Setup Platform/Renderer backends
//	ImGui_ImplSDL2_InitForSDLRenderer(window);
//	ImGui_ImplSDLRenderer_Init(renderer);
//}

//void Graphics::imguiFrame()
//{
//	
//
//	ImGui_ImplSDLRenderer_NewFrame();
//	ImGui_ImplSDL2_NewFrame(window);
//	ImGui::NewFrame();
//	
//	static float f = 0.0f;
//	static int counter = 0;
//
//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//	ImGui::Checkbox("Another Window", &show_another_window);
//
//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//		counter++;
//	ImGui::SameLine();
//	ImGui::Text("counter = %d", counter);
//
//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//	ImGui::End();
//}

void Graphics::PutPixel(int x, int y, int r, int g, int b)
{
	textureBuffer[ (y * 480) + x ] = 0xFF000000 | (r << 16) | (g << 8) | b;

}


int Graphics::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	window = SDL_CreateWindow("GameBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 320, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 480, 320);

}

void Graphics::Exit()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Graphics::Render()
{
	//SDL_LockTexture(texture, NULL, &address, &row);

	//int *textureBuffer = NULL;
	SDL_LockTexture(texture, NULL, (void **)&textureBuffer, &row);
	
	//for (int i = 0; i < 50; i++)
	//{
	//	for (int j = 0; j < 50; j++) {
	//		PutPixel(i, j, 255, 100, 30);
	//	}
	//}
	
	SDL_UnlockTexture(texture);
	
	
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	//ImGui::Render();
	//SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
	//SDL_RenderClear(renderer);
	//ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	//SDL_RenderPresent(renderer);

}
