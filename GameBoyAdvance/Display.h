#pragma once
#include "Graphics.h"

class Display
{
private:
	class Pixel
	{
	public:
		static constexpr int size = 2;
		void DrawPixel(Graphics& gfx, int x, int y);
		int r = 0;
		int g = 0;
		int b = 0;
	};

public:
	bool IsFilled(int pixel);
	void Draw(Graphics& gfx);
	void SetPixel(int pixel, int r, int g, int b);
	void SetPixel(int pixel, uint16_t color);
private:
	static constexpr int width = 240;
	static constexpr int height = 160;
	int offsetX = 40;
	int offsetY = 60;

	Pixel pixels[width * height];
	Pixel& PixelAt(int x, int y);
};

