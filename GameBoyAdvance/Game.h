#pragma once
#include "Graphics.h"
#include <cstdint>
#include "Arm.h"
#include "Display.h"
#include "MMU.h"
#include "PPU.h"
#include "Keyboard.h"

class Game
{
private:
	Arm arm;
	Display display;
	MMU mmu;
	PPU ppu;
	Keyboard kbd;
	bool start = false;
	int count = 0;
	void SetBit(uint16_t& val, int bit) {
		val |= (1 << bit);
	}
	void ClearBit(uint16_t& val, int bit) {
		val &= ~(1 << bit);
	}

public:
	Game();
	void Draw(Graphics& gfx);
	void Update(Graphics& gfx);
	void ProcessInput();

};

