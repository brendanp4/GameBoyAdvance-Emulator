#include "Display.h"

void Display::Pixel::DrawPixel(Graphics& gfx, int x, int y)
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			gfx.PutPixel(x + i, y + j, r, g, b);
		}
	}
}

bool Display::IsFilled(int pixel)
{
	if (pixels[pixel].r != 0 || pixels[pixel].g != 0 || pixels[pixel].b != 0) {
		return true;
	}
	else
	{
		return false;
	}
}

void Display::Draw(Graphics& gfx)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			PixelAt(i, j).DrawPixel(gfx, (i * Pixel::size), (j * Pixel::size));
		}
	}
}

void Display::SetPixel(int pixel, int r, int g, int b)
{
	pixels[pixel].r = r;
	pixels[pixel].g = g;
	pixels[pixel].b = b;
}

void Display::SetPixel(int pixel, uint16_t color)
{
	pixels[pixel].b = ((color & 0x7C00) >> 10) * 8;
	pixels[pixel].g = ((color & 0x3E0) >> 5) * 8;
	pixels[pixel].r = (color & 0x1F) * 8;
}

Display::Pixel & Display::PixelAt(int x, int y)
{
	return pixels[y * width + x];
}
