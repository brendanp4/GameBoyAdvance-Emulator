#pragma once
#include <cstdint>
#include "MMU.h"
#include "Display.h"



class PPU
{
private:
	struct PixelPlacement
	{
		PixelPlacement(uint16_t color, int loc, int bg, int priority, int x, int y);
		uint16_t color = 0;
		int pLoc = 0;
		int bg = 0;
		int priority = 0;
		int x = 0;
		int y = 0;
		bool win0 = false;
		bool win1 = false;
		bool winout = false;
	};
public:
	std::vector<PixelPlacement> frame;
	PPU();
	void Update(MMU& mmu, Display& display);
	//uint16_t DISPCNT = 0;
	//uint16_t DISPSTAT = 0;
	//uint16_t VCOUNT = 0;
	//uint16_t BG0CNT	= 0;
	//uint16_t BG1CNT	= 0;
	//uint16_t BG2CNT	= 0;
	//uint16_t BG3CNT	= 0;
	//uint16_t BG0HOFS = 0;
	//uint16_t BG0VOFS = 0;
	//uint16_t BG1HOFS = 0;
	//uint16_t BG1VOFS = 0;
	//uint16_t BG2HOFS = 0;
	//uint16_t BG2VOFS = 0;
	//uint16_t BG3HOFS = 0;
	//uint16_t BG3VOFS = 0;
	//
	//
	//uint16_t WIN0H  = 0;
	//uint16_t WIN1H  = 0;
	//uint16_t WIN0V  = 0;
	//uint16_t WIN1V  = 0;
	//uint16_t WININ  = 0;
	//uint16_t WINOUT = 0;



//private:
	int bgPriorities[240] = {0};
	int bgColorsB[240] = {0};
	void ClearFrame();
	void DetermineColor(Display& display);
	void SetPixel(Display& display, int pixel, uint16_t color);
	void SetPixel(Display& display, int pixel, int r, int g, int b);
	void PushColor(uint16_t color, int loc, int bg, int priority, int x, int y);
	bool access = false;


	void LoadScanline(MMU& mmu);
	void DrawScanline(Display& display, MMU& mmu);
	void DrawFrame();
	void SetBit(uint16_t& val, int bit);
	void ClearBit(uint16_t& val, int bit);
	void LoadBgBuffer1(MMU& mmu);
	void LoadSprites(MMU& mmu, Display& display, int spriteNum, int p);
	std::vector<uint16_t> bgBuffer;
	std::vector<uint16_t> tileMap;
	//uint16_t Sprite[128][128];
	std::vector< std::vector<uint16_t> > Sprite;

	uint16_t backdrop_color = 0;

	std::vector<uint16_t> bg0ScanlineBuffer;


	std::vector< std::vector<uint16_t> > bg0TileBuffer;
	std::vector< std::vector<uint16_t> > bg1TileBuffer;
	std::vector< std::vector<uint16_t> > bg2TileBuffer;
	std::vector< std::vector<uint16_t> > bg3TileBuffer;

	//using grid = std::array<std::array<uint16_t, 512>, 512>;
	//grid * ary = new grid;

	//auto array = new double[10][10];

	//uint16_t bg0TileBuffer[514][256];
	//uint16_t bg1TileBuffer[256][256];
	//uint16_t bg2TileBuffer[256][256];
	bool fOnce = false;
	//Dispcnt
	int VideoMode = 0;
	bool pageFlip = false;
	bool vram2d = false;
	bool forceBlank = false;
	bool bg0 = false;
	bool bg1 = false;
	bool bg2 = false;
	bool bg3 = false;
	bool obj = false;
	bool win0 = false;
	bool win1 = false;
	bool winObj = false;

	//Dispstat
	bool vBlank = false;
	bool hBlank = false;
	bool vCount = false;
	bool vBlankIrq = false;
	bool hBlankIrq = false;
	bool vCountIrq = false;
	uint16_t vCountSetting = 0;

	//Vcount
	uint16_t curScanLine = 0;

	//Interrupt
	bool vBlankEntered = false;

	//BG0CNT
	int bg0Priority = 0;
	int bg0CharBase = 0;
	bool bg0Mosaic = false;
	bool bg0cSize = false; //(0=16/16, 1=256/1)
	int bg0ScreenBase = 0;
	int bg0ScreenSize = 0;

	//BG1CNT
	int bg1Priority = 0;
	int bg1CharBase = 0;
	bool bg1Mosaic = false;
	bool bg1cSize = false; //(0=16/16, 1=256/1)
	int bg1ScreenBase = 0;
	int bg1ScreenSize = 0;

	//BG2CNT
	int  bg2Priority = 0;
	int  bg2CharBase = 0;
	bool bg2Mosaic = false;
	bool bg2cSize = false; //(0=16/16, 1=256/1)
	int  bg2ScreenBase = 0;
	int  bg2ScreenSize = 0;

	//BG3CNT
	int  bg3Priority = 0;
	int  bg3CharBase = 0;
	bool bg3Mosaic = false;
	bool bg3cSize = false; //(0=16/16, 1=256/1)
	int  bg3ScreenBase = 0;
	int  bg3ScreenSize = 0;

	//WININ
	bool Win0BG0 = false;
	bool Win0BG1 = false;
	bool Win0BG2 = false;
	bool Win0BG3 = false;
	bool Win0OBJ = false;
	bool Win0Col = false;

	bool Win1BG0 = false;
	bool Win1BG1 = false;
	bool Win1BG2 = false;
	bool Win1BG3 = false;
	bool Win1OBJ = false;
	bool Win1Col = false;

	//WINOUT
	bool WinOut0BG0 = false;
	bool WinOut0BG1 = false;
	bool WinOut0BG2 = false;
	bool WinOut0BG3 = false;
	bool WinOut0OBJ = false;
	bool WinOut0Col = false;

	bool WinOut1BG0 = false;
	bool WinOut1BG1 = false;
	bool WinOut1BG2 = false;
	bool WinOut1BG3 = false;
	bool WinOut1OBJ = false;
	bool WinOut1Col = false;

	//GFX effects
	uint16_t BLDCNT   = 0;
	uint16_t BLDALPHA = 0;
	uint16_t BLDY 	  = 0;


	bool bg0Target1 = false;
	bool bg1Target1 = false;
	bool bg2Target1 = false;
	bool bg3Target1 = false;
	bool objTarget1 = false;
	bool bdTarget1 = false;

	bool bg0Target2 = false;
	bool bg1Target2 = false;
	bool bg2Target2 = false;
	bool bg3Target2 = false;
	bool objTarget2 = false;
	bool bdTarget2 = false;
	int colorEffect = 0;

	int EVA = 0;
	int EVB = 0;

	int EVY = 0;


	uint32_t bitrange(int msb, int lsb, uint16_t insn);


	void Mode0Render(MMU& mmu, Display& display);

	void readDispcnt(MMU& mmu);
	void readDispstat(MMU& mmu);
	void readVcount(MMU& mmu);
	void readBg0Cnt(MMU& mmu);
	void readBg1Cnt(MMU& mmu);
	void readBg2Cnt(MMU& mmu);
	void readBg3Cnt(MMU& mmu);
	void readWinIn(MMU& mmu);
	void readWinOut(MMU& mmu);
	void readGFXEffects(MMU& mmu);

	bool yaga = false;
	int incr = 0;


};

