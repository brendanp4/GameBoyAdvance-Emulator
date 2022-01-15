#include "PPU.h"
#include <algorithm>

PPU::PPU()
{
	bgBuffer.resize(38400, 0);
	tileMap.resize(2048, 0);
	bg0ScanlineBuffer.resize(240, 0);
	bg0TileBuffer.resize(512, std::vector<uint16_t>(512, 0));
	bg1TileBuffer.resize(512, std::vector<uint16_t>(512, 0));
	bg2TileBuffer.resize(512, std::vector<uint16_t>(512, 0));
	bg3TileBuffer.resize(512, std::vector<uint16_t>(512, 0));
}

void PPU::Update(MMU& mmu, Display& display)
{
	readDispcnt(mmu);
	readDispstat(mmu);
	readVcount(mmu);
	readGFXEffects(mmu);
	mmu.VCOUNT++;

	if (mmu.VCOUNT == 160) {
		vBlankEntered = true;
	}
	else
	{
		vBlankEntered = false;
	}

	if (forceBlank) {
		vBlank = true;
		if (!fOnce) {
			mmu.VCOUNT = 159;
			fOnce = true;
		}
		SetBit(mmu.DISPSTAT, 0);
		if (mmu.VCOUNT == 227) {
			mmu.VCOUNT = 159;
		}
	}
	else
	{
		fOnce = false;
		if (mmu.VCOUNT > 159 && mmu.VCOUNT < 227) {
			SetBit(mmu.DISPSTAT, 0);
		}
		else
		{
			ClearBit(mmu.DISPSTAT, 0);
		}
		if (mmu.VCOUNT == 227) {
			mmu.VCOUNT = 0;
			curScanLine = 0;
		}

		if (VideoMode == 0) {
			if (mmu.VCOUNT == 162) {
				readWinIn(mmu);
				readWinOut(mmu);
				Mode0Render(mmu, display);
			}
			if (mmu.VCOUNT < 161) {
				//LoadScanline(mmu);
				//for (int i = 0; i < 240; i++) {
				//	int pos = (curScanLine * 240) + i;
				//	int b = bitrange(14, 10, bg0ScanlineBuffer[i]) * 8;
				//	int g = bitrange(9, 5, bg0ScanlineBuffer[i]) * 8;
				//	int r = bitrange(4, 0, bg0ScanlineBuffer[i]) * 8;
				//	display.SetPixel(pos, r,g,b);
				//}

				if (!bg0 && !bg1 && !bg2 && !bg3) {
					//Set backdrop color
					for (int i = 0; i < 240; i++) {
						int pos = (curScanLine * 240) + i;
						uint16_t color = mmu.BPRAM[1] << 8 | mmu.BPRAM[0];
						//int b = bitrange(14, 10, color) * 8;
						//int g = bitrange(9, 5, color) * 8;
						//int r = bitrange(4, 0, color) * 8;
						//display.SetPixel(pos, r, g, b);
						display.SetPixel(pos, color);
					}
				}
				else
				{
					DrawScanline(display, mmu);
				}
			}

			//readBg0Cnt(mmu);
			//Mode0Render(mmu, display);

		}

		if (VideoMode == 2) {
			if (mmu.VCOUNT < 161) {
				DrawScanline(display, mmu);
			}
		}

		if (VideoMode == 3) {
			if (bg2) {
				if (curScanLine < 160) {
					DrawScanline(display, mmu);
				}
				if (mmu.VCOUNT == 161) {
					LoadBgBuffer1(mmu);
				}
			}
		}
		if (VideoMode == 4) {
			if (bg2) {
				if (curScanLine < 160) {
					DrawScanline(display, mmu);
				}
				if (mmu.VCOUNT == 161) {
					LoadBgBuffer1(mmu);
				}
			}
		}
	}
	if (obj) {
		for (int i = 96; i >= 0; i--) {
			LoadSprites(mmu, display, i, 0);
		}
		
	}
	for (int i = 0; i < 240; i++)
	{
		bgPriorities[i] = 5;
		bgColorsB[i] = 0;
	}

	hBlank = true;

}

void PPU::ClearFrame()
{
	frame.clear();
}

void PPU::DetermineColor(Display& display)
{
	int layerA = -1;
	int layerB = -1;
	int lowestPriority = 5;
	uint16_t color = 0;
	uint16_t colorA = 0;
	uint16_t colorB = 0;
	int location = 0;
	if (frame.size() >= 1) {
		int xLoc = frame[0].x;
		//if (frame[0].pLoc == 34586) {
		//	SetPixel(display, location, 0xDF13);
		//}

		SetPixel(display, frame[0].pLoc, frame[0].color);

		//uint16_t bdColor = mmu.BPRAM[1] << 8 | mmu.BPRAM[0];

		bool found = false;
		//for (int i = frame.size() - 1; i >= 0; i--) {
		//	if (frame[i].color) {
		//		if (frame[i].pLoc == 4370) {
		//			int y = 0;
		//		}
		//		//if (!(frame[i].color >> 15)) {
		//		//	if (frame[i].bg == 0 && bg0Target1) {
		//		//		colorA = frame[i].color;
		//		//	}
		//		//	if (frame[i].bg == 1 && bg1Target1) {
		//		//		colorA = frame[i].color;
		//		//	}
		//		//}
		//
		//
		//		//if (!((frame[i].color >> 15) & 1) && found) {
		//		//	if (frame[i].priority <= lowestPriority)
		//		//	{
		//		//		if ((BLDCNT >> (frame[i].bg + 8)) & 1) {
		//		//			colorB = colorA;
		//		//			colorA = frame[i].color;
		//		//		}
		//		//		lowestPriority = frame[i].priority;
		//		//		location = frame[i].pLoc;
		//		//		color = frame[i].color;
		//		//		//SetPixel(display, frame[i].pLoc, frame[i].color);
		//		//	}
		//		//}
		//		//if (!((frame[i].color >> 15) & 1) && !found) {
		//		//	found = true;
		//		//	if ((BLDCNT >> frame[i].bg) & 1) {
		//		//		colorA = frame[i].color;
		//		//	}
		//		//	lowestPriority = frame[i].priority;
		//		//	location = frame[i].pLoc;
		//		//	color = frame[i].color;
		//		//	//SetPixel(display, frame[i].pLoc, frame[i].color);
		//		//}
		//
		//
		//		if (!((frame[i].color >> 15) & 1)) {
		//			if (frame[i].priority <= lowestPriority) {
		//				if ((BLDCNT >> frame[i].bg) & 1) {
		//					colorA = frame[i].color;
		//				}
		//				lowestPriority = frame[i].priority;
		//				location = frame[i].pLoc;
		//				color = frame[i].color;
		//			}
		//		}
		//	}
		//}

		for (size_t i = 0; i < frame.size(); i++)
		{
			//if (frame[i].color) {
				if (!((frame[i].color >> 15) & 1)) {
					if (frame[i].priority <= lowestPriority) {
						if ((BLDCNT >> frame[i].bg) & 1) {
							colorA = frame[i].color;
						}
						if ((BLDCNT >> (frame[i].bg + 8)) & 1) {
							colorB = frame[i].color;
							bgColorsB[xLoc] = frame[i].color;
						}
						layerB = layerA;
						layerA = i;
						lowestPriority = frame[i].priority;
						location = frame[i].pLoc;
						color = frame[i].color;
					}
				}
			//}
		}

		switch (colorEffect)
		{
		case 0:
			// No Effect
			SetPixel(display, location, color);
			break;
		case 1:
		{
			// Alpha Blend
			if (colorA && colorB) {
				int redA = (colorA & 0x1F);
				int greenA = ((colorA & 0x3E0) >> 5);
				int blueA = ((colorA & 0x7C00) >> 10);
				if (EVA == 0) {
					redA = greenA = blueA = 0;
				}
				else
				{
					redA   = float(redA) * (float(EVA) / 16);
					greenA = float(greenA) * (float(EVA) / 16);
					blueA  = float(blueA) * (float(EVA) / 16);
				}

				int redB = (colorB & 0x1F);
				int greenB = ((colorB & 0x3E0) >> 5);
				int blueB = ((colorB & 0x7C00) >> 10);
				if (EVB == 0) {
					redB = greenB = blueB = 0;
				}
				else
				{
					redB = float(redB) * (float(EVB) / 16);
					greenB = float(greenB) * (float(EVB) / 16);
					blueB = float(blueB) * (float(EVB) / 16);
				}

				int red = redA + redB;
				red *= 8;
				if (red > 255) {
					red = 255;
				}
				int green = greenA + greenB;
				green *= 8;
				if (green > 255) {
					green = 255;
				}
				int blue = blueA + blueB;
				blue *= 8;
				if (blue > 255) {
					blue = 255;
				}
				SetPixel(display, location, red, green, blue);
			}
			else
			{
				SetPixel(display, location, color);
			}

			break;
		}
		case 2:
			// Fade to white
		{
			if (colorA) {
				int redA = (colorA & 0x1F);
				int greenA = ((colorA & 0x3E0) >> 5);
				int blueA = ((colorA & 0x7C00) >> 10);

				float EVZ = 16 - EVY;
				redA = float(redA) *     (EVZ / 16);
				greenA = float(greenA) * (EVZ / 16);
				blueA = float(blueA) *   (EVZ / 16);
				

				int redB =   31;
				int greenB = 31;
				int blueB =  31;

				redB = float(redB) *     (float(EVY) / 16);
				greenB = float(greenB) * (float(EVY) / 16);
				blueB = float(blueB) *   (float(EVY) / 16);

				int red = redA + redB;
				red *= 8;
				if (red > 255) {
					red = 255;
				}
				int green = greenA + greenB;
				green *= 8;
				if (green > 255) {
					green = 255;
				}
				int blue = blueA + blueB;
				blue *= 8;
				if (blue > 255) {
					blue = 255;
				}
				SetPixel(display, location, red, green, blue);
			}
			else
			{
				SetPixel(display, location, color);
			}
		}
			break;
		case 3:
			// Fade to black
		{
			if (colorA) {
				int redA = (colorA & 0x1F);
				int greenA = ((colorA & 0x3E0) >> 5);
				int blueA = ((colorA & 0x7C00) >> 10);

				float EVZ = 16 - EVY;
				redA = float(redA) *     (EVZ / 16);
				greenA = float(greenA) * (EVZ / 16);
				blueA = float(blueA) *   (EVZ / 16);


				int redB =   0;
				int greenB = 0;
				int blueB =  0;

				redB = float(redB) *     (float(EVY) / 16);
				greenB = float(greenB) * (float(EVY) / 16);
				blueB = float(blueB) *   (float(EVY) / 16);

				int red = redA + redB;
				red *= 8;
				if (red > 255) {
					red = 255;
				}
				int green = greenA + greenB;
				green *= 8;
				if (green > 255) {
					green = 255;
				}
				int blue = blueA + blueB;
				blue *= 8;
				if (blue > 255) {
					blue = 255;
				}
				SetPixel(display, location, red, green, blue);
			}
			else
			{
				SetPixel(display, location, color);
			}
		}

			break;
		default:
			SetPixel(display, location, color);

			break;
		}
		//if (frame[0].pLoc == 34586) {
		//	SetPixel(display, location, 0xDF13);
		//}
		bgPriorities[xLoc] = lowestPriority;
	}
}

void PPU::SetPixel(Display & display, int pixel, uint16_t color)
{
	display.SetPixel(pixel, color);
}

void PPU::SetPixel(Display & display, int pixel, int r, int g, int b)
{
	display.SetPixel(pixel, r, g, b);
}

void PPU::PushColor(uint16_t color, int loc, int bg, int priority, int x, int y)
{
	frame.emplace_back(color, loc, bg, priority, x, y);
}


void PPU::LoadScanline(MMU & mmu)
{
	if (bg0) {

		readBg0Cnt(mmu);

		bg0ScreenBase = (bg0ScreenBase * 0x800);
		bg0CharBase = (bg0CharBase * 0x4000);
		switch (bg0ScreenSize)
		{
		case 0:
			tileMap.resize(1024, 0);
			break;
		case 1:
			tileMap.resize(2048, 0);
			break;
		case 2:
			tileMap.resize(2048, 0);
			break;
		case 3:
			tileMap.resize(4096, 0);
			break;
		}

		if (curScanLine == 11) {
			int y = 0;
		}

		int start = (curScanLine / 8) * 32;


		for (int i = start; i < start + 32; i++)
		{
			// Tile map is index
			tileMap[i] = mmu.VRAM[bg0ScreenBase + (i * 2) + 1] << 8 | mmu.VRAM[bg0ScreenBase + (i * 2)];
			int pBank = ((tileMap[i]) >> 12);
			bool hFlip = false;
			bool vFlip = false;
			if (tileMap[i] >> 10 & 1) {
				hFlip = true;
			}
			if (tileMap[i] >> 11 & 1) {
				vFlip = true;
			}

			if (bg0cSize) {
				int location = (bitrange(9, 0, tileMap[i]) * 64) + bg0CharBase;
				for (int j = 0; j < 64; j++) {

					int colorLocation = (mmu.VRAM[location + j] * 2);

					uint16_t color = mmu.BPRAM[colorLocation + 1] << 8 | mmu.BPRAM[colorLocation];

					int b = bitrange(14, 10, color) * 8;
					int g = bitrange(9, 5, color) * 8;
					int r = bitrange(4, 0, color) * 8;

					// Top left corner coordinates of tile:
					int hOffset = (i % 32) * 8;
					int vOffset = (i / 32) * 8;
					int initPos = (vOffset * 240) + hOffset;
					int ye = j % 8;
					int ya = j / 8;

					int posX = 0;
					int posY = 0;

					if (i < 1024) {
						posX = hOffset + ye;
						posY = vOffset + ya;
					}
					else if (i < 2048) {
						posX = hOffset + ye + 256;
						posY = (vOffset + ya) % 256;
					}
					else if (i < 3072) {
						posX = ((hOffset + ye));
						posY = ((vOffset + ya) + 256) % 512;
					}
					else if (i < 4096) {
						posX = ((hOffset + ye) + 256);
						posY = ((vOffset + ya) + 256) % 512;
					}

					bg0TileBuffer[posX][posY] = color;
				}
			}
			else
			{
				int location = (bitrange(9, 0, tileMap[i]) * 32) + bg0CharBase;
				for (int j = 0; j < 32; j++) {
					int index = ((location + j));
					int drawDot = j * 2;
					int colorLocation = (mmu.VRAM[index]);

					uint16_t colorLeftIndex = (bitrange(3, 0, mmu.VRAM[index]) * 2) + (pBank * 32);
					uint16_t colorRightIndex = (bitrange(7, 4, mmu.VRAM[index]) * 2) + (pBank * 32);

					uint16_t colorLeft = mmu.BPRAM[colorLeftIndex + 1] << 8 | mmu.BPRAM[colorLeftIndex];
					uint16_t colorRight = mmu.BPRAM[colorRightIndex + 1] << 8 | mmu.BPRAM[colorRightIndex];



					if (colorLeftIndex % 32 == 0) {

						//Use backdrop color
						colorLeft = mmu.BPRAM[1] << 8 | mmu.BPRAM[0];
						//SetBit(colorLeft, 15);
					}
					else
					{
						ClearBit(colorLeft, 15);
					}
					if (colorRightIndex % 32 == 0) {

						//Use backdrop color
						colorRight = mmu.BPRAM[1] << 8 | mmu.BPRAM[0];
						//SetBit(colorRight, 15);
					}
					else
					{
						ClearBit(colorRight, 15);
					}

					// Top left corner coordinates of tile:


					int vOffset = (i / 32) * 8;				//Column

					int hOffset = (i - start) * 8;



					int ye = drawDot % 8;						//Horizontal position inside tile
					int ya = (drawDot / 8);						//Vertical position inside tile

					if (vFlip) {
						ya = abs(ya - 7);
					}

					int posX = 0;
					int posY = 0;

					if (i < 1024) {
						posX = hOffset + ye;
						posY = vOffset + ya;
					}
					else if (i < 2048) {
						posX = hOffset + ye + 256;
						posY = (vOffset + ya) % 256;
					}
					else if (i < 3072) {
						posX = ((hOffset + ye));
						posY = ((vOffset + ya) + 256) % 512;
					}
					else if (i < 4096) {
						posX = ((hOffset + ye) + 256);
						posY = ((vOffset + ya) + 256) % 512;
					}


					if (posY == curScanLine && posX < 240) {
						bg0ScanlineBuffer[posX] = colorLeft;
					}


					drawDot++;


					// Top left corner coordinates of tile:
					hOffset = ((i % 32) * 8);
					vOffset = (((i % 1024) / 32) * 8);
					ye = drawDot % 8;
					ya = (drawDot / 8);
					if (vFlip) {
						ya = abs(ya - 7);
					}
					if (i < 1024) {
						posX = hOffset + ye;
						posY = vOffset + ya;
					}
					else if (i < 2048) {
						posX = hOffset + ye + 256;
						posY = (vOffset + ya) % 256;
					}
					else if (i < 3072) {
						posX = ((hOffset + ye));
						posY = ((vOffset + ya) + 256) % 512;
					}
					else if (i < 4096) {
						posX = ((hOffset + ye) + 256) % 512;
						posY = ((vOffset + ya) + 256) % 512;
					}

					if (posY == curScanLine && posX < 240) {
						bg0ScanlineBuffer[posX] = colorRight;
					}

				}
			}

		}
	}





}

void PPU::DrawScanline(Display& display, MMU& mmu)
{
	ClearBit(mmu.DISPSTAT, 1);
	hBlank = false;
	bool first = false;
	if (VideoMode == 0) {
		if (!win0 && !win1 && !winObj) {
			for (int i = 0; i < 240; i++) {
				//uint16_t bdColor = mmu.BPRAM[3] << 8 | mmu.BPRAM[2];
				//int cur = (curScanLine * 240) + i;
				//SetPixel(display, cur, bdColor);
				if (bg3) {
					int color = 0;
					first = true;
					switch (bg3ScreenSize)
					{
					case 0:
						color = bg3TileBuffer[(i + mmu.BG3HOFS) % 256][(curScanLine + mmu.BG3VOFS) % 256];
						break;
					case 1:
						color = bg3TileBuffer[(i + mmu.BG3HOFS) % 512][(curScanLine + mmu.BG3VOFS) % 256];
						break;
					case 2:
						color = bg3TileBuffer[(i + mmu.BG3HOFS) % 256][(curScanLine + mmu.BG3VOFS) % 512];
						break;
					case 3:
						color = bg3TileBuffer[(i + mmu.BG3HOFS) % 512][(curScanLine + mmu.BG3VOFS) % 512];
						break;
					}
					int cur = (curScanLine * 240) + i;
					PushColor(color, cur, 3, bg3Priority, i, curScanLine);
				}
				if (bg2) {
					int color = 0;
					first = true;
					switch (bg2ScreenSize)
					{
					case 0:
						color = bg2TileBuffer[(i + mmu.BG2HOFS) % 256][(curScanLine + mmu.BG2VOFS) % 256];
						break;
					case 1:
						color = bg2TileBuffer[(i + mmu.BG2HOFS) % 512][(curScanLine + mmu.BG2VOFS) % 256];
						break;
					case 2:
						color = bg2TileBuffer[(i + mmu.BG2HOFS) % 256][(curScanLine + mmu.BG2VOFS) % 512];
						break;
					case 3:
						color = bg2TileBuffer[(i + mmu.BG2HOFS) % 512][(curScanLine + mmu.BG2VOFS) % 512];
						break;
					}
					int cur = (curScanLine * 240) + i;
					PushColor(color, cur, 2, bg2Priority, i, curScanLine);
				}
				if (bg1) {
					int color = 0;
					first = true;
					switch (bg1ScreenSize)
					{
					case 0:
						color = bg1TileBuffer[(i + mmu.BG1HOFS) % 256][(curScanLine + mmu.BG1VOFS) % 256];
						break;
					case 1:
						color = bg1TileBuffer[(i + mmu.BG1HOFS) % 512][(curScanLine + mmu.BG1VOFS) % 256];
						break;
					case 2:
						color = bg1TileBuffer[(i + mmu.BG1HOFS) % 256][(curScanLine + mmu.BG1VOFS) % 512];
						break;
					case 3:
						color = bg1TileBuffer[(i + mmu.BG1HOFS) % 512][(curScanLine + mmu.BG1VOFS) % 512];
						break;
					}
					int cur = (curScanLine * 240) + i;
					PushColor(color, cur, 1, bg1Priority, i, curScanLine);
				}
				if (bg0) {
					int color = 0;
					first = true;
					switch (bg0ScreenSize)
					{
					case 0:
						color = bg0TileBuffer[(i + mmu.BG0HOFS) % 256][(curScanLine + mmu.BG0VOFS) % 256];
						break;
					case 1:
						color = bg0TileBuffer[(i + mmu.BG0HOFS) % 512][(curScanLine + mmu.BG0VOFS) % 256];
						break;
					case 2:
						color = bg0TileBuffer[(i + mmu.BG0HOFS) % 256][(curScanLine + mmu.BG0VOFS) % 512];
						break;
					case 3:
						color = bg0TileBuffer[(i + mmu.BG0HOFS) % 512][(curScanLine + mmu.BG0VOFS) % 512];
						break;
					}
					int cur = (curScanLine * 240) + i;
					PushColor(color, cur, 0, bg0Priority, i, curScanLine);
				}
				DetermineColor(display);
				ClearFrame();
			}

		}
		else
		{
			for (int i = 0; i < 240; i++) {
				ClearFrame();
				if (WinOut0BG3) {
					int color = 0;
					first = true;
					switch (bg3ScreenSize)
					{
					case 0:
						color = bg3TileBuffer[(i + mmu.BG3HOFS) % 256][(curScanLine + mmu.BG3VOFS) % 256];
						break;
					case 1:
						color = bg3TileBuffer[(i + mmu.BG3HOFS) % 512][(curScanLine + mmu.BG3VOFS) % 256];
						break;
					case 2:
						color = bg3TileBuffer[(i + mmu.BG3HOFS) % 256][(curScanLine + mmu.BG3VOFS) % 512];
						break;
					case 3:
						color = bg3TileBuffer[(i + mmu.BG3HOFS) % 512][(curScanLine + mmu.BG3VOFS) % 512];
						break;
					}
					int cur = (curScanLine * 240) + i;
					PushColor(color, cur, 3, bg3Priority, i, curScanLine);
				}
				if (WinOut0BG2) {
					int color = 0;
					switch (bg2ScreenSize)
					{
					case 0:
						color = bg2TileBuffer[(i + mmu.BG2HOFS) % 256][(curScanLine + mmu.BG2VOFS) % 256];
						break;
					case 1:
						color = bg2TileBuffer[(i + mmu.BG2HOFS) % 512][(curScanLine + mmu.BG2VOFS) % 256];
						break;
					case 2:
						color = bg2TileBuffer[(i + mmu.BG2HOFS) % 256][(curScanLine + mmu.BG2VOFS) % 512];
						break;
					case 3:
						color = bg2TileBuffer[(i + mmu.BG2HOFS) % 512][(curScanLine + mmu.BG2VOFS) % 512];
						break;
					}
					int cur = (curScanLine * 240) + i;
					PushColor(color, cur, 2, bg2Priority, i, curScanLine);
				}
				if (WinOut0BG1) {
					int color = 0;
					switch (bg1ScreenSize)
					{
					case 0:
						color = bg1TileBuffer[(i + mmu.BG1HOFS) % 256][(curScanLine + mmu.BG1VOFS) % 256];
						break;
					case 1:
						color = bg1TileBuffer[(i + mmu.BG1HOFS) % 512][(curScanLine + mmu.BG1VOFS) % 256];
						break;
					case 2:
						color = bg1TileBuffer[(i + mmu.BG1HOFS) % 256][(curScanLine + mmu.BG1VOFS) % 512];
						break;
					case 3:
						color = bg1TileBuffer[(i + mmu.BG1HOFS) % 512][(curScanLine + mmu.BG1VOFS) % 512];
						break;
					}
					int cur = (curScanLine * 240) + i;
					PushColor(color, cur, 1, bg1Priority, i, curScanLine);
				}
				if (WinOut0BG0) {
					int color = 0;
					switch (bg0ScreenSize)
					{
					case 0:
						color = bg0TileBuffer[(i + mmu.BG0HOFS) % 256][(curScanLine + mmu.BG0VOFS) % 256];
						break;
					case 1:
						color = bg0TileBuffer[(i + mmu.BG0HOFS) % 512][(curScanLine + mmu.BG0VOFS) % 256];
						break;
					case 2:
						color = bg0TileBuffer[(i + mmu.BG0HOFS) % 256][(curScanLine + mmu.BG0VOFS) % 512];
						break;
					case 3:
						color = bg0TileBuffer[(i + mmu.BG0HOFS) % 512][(curScanLine + mmu.BG0VOFS) % 512];
						break;
					}
					int cur = (curScanLine * 240) + i;
					PushColor(color, cur, 0, bg0Priority, i, curScanLine);
				}
				DetermineColor(display);
				ClearFrame();
			}

			if (win1) {
				int left = bitrange(15, 8, mmu.WIN1H);
				int right = bitrange(7, 0, mmu.WIN1H);

				if ((left > right)) {
					right = 240;
				}

				int up = bitrange(15, 8, mmu.WIN1V);
				int down = bitrange(7, 0, mmu.WIN1V);

				if (up > down || down > 160) {
					down = 160;
				}

				for (int i = left; i < right; i++) {
					if (curScanLine >= up && curScanLine < down) {
						int ya = 0;
						display.SetPixel((curScanLine * 240) + i, 0, 0, 0);
						if (Win1BG3) {
							int color = 0;
							first = true;
							switch (bg3ScreenSize)
							{
							case 0:
								color = bg3TileBuffer[(i + mmu.BG3HOFS) % 256][(curScanLine + mmu.BG3VOFS) % 256];
								break;
							case 1:
								color = bg3TileBuffer[(i + mmu.BG3HOFS) % 512][(curScanLine + mmu.BG3VOFS) % 256];
								break;
							case 2:
								color = bg3TileBuffer[(i + mmu.BG3HOFS) % 256][(curScanLine + mmu.BG3VOFS) % 512];
								break;
							case 3:
								color = bg3TileBuffer[(i + mmu.BG3HOFS) % 512][(curScanLine + mmu.BG3VOFS) % 512];
								break;
							}
							int cur = (curScanLine * 240) + i;
							PushColor(color, cur, 3, bg3Priority, i, curScanLine);
						}
						if (Win1BG2) {
							int color = 0;
							switch (bg2ScreenSize)
							{
							case 0:
								color = bg2TileBuffer[(i + mmu.BG2HOFS) % 256][(curScanLine + mmu.BG2VOFS) % 256];
								break;
							case 1:
								color = bg2TileBuffer[(i + mmu.BG2HOFS) % 512][(curScanLine + mmu.BG2VOFS) % 256];
								break;
							case 2:
								color = bg2TileBuffer[(i + mmu.BG2HOFS) % 256][(curScanLine + mmu.BG2VOFS) % 512];
								break;
							case 3:
								color = bg2TileBuffer[(i + mmu.BG2HOFS) % 512][(curScanLine + mmu.BG2VOFS) % 512];
								break;
							}
							int cur = (curScanLine * 240) + i;
							PushColor(color, cur, 2, bg2Priority, i, curScanLine);
						}
						if (Win1BG1) {
							int color = 0;
							first = true;
							switch (bg1ScreenSize)
							{
							case 0:
								color = bg1TileBuffer[(i + mmu.BG1HOFS) % 256][(curScanLine + mmu.BG1VOFS) % 256];
								break;
							case 1:
								color = bg1TileBuffer[(i + mmu.BG1HOFS) % 512][(curScanLine + mmu.BG1VOFS) % 256];
								break;
							case 2:
								color = bg1TileBuffer[(i + mmu.BG1HOFS) % 256][(curScanLine + mmu.BG1VOFS) % 512];
								break;
							case 3:
								color = bg1TileBuffer[(i + mmu.BG1HOFS) % 512][(curScanLine + mmu.BG1VOFS) % 512];
								break;
							}
							int cur = (curScanLine * 240) + i;
							PushColor(color, cur, 1, bg1Priority, i, curScanLine);
						}
						if (Win1BG0) {
							int color = 0;
							first = true;
							switch (bg0ScreenSize)
							{
							case 0:
								color = bg0TileBuffer[(i + mmu.BG0HOFS) % 256][(curScanLine + mmu.BG0VOFS) % 256];
								break;
							case 1:
								color = bg0TileBuffer[(i + mmu.BG0HOFS) % 512][(curScanLine + mmu.BG0VOFS) % 256];
								break;
							case 2:
								color = bg0TileBuffer[(i + mmu.BG0HOFS) % 256][(curScanLine + mmu.BG0VOFS) % 512];
								break;
							case 3:
								color = bg0TileBuffer[(i + mmu.BG0HOFS) % 512][(curScanLine + mmu.BG0VOFS) % 512];
								break;
							}
							int cur = (curScanLine * 240) + i;
							PushColor(color, cur, 0, bg0Priority, i, curScanLine);
						}
						DetermineColor(display);
						ClearFrame();
					}

				}
			}
			if (win0) {
				int left = bitrange(15, 8, mmu.WIN0H);
				int right = bitrange(7, 0, mmu.WIN0H);

				if ((left > right)) {
					right = 240;
				}

				int up = bitrange(15, 8, mmu.WIN0V);
				int down = bitrange(7, 0, mmu.WIN0V);

				if (up > down || down > 160) {
					down = 160;
				}

				for (int i = left; i < right; i++) {
					if (curScanLine >= up && curScanLine < down) {
						int ya = 0;
						display.SetPixel((curScanLine * 240) + i, 0, 0, 0);
						if (Win0BG3) {
							int color = 0;
							first = true;
							switch (bg3ScreenSize)
							{
							case 0:
								color = bg3TileBuffer[(i + mmu.BG3HOFS) % 256][(curScanLine + mmu.BG3VOFS) % 256];
								break;
							case 1:
								color = bg3TileBuffer[(i + mmu.BG3HOFS) % 512][(curScanLine + mmu.BG3VOFS) % 256];
								break;
							case 2:
								color = bg3TileBuffer[(i + mmu.BG3HOFS) % 256][(curScanLine + mmu.BG3VOFS) % 512];
								break;
							case 3:
								color = bg3TileBuffer[(i + mmu.BG3HOFS) % 512][(curScanLine + mmu.BG3VOFS) % 512];
								break;
							}
							int cur = (curScanLine * 240) + i;
							PushColor(color, cur, 3, bg3Priority, i, curScanLine);
						}
						if (Win0BG2) {
							int color = 0;
							switch (bg2ScreenSize)
							{
							case 0:
								color = bg2TileBuffer[(i + mmu.BG2HOFS) % 256][(curScanLine + mmu.BG2VOFS) % 256];
								break;
							case 1:
								color = bg2TileBuffer[(i + mmu.BG2HOFS) % 512][(curScanLine + mmu.BG2VOFS) % 256];
								break;
							case 2:
								color = bg2TileBuffer[(i + mmu.BG2HOFS) % 256][(curScanLine + mmu.BG2VOFS) % 512];
								break;
							case 3:
								color = bg2TileBuffer[(i + mmu.BG2HOFS) % 512][(curScanLine + mmu.BG2VOFS) % 512];
								break;
							}
							int cur = (curScanLine * 240) + i;
							PushColor(color, cur, 2, bg2Priority, i, curScanLine);
						}
						if (Win0BG1) {
							int color = 0;
							first = true;
							switch (bg1ScreenSize)
							{
							case 0:
								color = bg1TileBuffer[(i + mmu.BG1HOFS) % 256][(curScanLine + mmu.BG1VOFS) % 256];
								break;
							case 1:
								color = bg1TileBuffer[(i + mmu.BG1HOFS) % 512][(curScanLine + mmu.BG1VOFS) % 256];
								break;
							case 2:
								color = bg1TileBuffer[(i + mmu.BG1HOFS) % 256][(curScanLine + mmu.BG1VOFS) % 512];
								break;
							case 3:
								color = bg1TileBuffer[(i + mmu.BG1HOFS) % 512][(curScanLine + mmu.BG1VOFS) % 512];
								break;
							}
							int cur = (curScanLine * 240) + i;
							PushColor(color, cur, 1, bg1Priority, i, curScanLine);
						}
						if (Win0BG0) {
							int color = 0;
							first = true;
							switch (bg0ScreenSize)
							{
							case 0:
								color = bg0TileBuffer[(i + mmu.BG0HOFS) % 256][(curScanLine + mmu.BG0VOFS) % 256];
								break;
							case 1:
								color = bg0TileBuffer[(i + mmu.BG0HOFS) % 512][(curScanLine + mmu.BG0VOFS) % 256];
								break;
							case 2:
								color = bg0TileBuffer[(i + mmu.BG0HOFS) % 256][(curScanLine + mmu.BG0VOFS) % 512];
								break;
							case 3:
								color = bg0TileBuffer[(i + mmu.BG0HOFS) % 512][(curScanLine + mmu.BG0VOFS) % 512];
								break;
							}
							int cur = (curScanLine * 240) + i;
							PushColor(color, cur, 0, bg0Priority, i, curScanLine);
						}
						DetermineColor(display);
						ClearFrame();
					}

				}
			}

		}
	}

	if (VideoMode == 2) {
		//if (!bg2 && !bg3) {
		for (int i = 0; i < 240; i++) {
			int cur = (curScanLine * 240) + i;
			uint16_t color = mmu.BPRAM[1] << 8 | mmu.BPRAM[0];
			int b = bitrange(14, 10, color) * 8;
			int g = bitrange(9, 5, color) * 8;
			int r = bitrange(4, 0, color) * 8;
			display.SetPixel(cur, r, g, b);
		}
		//}
	}


	if (VideoMode == 3 || VideoMode == 4) {
		for (int i = 0; i < 240; i++) {
			int cur = (curScanLine * 240) + i;
			//int b = bitrange(14, 10, bgBuffer[cur]) * 8;
			//int g = bitrange(9, 5, bgBuffer[cur]) * 8;
			//int r = bitrange(4, 0, bgBuffer[cur]) * 8;
			//display.SetPixel(cur, r, g, b);
			display.SetPixel(cur, bgBuffer[cur]);
		}
	}
	SetBit(mmu.DISPSTAT, 1);
	hBlank = true;
}


void PPU::SetBit(uint16_t & val, int bit)
{
	val |= (1 << bit);
}

void PPU::ClearBit(uint16_t & val, int bit)
{
	val &= ~(1 << bit);
}

void PPU::LoadBgBuffer1(MMU& mmu)
{
	if (VideoMode == 3) {
		for (int i = 0; i < 38400; i++) {
			uint16_t color = mmu.VRAM[(i * 2) + 1] << 8 | mmu.VRAM[(i * 2)];
			bgBuffer[i] = color;
		}
	}
	if (VideoMode == 4) {
		for (int i = 0; i < 38400; i++) {
			int index = i;
			if (pageFlip) {
				index += 0xA000;
			}
			uint8_t pxMap = mmu.VRAM[index];
			pxMap *= 2;
			uint16_t color = mmu.BPRAM[pxMap + 1] << 8 | mmu.BPRAM[pxMap];
			bgBuffer[i] = color;
		}
	}
}

void PPU::LoadSprites(MMU & mmu, Display & display, int spriteNum, int p)
{
	if (spriteNum == 6) {
		int h = 0;
	}
	int start = spriteNum * 8;
	uint16_t attr0 = mmu.OBJA[start + 1] << 8 | mmu.OBJA[start];
	uint16_t attr1 = mmu.OBJA[start + 3] << 8 | mmu.OBJA[start + 2];
	uint16_t attr2 = mmu.OBJA[start + 5] << 8 | mmu.OBJA[start + 4];
	int priority = bitrange(11, 10, attr2);
	//uint16_t attr3 = mmu.OBJA[start + 7] << 8 | mmu.OBJA[start + 6];

	if ( (bitrange(9, 8, attr0) != 0b10) ) {

		//Affine sprite settings
		bool affine = false;
		bool dSize = false;
		int affineParam = 0;
		int pa = 0;
		int pb = 0;
		int pc = 0;
		int pd = 0;
		if (attr0 >> 8 & 1) {
			affine = true;
			if (attr0 >> 9 & 1) {
				dSize = true;
			}
			affineParam = bitrange(13, 9, attr1);
			int indexA = 6 + (32 * affineParam);
			pa = mmu.OBJA[indexA + 1] << 8 | mmu.OBJA[indexA];

			int indexB = 14 + (32 * affineParam);
			pb = mmu.OBJA[indexB + 1] << 8 | mmu.OBJA[indexB];

			int indexC = 22 + (32 * affineParam);
			pc = mmu.OBJA[indexC + 1] << 8 | mmu.OBJA[indexC];

			int indexD = 30 + (32 * affineParam);
			pd = mmu.OBJA[indexD + 1] << 8 | mmu.OBJA[indexD];
		}

		//Regular sprite settings
		//std::vector< std::vector<uint16_t> > Sprite;
		int objMode = 0;
		objMode = bitrange(11, 10, attr0);
		bool mosaic = false;
		bool colorMode = false;
		if (attr0 >> 13 & 1) {
			colorMode = true;
		}
		bool hFlip = false;
		bool vFlip = false;
		if (!affine) {
			if (attr1 >> 12 & 1) {
				hFlip = true;
			}
			if (attr1 >> 13 & 1) {
				vFlip = true;
			}
		}
		int pallete = bitrange(15, 12, attr2);
		
		int width = 0;
		int height = 0;

		uint16_t yCoord = bitrange(7, 0, attr0);
		//if (yCoord >> 7 & 1) {
		//	yCoord |= 0xFF00;
		//}


		uint16_t xCoord = bitrange(8, 0, attr1);
		//if (xCoord >> 8 & 1) {
		//	xCoord |= 0xFE00;
		//}



		// Size
		switch (bitrange(15, 14, attr1))
		{
		case 0:
			switch (bitrange(15, 14, attr0))
			{
			case 0:
				width = 8;
				height = 8;
				break;
			case 1:
				width = 16;
				height = 8;
				break;
			case 2:
				width = 8;
				height = 16;
				break;
			}
			break;
		case 1:
			switch (bitrange(15, 14, attr0))
			{
			case 0:
				width = 16;
				height = 16;
				break;
			case 1:
				width = 32;
				height = 8;
				break;
			case 2:
				width = 8;
				height = 32;
				break;
			}
			break;
		case 2:
			switch (bitrange(15, 14, attr0))
			{
			case 0:
				width = 32;
				height = 32;
				break;
			case 1:
				width = 32;
				height = 16;
				break;
			case 2:
				width = 16;
				height = 32;
				break;
			}
			break;
		case 3:
			switch (bitrange(15, 14, attr0))
			{
			case 0:
				width = 64;
				height = 64;
				break;
			case 1:
				width = 64;
				height = 32;
				break;
			case 2:
				width = 32;
				height = 64;
				break;
			}
			break;
		}

		//Sprite.resize(width, std::vector<uint16_t>(height, 0));

		int size = (width / 8) * (height / 8);

		if (vram2d) {
			for (int i = 0; i < size; i++) {
				int g = 0;
				if (colorMode) {
					for (int j = 0; j < 64; j++) {

						int offset = i / (width / 8);

						int offsetN = i % (width / 8);

						int tileNum = bitrange(9, 0, attr2);

						tileNum += (offsetN * 2) + (offset * 32);

						int tileIndex = (tileNum * 32) + 0x10000;
						int index = (tileIndex + j);

						//int tileIndex = (tileNum * 32) + 0x10000;
						//int index = (tileIndex + j) + (i * 64) + (offset * 512);

						uint16_t colorIndex = (mmu.VRAM[index] * 2) + 0x200;
						uint16_t colorLeft = mmu.BPRAM[colorIndex + 1] << 8 | mmu.BPRAM[colorIndex];


						int hOffset = (i % (width / 8)) * 8;						//Column
						int vOffset = (i / (width / 8)) * 8;						//Row
						int ye = (j) % 8;											//Horizontal position inside tile
						int ya = (j) / 8;											//Vertical position inside tile



						if (dSize) {

						}

						int xPos = hOffset + ye;
						int yPos = vOffset + ya;


						//if (dSize) {
						//	xPos += (width / 2) / 2;
						//	yPos += (height / 2) / 2;
						//}

						if (colorIndex == 512) {
							SetBit(colorLeft, 15);
						}
						//else
						//{
						//	if (affine) {
						//		int y = 0;
						//	}
						//	Sprite[xPos][yPos] = colorLeft;
						//}
						Sprite[xPos][yPos] = colorLeft;

					}
				}
				else
				{
					for (int j = 0; j < 32; j++) {
						int offset = i / (width / 8);

						int offsetN = i % (width / 8);

						int tileNum = bitrange(9, 0, attr2);

						tileNum += (offsetN)+(offset * 32);

						int tileIndex = (tileNum * 32) + 0x10000;
						int index = (tileIndex + j);

						uint16_t colorLeftIndex = (bitrange(3, 0, mmu.VRAM[index]) * 2) + 0x200 + (pallete * 32);
						uint16_t colorRightIndex = (bitrange(7, 4, mmu.VRAM[index]) * 2) + 0x200 + (pallete * 32);

						uint16_t colorLeft = mmu.BPRAM[colorLeftIndex + 1] << 8 | mmu.BPRAM[colorLeftIndex];
						uint16_t colorRight = mmu.BPRAM[colorRightIndex + 1] << 8 | mmu.BPRAM[colorRightIndex];


						// Top left corner coordinates of tile:
						int hOffset = ((i % (width / 8)) * 8);						//Column
						int vOffset = ((i / (width / 8)) * 8);						//Row
						int ye = (j * 2) % 8;										//Horizontal position inside tile
						int ya = (j * 2) / 8;										//Vertical position inside tile


						int xPos = hOffset + ye;
						int yPos = vOffset + ya;

						if (colorLeftIndex % 32 == 0) {
							SetBit(colorLeft, 15);
						}
						Sprite[xPos][yPos] = colorLeft;


						ye = ((j * 2) + 1) % 8;
						ya = ((j * 2) + 1) / 8;

						xPos = hOffset + ye;
						yPos = vOffset + ya;

						if (colorRightIndex % 32 == 0) {
							SetBit(colorRight, 15);
						}
						Sprite[xPos][yPos] = colorRight;




					}
				}
			}
		}
		else
		{
			for (int i = 0; i < size; i++)
			{
				int sdahds = 0;
				for (int j = 0; j < 32; j++) {
					int tileIndex = (bitrange(9, 0, attr2) * 32) + 0x10000;
					int index = (tileIndex + j) + (i * 32);

					uint16_t colorLeftIndex = (bitrange(3, 0, mmu.VRAM[index]) * 2) + 0x200 + (pallete * 32);
					uint16_t colorRightIndex = (bitrange(7, 4, mmu.VRAM[index]) * 2) + 0x200 + (pallete * 32);

					uint16_t colorLeft = mmu.BPRAM[colorLeftIndex + 1] << 8 | mmu.BPRAM[colorLeftIndex];
					uint16_t colorRight = mmu.BPRAM[colorRightIndex + 1] << 8 | mmu.BPRAM[colorRightIndex];

					// Top left corner coordinates of tile:
					int hOffset = ((i % (width / 8)) * 8);						//Column
					int vOffset = ((i / (height / 8)) * 8);						//Row
					int ye = (j * 2) % 8;										//Horizontal position inside tile
					int ya = (j * 2) / 8;										//Vertical position inside tile

					//if (vFlip) {
					//	ya = abs(ya - 7);
					//}
					//if (hFlip) {
					//	ye = abs(ye - 7);
					//}

					if (colorLeftIndex % 32 == 0) {
						SetBit(colorLeft, 15);
					}
					Sprite[hOffset + ye][vOffset + ya] = colorLeft;
					ye = ((j * 2) + 1) % 8;
					ya = ((j * 2) + 1) / 8;

					//if (vFlip) {
					//	ya = abs(ya - 7);
					//}
					//if (hFlip) {
					//	ye = abs(ye - 7);
					//}

					if (colorRightIndex % 32 == 0) {
						SetBit(colorRight, 15);
					}
					Sprite[hOffset + ye][vOffset + ya] = colorRight;

				}

			}
		}

		if (hFlip) {
			for (int j = 0; j < height; j++) {
				std::vector<uint16_t> temp;
				temp.resize(width, 0);
				int index = 0;
				for (int i = width - 1; i > -1; i--)
				{
					temp[index] = Sprite[i][j];
					index++;
				}
				for (int i = 0; i < width; i++) {
					Sprite[i][j] = temp[i];
				}
			}
		}
		if (vFlip) {
			for (int j = 0; j < width; j++) {
				std::vector<uint16_t> temp;
				temp.resize(height, 0);
				int index = 0;
				for (int i = height - 1; i > -1; i--)
				{
					temp[index] = Sprite[j][i];
					index++;
				}
				for (int i = 0; i < height; i++) {
					Sprite[j][i] = temp[i];
				}
			}
		}

		//Draw Sprite

		//if (affine) {
		//	int offsetX = 0;
		//	int offsetY = 0;
		//	//if (dSize) {
		//	//	offsetX = (width / 2) / 2;
		//	//	offsetY = (height / 2) / 2;
		//	//}
		//	//for (int i = 0; i < width; i++) {
		//	//	for (int j = 0; j < height; j++) {
		//	//		if (Sprite[i + offsetX][j + offsetY]) {
		//	//			int b = bitrange(14, 10, Sprite[i + offsetX][j + offsetY]) * 8;
		//	//			int g = bitrange(9, 5, Sprite[i + offsetX][j + offsetY]) * 8;
		//	//			int r = bitrange(4, 0, Sprite[i + offsetX][j + offsetY]) * 8;
		//	//			int pos = (((j + yCoord) % 160) * 240) + ((i + xCoord) % 240);
		//	//			display.SetPixel(pos, r, g, b);
		//	//		}
		//	//	}
		//	//}
		//}
		//else
		//{
		//for (int i = 0; i < width; i++) {
		//	for (int j = 0; j < height; j++) {
		//		int pos = (((j + yCoord) % 160) * 240) + ((i + xCoord) % 240);
		//		if (Sprite[i][j] != 0) {
		//			int b = bitrange(14, 10, Sprite[i][j]) * 8;
		//			int g = bitrange(9, 5, Sprite[i][j]) * 8;
		//			int r = bitrange(4, 0, Sprite[i][j]) * 8;
		//			
		//			display.SetPixel(pos, r, g, b);
		//		}
		//		else
		//		{
		//			display.SetPixel(pos, 0, 0, 0);
		//		}
		//	}
		//}
		//}

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {



				bool show = true;

				int xPos = xCoord + i;
				if (xPos >= 240) {
					xPos -= 512;
				}

				int yPos = yCoord + j;
				if (yPos >= 160) {
					yPos -= 256;
				}

				//int pos = (((yPos)) * 240) + ((xPos));

				if (xPos < 0 || yPos < 0) {
					show = false;
				}

				//int pos = (((j + yCoord) % 160) * 240) + ((i + xCoord) % 240);
				int pos = (yPos * 240) + (xPos);


				if (pos > 0 && pos < 38400 && show) {

					//if (((j + yCoord) % 160) == curScanLine) {
					if ((yPos) == curScanLine) {
						if (!((Sprite[i][j] >> 15) & 1)) {
							if (priority <= bgPriorities[xPos]) {
								switch (objMode)
								{
								case 0:
									// Normal
									switch (colorEffect)
									{
									case 0:
										// No Effect
										display.SetPixel(pos, Sprite[i][j]);
										break;
									case 1:
										// Alpha Blending
										display.SetPixel(pos, Sprite[i][j]);
										break;
									case 2:
										// Fade to white
										if ((BLDCNT >> 4) & 1) {
											uint16_t colorA = Sprite[i][j];
											int redA = (colorA & 0x1F);
											int greenA = ((colorA & 0x3E0) >> 5);
											int blueA = ((colorA & 0x7C00) >> 10);

											float EVZ = 16 - EVY;
											redA = float(redA) *     (EVZ / 16);
											greenA = float(greenA) * (EVZ / 16);
											blueA = float(blueA) *   (EVZ / 16);


											int redB = 31;
											int greenB = 31;
											int blueB = 31;

											redB = float(redB) *     (float(EVY) / 16);
											greenB = float(greenB) * (float(EVY) / 16);
											blueB = float(blueB) *   (float(EVY) / 16);

											int red = redA + redB;
											red *= 8;
											if (red > 255) {
												red = 255;
											}
											int green = greenA + greenB;
											green *= 8;
											if (green > 255) {
												green = 255;
											}
											int blue = blueA + blueB;
											blue *= 8;
											if (blue > 255) {
												blue = 255;
											}
											SetPixel(display, pos, red, green, blue);
										}
										else
										{
											display.SetPixel(pos, Sprite[i][j]);
										}
										break;
									case 3:
										// Fade to black
										if ((BLDCNT >> 4) & 1) {
											uint16_t colorA = Sprite[i][j];
											int redA = (colorA & 0x1F);
											int greenA = ((colorA & 0x3E0) >> 5);
											int blueA = ((colorA & 0x7C00) >> 10);

											float EVZ = 16 - EVY;
											redA = float(redA) *     (EVZ / 16);
											greenA = float(greenA) * (EVZ / 16);
											blueA = float(blueA) *   (EVZ / 16);


											int redB = 0;
											int greenB = 0;
											int blueB = 0;

											redB = float(redB) *     (float(EVY) / 16);
											greenB = float(greenB) * (float(EVY) / 16);
											blueB = float(blueB) *   (float(EVY) / 16);

											int red = redA + redB;
											red *= 8;
											if (red > 255) {
												red = 255;
											}
											int green = greenA + greenB;
											green *= 8;
											if (green > 255) {
												green = 255;
											}
											int blue = blueA + blueB;
											blue *= 8;
											if (blue > 255) {
												blue = 255;
											}
											SetPixel(display, pos, red, green, blue);
										}
										break;
									default:
										display.SetPixel(pos, Sprite[i][j]);
										break;
									}
									//display.SetPixel(pos, Sprite[i][j]);
									break;
								case 1:
									// Semi-transparent (Alpha blend)
								{
									if (bgColorsB[xCoord + i]) {
										uint16_t colorA = Sprite[i][j];
										uint16_t colorB = bgColorsB[xCoord + i];
										int redA = (colorA & 0x1F);
										int greenA = ((colorA & 0x3E0) >> 5);
										int blueA = ((colorA & 0x7C00) >> 10);
										if (EVA == 0) {
											redA = greenA = blueA = 0;
										}
										else
										{
											redA = float(redA) * (float(EVA) / 16);
											greenA = float(greenA) * (float(EVA) / 16);
											blueA = float(blueA) * (float(EVA) / 16);
										}

										int redB = (colorB & 0x1F);
										int greenB = ((colorB & 0x3E0) >> 5);
										int blueB = ((colorB & 0x7C00) >> 10);
										if (EVB == 0) {
											redB = greenB = blueB = 0;
										}
										else
										{
											redB = float(redB) * (float(EVB) / 16);
											greenB = float(greenB) * (float(EVB) / 16);
											blueB = float(blueB) * (float(EVB) / 16);
										}

										int red = redA + redB;
										red *= 8;
										if (red > 255) {
											red = 255;
										}
										int green = greenA + greenB;
										green *= 8;
										if (green > 255) {
											green = 255;
										}
										int blue = blueA + blueB;
										blue *= 8;
										if (blue > 255) {
											blue = 255;
										}
										SetPixel(display, pos, red, green, blue);
									}
									else
									{
										if (colorEffect == 2) {
											if ((BLDCNT >> 4) & 1) {
												uint16_t colorA = Sprite[i][j];
												int redA = (colorA & 0x1F);
												int greenA = ((colorA & 0x3E0) >> 5);
												int blueA = ((colorA & 0x7C00) >> 10);

												float EVZ = 16 - EVY;
												redA = float(redA) *     (EVZ / 16);
												greenA = float(greenA) * (EVZ / 16);
												blueA = float(blueA) *   (EVZ / 16);


												int redB = 31;
												int greenB = 31;
												int blueB = 31;

												redB = float(redB) *     (float(EVY) / 16);
												greenB = float(greenB) * (float(EVY) / 16);
												blueB = float(blueB) *   (float(EVY) / 16);

												int red = redA + redB;
												red *= 8;
												if (red > 255) {
													red = 255;
												}
												int green = greenA + greenB;
												green *= 8;
												if (green > 255) {
													green = 255;
												}
												int blue = blueA + blueB;
												blue *= 8;
												if (blue > 255) {
													blue = 255;
												}
												SetPixel(display, pos, red, green, blue);
											}
											else
											{
												display.SetPixel(pos, Sprite[i][j]);
											}
										}
										else if (colorEffect == 3) {
											if ((BLDCNT >> 4) & 1) {
												uint16_t colorA = Sprite[i][j];
												int redA = (colorA & 0x1F);
												int greenA = ((colorA & 0x3E0) >> 5);
												int blueA = ((colorA & 0x7C00) >> 10);

												float EVZ = 16 - EVY;
												redA = float(redA) *     (EVZ / 16);
												greenA = float(greenA) * (EVZ / 16);
												blueA = float(blueA) *   (EVZ / 16);


												int redB = 0;
												int greenB = 0;
												int blueB = 0;

												redB = float(redB) *     (float(EVY) / 16);
												greenB = float(greenB) * (float(EVY) / 16);
												blueB = float(blueB) *   (float(EVY) / 16);

												int red = redA + redB;
												red *= 8;
												if (red > 255) {
													red = 255;
												}
												int green = greenA + greenB;
												green *= 8;
												if (green > 255) {
													green = 255;
												}
												int blue = blueA + blueB;
												blue *= 8;
												if (blue > 255) {
													blue = 255;
												}
												SetPixel(display, pos, red, green, blue);
											}
											else
											{
												display.SetPixel(pos, Sprite[i][j]);
											}
										}
										else
										{
											display.SetPixel(pos, Sprite[i][j]);
										}
									}
								}
									break;
								case 2:
									// OBJ Window
									break;
								case 3:
									// Prohibited
									break;
								default:
									display.SetPixel(pos, Sprite[i][j]);
									break;
								}
								//display.SetPixel(pos, Sprite[i][j]);
							}
						}
					}

				}

			}
		}

	}

}


uint32_t PPU::bitrange(int msb, int lsb, uint16_t insn)
{
	return (insn >> lsb) & ~(~0 << (msb - lsb + 1));
}

void PPU::Mode0Render(MMU& mmu, Display& display)
{
	if (bg0) {
		readBg0Cnt(mmu);
		bg0ScreenBase = (bg0ScreenBase * 0x800);
		bg0CharBase = (bg0CharBase * 0x4000);
		switch (bg0ScreenSize)
		{
		case 0:
			tileMap.resize(1024, 0);
			break;
		case 1:
			tileMap.resize(2048, 0);
			break;
		case 2:
			tileMap.resize(2048, 0);
			break;
		case 3:
			tileMap.resize(4096, 0);
			break;
		}
		for (int i = 0; i < tileMap.size(); i++)
		{
			// Tile map is index
			tileMap[i] = mmu.VRAM[bg0ScreenBase + (i * 2) + 1] << 8 | mmu.VRAM[bg0ScreenBase + (i * 2)];
			int pBank = ((tileMap[i]) >> 12);
			bool hFlip = false;
			bool vFlip = false;
			if (tileMap[i] >> 10 & 1) {
				hFlip = true;
			}
			if (tileMap[i] >> 11 & 1) {
				vFlip = true;
			}

			if (bg0cSize) {
				int location = (bitrange(9, 0, tileMap[i]) * 64) + bg0CharBase;
				for (int j = 0; j < 64; j++) {

					int colorLocation = (mmu.VRAM[location + j] * 2);

					uint16_t color = mmu.BPRAM[colorLocation + 1] << 8 | mmu.BPRAM[colorLocation];

					if (colorLocation == 0) {
						SetBit(color, 15);
					}

					//int b = bitrange(14, 10, color) * 8;
					//int g = bitrange(9, 5, color) * 8;
					//int r = bitrange(4, 0, color) * 8;

					// Top left corner coordinates of tile:
					int hOffset = (i % 32) * 8;
					int vOffset = (i / 32) * 8;
					int initPos = (vOffset * 240) + hOffset;
					int ye = j % 8;
					int ya = j / 8;

					if (vFlip) {
						ya = abs(ya - 7);
					}
					if (hFlip) {
						ye = abs(ye - 7);
					}

					int posX = 0;
					int posY = 0;

					if (i < 1024) {
						posX = hOffset + ye;
						posY = vOffset + ya;
					}
					else if (i < 2048) {
						posX = hOffset + ye + 256;
						posY = (vOffset + ya) % 256;
					}
					else if (i < 3072) {
						posX = ((hOffset + ye));
						posY = ((vOffset + ya) + 256) % 512;
					}
					else if (i < 4096) {
						posX = ((hOffset + ye) + 256);
						posY = ((vOffset + ya) + 256) % 512;
					}

					bg0TileBuffer[posX][posY] = color;
				}
			}
			else
			{
				int location = (bitrange(9, 0, tileMap[i]) * 32) + bg0CharBase;
				for (int j = 0; j < 32; j++) {
					int index = ((location + j));
					int drawDot = j * 2;
					int colorLocation = (mmu.VRAM[index]);

					uint16_t colorLeftIndex = (bitrange(7, 4, mmu.VRAM[index]) * 2) + (pBank * 32);
					uint16_t colorRightIndex = (bitrange(3, 0, mmu.VRAM[index]) * 2) + (pBank * 32);

					uint16_t colorLeft = mmu.BPRAM[colorLeftIndex + 1] << 8 | mmu.BPRAM[colorLeftIndex];
					uint16_t colorRight = mmu.BPRAM[colorRightIndex + 1] << 8 | mmu.BPRAM[colorRightIndex];

					if (colorLeftIndex % 32 == 0) {
						SetBit(colorLeft, 15);
					}
					else
					{
						ClearBit(colorLeft, 15);
					}
					if (colorRightIndex % 32 == 0) {
						SetBit(colorRight, 15);
					}
					else
					{
						ClearBit(colorRight, 15);
					}

					// Top left corner coordinates of tile:
					int hOffset = ((i % 32) * 8);					//Column
					int vOffset = (((i % 1024) / 32) * 8);			//Row
					int ye = drawDot % 8;							//Horizontal position inside tile
					int ya = (drawDot / 8);							//Vertical position inside tile

					if (vFlip) {
						ya = abs(ya - 7);
					}
					if (hFlip) {
						ye = abs(ye - 7);
					}

					int posX = 0;
					int posY = 0;

					if (i < 1024) {
						posX = hOffset + ye;
						posY = vOffset + ya;
					}
					else if (i < 2048) {
						posX = hOffset + ye + 256;
						posY = (vOffset + ya) % 256;
					}
					else if (i < 3072) {
						posX = ((hOffset + ye));
						posY = ((vOffset + ya) + 256) % 512;
					}
					else if (i < 4096) {
						posX = ((hOffset + ye) + 256);
						posY = ((vOffset + ya) + 256) % 512;
					}

					bg0TileBuffer[posX][posY] = colorRight;

					drawDot++;


					// Top left corner coordinates of tile:
					hOffset = ((i % 32) * 8);
					vOffset = (((i % 1024) / 32) * 8);
					ye = drawDot % 8;
					ya = (drawDot / 8);

					if (vFlip) {
						ya = abs(ya - 7);
					}
					if (hFlip) {
						ye = abs(ye - 7);
					}

					if (i < 1024) {
						posX = hOffset + ye;
						posY = vOffset + ya;
					}
					else if (i < 2048) {
						posX = hOffset + ye + 256;
						posY = (vOffset + ya) % 256;
					}
					else if (i < 3072) {
						posX = ((hOffset + ye));
						posY = ((vOffset + ya) + 256) % 512;
					}
					else if (i < 4096) {
						posX = ((hOffset + ye) + 256) % 512;
						posY = ((vOffset + ya) + 256) % 512;
					}
					bg0TileBuffer[posX][posY] = colorLeft;
				}
			}

		}
	}
	if (bg1) {
		readBg1Cnt(mmu);
		bg1ScreenBase = (bg1ScreenBase * 0x800);
		bg1CharBase = (bg1CharBase * 0x4000);
		switch (bg1ScreenSize)
		{
		case 0:
			tileMap.resize(1024, 0);
			break;
		case 1:
			tileMap.resize(2048, 0);
			break;
		case 2:
			tileMap.resize(2048, 0);
			break;
		case 3:
			tileMap.resize(4096, 0);
			break;
		}
		for (int i = 0; i < tileMap.size(); i++)
		{
			// Tile map is index
			tileMap[i] = mmu.VRAM[bg1ScreenBase + (i * 2) + 1] << 8 | mmu.VRAM[bg1ScreenBase + (i * 2)];
			int pBank = ((tileMap[i]) >> 12);
			bool hFlip = false;
			bool vFlip = false;
			if (tileMap[i] >> 10 & 1) {
				hFlip = true;
			}
			if (tileMap[i] >> 11 & 1) {
				vFlip = true;
			}

			if (true) {
				if (bg1cSize) {
					int location = (bitrange(9, 0, tileMap[i]) * 64) + bg1CharBase;
					for (int j = 0; j < 64; j++) {

						int colorLocation = (mmu.VRAM[location + j] * 2);

						uint16_t color = mmu.BPRAM[colorLocation + 1] << 8 | mmu.BPRAM[colorLocation];

						if (colorLocation == 0) {
							SetBit(color, 15);
						}

						//int b = bitrange(14, 10, color) * 8;
						//int g = bitrange(9, 5, color) * 8;
						//int r = bitrange(4, 0, color) * 8;
						//
						//// Top left corner coordinates of tile:
						//int hOffset = (i % 32) * 8;
						//int vOffset = (i / 32) * 8;
						//int initPos = (vOffset * 240) + hOffset;
						//int ye = j % 8;
						//int ya = j / 8;
						//
						//if ((hOffset + 8) <= 240 && (vOffset + 8) <= 160) {
						//	display.SetPixel(initPos + ye + (ya * 240), r, g, b);
						//}

						int hOffset = (i % 32) * 8;
						int vOffset = (i / 32) * 8;
						int initPos = (vOffset * 240) + hOffset;
						int ye = j % 8;
						int ya = j / 8;

						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}


						int posX = 0;
						int posY = 0;

						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256);
							posY = ((vOffset + ya) + 256) % 512;
						}

						bg1TileBuffer[posX][posY] = color;


					}
				}
				else
				{
					int location = (bitrange(9, 0, tileMap[i]) * 32) + bg1CharBase;
					for (int j = 0; j < 32; j++) {
						int index = (location + j);
						int drawDot = j * 2;
						//int colorLocation = (mmu.VRAM[index]);

						uint16_t colorLeftIndex = (bitrange(7, 4, mmu.VRAM[index]) * 2) + (pBank * 32);
						uint16_t colorRightIndex = (bitrange(3, 0, mmu.VRAM[index]) * 2) + (pBank * 32);

						uint16_t colorLeft = mmu.BPRAM[colorLeftIndex + 1] << 8 | mmu.BPRAM[colorLeftIndex];
						uint16_t colorRight = mmu.BPRAM[colorRightIndex + 1] << 8 | mmu.BPRAM[colorRightIndex];

						if (colorLeftIndex % 32 == 0) {
							SetBit(colorLeft, 15);
						}
						else
						{
							ClearBit(colorLeft, 15);
						}
						if (colorRightIndex % 32 == 0) {
							SetBit(colorRight, 15);
						}
						else
						{
							ClearBit(colorRight, 15);
						}

						//// Top left corner coordinates of tile:
						//int hOffset = (i % 32) * 8;           //Column
						//int vOffset = (i / 32) * 8;			//Row
						//int ye = drawDot % 8;						//Horizontal position inside tile
						//int ya = drawDot / 8;						//Vertical position inside tile
						//
						//if (vFlip) {
						//	ya = abs(ya - 7);
						//}
						//if (hFlip) {
						//	ye = abs(ye - 7);
						//}
						//
						//if (i < 1024) {
						//	bg1TileBuffer[hOffset + ye][vOffset + ya] = colorLeft;
						//}
						//else
						//{
						//	bg1TileBuffer[hOffset + ye + 256][(vOffset + ya) % 256] = colorLeft;
						//}

						// Top left corner coordinates of tile:
						int hOffset = ((i % 32) * 8);					//Column
						int vOffset = (((i % 1024) / 32) * 8);			//Row
						int ye = drawDot % 8;							//Horizontal position inside tile
						int ya = (drawDot / 8);							//Vertical position inside tile

						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}

						int posX = 0;
						int posY = 0;

						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256);
							posY = ((vOffset + ya) + 256) % 512;
						}

						bg1TileBuffer[posX][posY] = colorRight;
						drawDot++;


						//// Top left corner coordinates of tile:
						//hOffset = ((i % 32) * 8);
						//vOffset = ((i / 32) * 8);
						//ye = drawDot % 8;
						//ya = drawDot / 8;
						//
						//
						//if (vFlip) {
						//	ya = abs(ya - 7);
						//}
						//if (hFlip) {
						//	ye = abs(ye - 7);
						//}
						//
						//if (i < 1024) {
						//	bg1TileBuffer[hOffset + ye][vOffset + ya] = colorRight;
						//}
						//else
						//{
						//	bg1TileBuffer[hOffset + ye + 256][(vOffset + ya) % 256] = colorRight;
						//}

						// Top left corner coordinates of tile:
						hOffset = ((i % 32) * 8);
						vOffset = (((i % 1024) / 32) * 8);
						ye = drawDot % 8;
						ya = (drawDot / 8);

						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}

						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256) % 512;
							posY = ((vOffset + ya) + 256) % 512;
						}
						bg1TileBuffer[posX][posY] = colorLeft;

					}
				}
			}
		}
	}
	if (bg2) {
		readBg2Cnt(mmu);
		bg2ScreenBase = (bg2ScreenBase * 0x800);
		bg2CharBase = (bg2CharBase * 0x4000);
		switch (bg2ScreenSize)
		{
		case 0:
			tileMap.resize(1024, 0);
			break;
		case 1:
			tileMap.resize(2048, 0);
			break;
		case 2:
			tileMap.resize(2048, 0);
			break;
		case 3:
			tileMap.resize(4096, 0);
			break;
		}
		for (int i = 0; i < tileMap.size(); i++)
		{
			// Tile map is index
			tileMap[i] = mmu.VRAM[bg2ScreenBase + (i * 2) + 1] << 8 | mmu.VRAM[bg2ScreenBase + (i * 2)];
			int pBank = ((tileMap[i]) >> 12);
			bool hFlip = false;
			bool vFlip = false;
			if (tileMap[i] >> 10 & 1) {
				hFlip = true;
			}
			if (tileMap[i] >> 11 & 1) {
				vFlip = true;
			}


			if (true) {
				if (bg2cSize) {
					int location = (bitrange(9, 0, tileMap[i]) * 64) + bg2CharBase;
					for (int j = 0; j < 64; j++) {

						int colorLocation = (mmu.VRAM[location + j] * 2);

						uint16_t color = mmu.BPRAM[colorLocation + 1] << 8 | mmu.BPRAM[colorLocation];

						if (colorLocation == 0) {
							SetBit(color, 15);
						}

						int b = bitrange(14, 10, color) * 8;
						int g = bitrange(9, 5, color) * 8;
						int r = bitrange(4, 0, color) * 8;

						// Top left corner coordinates of tile:
						int hOffset = (i % 32) * 8;
						int vOffset = (i / 32) * 8;
						int initPos = (vOffset * 240) + hOffset;
						int ye = j % 8;
						int ya = j / 8;

						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}


						int posX = 0;
						int posY = 0;

						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256);
							posY = ((vOffset + ya) + 256) % 512;
						}

						bg2TileBuffer[posX][posY] = color;
					}
				}
				else
				{
					int location = (bitrange(9, 0, tileMap[i]) * 32) + bg2CharBase;
					for (int j = 0; j < 32; j++) {
						int index = ((location + j));
						int drawDot = j * 2;
						int colorLocation = (mmu.VRAM[index]);

						uint16_t colorLeftIndex = (bitrange(7, 4, mmu.VRAM[index]) * 2) + (pBank * 32);
						uint16_t colorRightIndex = (bitrange(3, 0, mmu.VRAM[index]) * 2) + (pBank * 32);

						uint16_t colorLeft = mmu.BPRAM[colorLeftIndex + 1] << 8 | mmu.BPRAM[colorLeftIndex];
						uint16_t colorRight = mmu.BPRAM[colorRightIndex + 1] << 8 | mmu.BPRAM[colorRightIndex];

						if (colorLeftIndex % 32 == 0) {
							SetBit(colorLeft, 15);
						}
						else
						{
							ClearBit(colorLeft, 15);
						}
						if (colorRightIndex % 32 == 0) {
							SetBit(colorRight, 15);
						}
						else
						{
							ClearBit(colorRight, 15);
						}

						

						// Top left corner coordinates of tile:
						int hOffset = ((i % 32) * 8);					//Column
						int vOffset = (((i % 1024) / 32) * 8);					//Row
						int ye = drawDot % 8;						//Horizontal position inside tile
						int ya = (drawDot / 8);						//Vertical position inside tile

						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}

						int posX = 0;
						int posY = 0;

						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256);
							posY = ((vOffset + ya) + 256) % 512;
						}

						bg2TileBuffer[posX][posY] = colorRight;

						drawDot++;


						// Top left corner coordinates of tile:
						hOffset = ((i % 32) * 8);
						vOffset = (((i % 1024) / 32) * 8);
						ye = drawDot % 8;
						ya = (drawDot / 8);
						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}
						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256) % 512;
							posY = ((vOffset + ya) + 256) % 512;
						}
						bg2TileBuffer[posX][posY] = colorLeft;
					}
				}
			}
		}
	}
	if (bg3) {
		readBg3Cnt(mmu);
		bg3ScreenBase = (bg3ScreenBase * 0x800);
		bg3CharBase = (bg3CharBase * 0x4000);
		switch (bg3ScreenSize)
		{
		case 0:
			tileMap.resize(1024, 0);
			break;
		case 1:
			tileMap.resize(2048, 0);
			break;
		case 2:
			tileMap.resize(2048, 0);
			break;
		case 3:
			tileMap.resize(4096, 0);
			break;
		}
		for (int i = 0; i < tileMap.size(); i++)
		{
			if (i == 3) {
				int w = 0;
			}
			// Tile map is index
			tileMap[i] = mmu.VRAM[bg3ScreenBase + (i * 2) + 1] << 8 | mmu.VRAM[bg3ScreenBase + (i * 2)];
			int pBank = ((tileMap[i]) >> 12);
			bool hFlip = false;
			bool vFlip = false;
			if (tileMap[i] >> 10 & 1) {
				hFlip = true;
			}
			if (tileMap[i] >> 11 & 1) {
				vFlip = true;
			}


			if (true) {
				if (bg3cSize) {
					//int location = (tileMap[i] * 64) + bg3CharBase;
					int location = (bitrange(9, 0, tileMap[i]) * 64) + bg3CharBase;

					for (int j = 0; j < 64; j++) {

						int colorLocation = (mmu.VRAM[location + j] * 2);
						//int colorLocation = (mmu.MemoryRead(location + j + 0x6000000) * 2);

						uint16_t color = mmu.BPRAM[colorLocation + 1] << 8 | mmu.BPRAM[colorLocation];

						if (colorLocation == 0) {
							SetBit(color, 15);
						}

						//int b = bitrange(14, 10, color) * 8;
						//int g = bitrange(9, 5, color) * 8;
						//int r = bitrange(4, 0, color) * 8;
						//
						//// Top left corner coordinates of tile:
						//int hOffset = (i % 32) * 8;
						//int vOffset = (i / 32) * 8;
						//int initPos = (vOffset * 240) + hOffset;
						//int ye = j % 8;
						//int ya = j / 8;
						//
						//if ((hOffset + 8) <= 240 && (vOffset + 8) <= 160) {
						//	display.SetPixel(initPos + ye + (ya * 240), r, g, b);
						//}

						int hOffset = (i % 32) * 8;
						int vOffset = (i / 32) * 8;
						int initPos = (vOffset * 240) + hOffset;
						int ye = j % 8;
						int ya = j / 8;

						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}

						int posX = 0;
						int posY = 0;

						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256);
							posY = ((vOffset + ya) + 256) % 512;
						}

						bg3TileBuffer[posX][posY] = color;


					}
				}
				else
				{
					int location = (bitrange(9, 0, tileMap[i]) * 32) + bg3CharBase;

					for (int j = 0; j < 32; j++) {
						//int index = (location + j);
						//int drawDot = j * 2;
						//int colorLocation = (mmu.VRAM[index]);
						//
						//uint16_t colorLeftIndex = (bitrange(3, 0, mmu.VRAM[index]) * 2) + (pBank * 32);
						//uint16_t colorRightIndex = (bitrange(7, 4, mmu.VRAM[index]) * 2) + (pBank * 32);
						//
						//uint16_t colorLeft = mmu.BPRAM[colorLeftIndex + 1] << 8 | mmu.BPRAM[colorLeftIndex];
						//uint16_t colorRight = mmu.BPRAM[colorRightIndex + 1] << 8 | mmu.BPRAM[colorRightIndex];;
						//
						//// Top left corner coordinates of tile:
						//int hOffset = (i % 32) * 8;           //Column
						//int vOffset = (i / 32) * 8;			//Row
						//int ye = drawDot % 8;						//Horizontal position inside tile
						//int ya = drawDot / 8;						//Vertical position inside tile
						//
						//if (vFlip) {
						//	ya = abs(ya - 7);
						//}
						//if (hFlip) {
						//	ye = abs(ye - 7);
						//}
						//
						//if (i < 1024) {
						//	bg3TileBuffer[hOffset + ye][vOffset + ya] = colorLeft;
						//}
						//else
						//{
						//	bg3TileBuffer[hOffset + ye + 256][(vOffset + ya) % 256] = colorLeft;
						//}
						//
						//drawDot++;
						//
						//
						//// Top left corner coordinates of tile:
						//hOffset = ((i % 32) * 8);
						//vOffset = ((i / 32) * 8);
						//ye = drawDot % 8;
						//ya = drawDot / 8;
						//
						//if (vFlip) {
						//	ya = abs(ya - 7);
						//}
						//if (hFlip) {
						//	ye = abs(ye - 7);
						//}
						//
						//if (i < 1024) {
						//	bg3TileBuffer[hOffset + ye][vOffset + ya] = colorRight;
						//}
						//else
						//{
						//	bg3TileBuffer[hOffset + ye + 256][(vOffset + ya) % 256] = colorRight;
						//}
						int index = ((location + j));
						int drawDot = j * 2;
						int colorLocation = (mmu.VRAM[index]);

						uint16_t colorLeftIndex = (bitrange(7, 4, mmu.VRAM[index]) * 2) + (pBank * 32);
						uint16_t colorRightIndex = (bitrange(3, 0, mmu.VRAM[index]) * 2) + (pBank * 32);

						uint16_t colorLeft = mmu.BPRAM[colorLeftIndex + 1] << 8 | mmu.BPRAM[colorLeftIndex];
						uint16_t colorRight = mmu.BPRAM[colorRightIndex + 1] << 8 | mmu.BPRAM[colorRightIndex];

						if (colorLeftIndex % 32 == 0) {
							SetBit(colorLeft, 15);
						}
						else
						{
							ClearBit(colorLeft, 15);
						}
						if (colorRightIndex % 32 == 0) {
							SetBit(colorRight, 15);
						}
						else
						{
							ClearBit(colorRight, 15);
						}

						// Top left corner coordinates of tile:
						int hOffset = ((i % 32) * 8);					//Column
						int vOffset = (((i % 1024) / 32) * 8);			//Row
						int ye = drawDot % 8;							//Horizontal position inside tile
						int ya = (drawDot / 8);							//Vertical position inside tile

						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}

						int posX = 0;
						int posY = 0;

						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256);
							posY = ((vOffset + ya) + 256) % 512;
						}

						bg3TileBuffer[posX][posY] = colorRight;

						drawDot++;


						// Top left corner coordinates of tile:
						hOffset = ((i % 32) * 8);
						vOffset = (((i % 1024) / 32) * 8);
						ye = drawDot % 8;
						ya = (drawDot / 8);

						if (vFlip) {
							ya = abs(ya - 7);
						}
						if (hFlip) {
							ye = abs(ye - 7);
						}

						if (i < 1024) {
							posX = hOffset + ye;
							posY = vOffset + ya;
						}
						else if (i < 2048) {
							posX = hOffset + ye + 256;
							posY = (vOffset + ya) % 256;
						}
						else if (i < 3072) {
							posX = ((hOffset + ye));
							posY = ((vOffset + ya) + 256) % 512;
						}
						else if (i < 4096) {
							posX = ((hOffset + ye) + 256) % 512;
							posY = ((vOffset + ya) + 256) % 512;
						}
						bg3TileBuffer[posX][posY] = colorLeft;
					}
				}
			}
		}
	}
}

void PPU::readDispcnt(MMU& mmu)
{
	uint16_t DISPCNT = mmu.DISPCNT;
	if (bitrange(2, 0, DISPCNT) < 6) {
		VideoMode = bitrange(2, 0, DISPCNT);
	}
	if ((DISPCNT >> 4) & 1) {
		pageFlip = true;
	}
	else
	{
		pageFlip = false;
	}
	if ((DISPCNT >> 6) & 1) {
		vram2d = false;
	}
	else
	{
		vram2d = true;
	}
	if ((DISPCNT >> 7) & 1) {
		forceBlank = true;
	}
	else
	{
		forceBlank = false;
	}
	if ((DISPCNT >> 8) & 1) {
		bg0 = true;
	}
	else
	{
		bg0 = false;
	}
	if ((DISPCNT >> 9) & 1) {
		bg1 = true;
	}
	else
	{
		bg1 = false;
	}
	if ((DISPCNT >> 10) & 1) {
		bg2 = true;
	}
	else
	{
		bg2 = false;
	}
	if ((DISPCNT >> 11) & 1) {
		bg3 = true;
	}
	else
	{
		bg3 = false;
	}
	if ((DISPCNT >> 12) & 1) {
		obj = true;
	}
	else
	{
		obj = false;
	}
	if ((DISPCNT >> 13) & 1) {
		win0 = true;
	}
	else
	{
		win0 = false;
	}
	if ((DISPCNT >> 14) & 1) {
		win1 = true;
	}
	else
	{
		win1 = false;
	}
	if ((DISPCNT >> 15) & 1) {
		winObj = true;
	}
	else
	{
		winObj = false;
	}
}

void PPU::readDispstat(MMU& mmu)
{
	uint16_t DISPSTAT = mmu.DISPSTAT;
	vCountSetting = bitrange(15, 8, DISPSTAT);

	if (vCountSetting == mmu.VCOUNT) {
		SetBit(DISPSTAT, 2);
		SetBit(mmu.DISPSTAT, 2);
	}
	else
	{
		ClearBit(DISPSTAT, 2);
		ClearBit(mmu.DISPSTAT, 2);
	}

	if (vBlank) {
		SetBit(mmu.DISPSTAT, 0);
	}
	else
	{
		ClearBit(mmu.DISPSTAT, 0);
	}
	if (hBlank) {
		SetBit(mmu.DISPSTAT, 1);
	}
	else
	{
		ClearBit(mmu.DISPSTAT, 1);
	}

	//if ((DISPSTAT >> 0) & 1) {
	//	vBlank = true;
	//}
	//else
	//{
	//	vBlank = false;
	//}
	//if ((DISPSTAT >> 1) & 1) {
	//	hBlank = true;
	//}
	//else
	//{
	//	hBlank = false;
	//}
	if ((DISPSTAT >> 2) & 1) {
		vCount = true;
	}
	else
	{
		vCount = false;
	}
	if ((DISPSTAT >> 3) & 1) {
		vBlankIrq = true;
	}
	else
	{
		vBlankIrq = false;
	}
	if ((DISPSTAT >> 4) & 1) {
		hBlankIrq = true;
	}
	else
	{
		hBlankIrq = false;
	}
	if ((DISPSTAT >> 5) & 1) {
		vCountIrq = true;
	}
	else
	{
		vCountIrq = false;
	}

}

void PPU::readVcount(MMU& mmu)
{
	uint16_t VCOUNT = mmu.VCOUNT;
	curScanLine = bitrange(7, 0, VCOUNT);
}

void PPU::readBg0Cnt(MMU & mmu)
{
	uint16_t BG0CNT = mmu.BG0CNT;
	bg0Priority = bitrange(1, 0, BG0CNT);
	bg0CharBase = bitrange(3, 2, BG0CNT);
	if ((BG0CNT >> 6) & 1) {
		bg0Mosaic = true;
	}
	else
	{
		bg0Mosaic = false;
	}
	if ((BG0CNT >> 7) & 1) {
		bg0cSize = true;
	}
	else
	{
		bg0cSize = false;
	}
	bg0ScreenBase = bitrange(12, 8, BG0CNT);
	bg0ScreenSize = bitrange(15, 14, BG0CNT);
}

void PPU::readBg1Cnt(MMU & mmu)
{
	uint16_t BG1CNT = mmu.BG1CNT;
	bg1Priority = bitrange(1, 0, BG1CNT);
	bg1CharBase = bitrange(3, 2, BG1CNT);
	if ((BG1CNT >> 6) & 1) {
		bg1Mosaic = true;
	}
	else
	{
		bg1Mosaic = false;
	}
	if ((BG1CNT >> 7) & 1) {
		bg1cSize = true;
	}
	else
	{
		bg1cSize = false;
	}
	bg1ScreenBase = bitrange(12, 8, BG1CNT);
	bg1ScreenSize = bitrange(15, 14, BG1CNT);
}

void PPU::readBg2Cnt(MMU & mmu)
{
	uint16_t BG2CNT = mmu.BG2CNT;
	bg2Priority = bitrange(1, 0, BG2CNT);
	bg2CharBase = bitrange(3, 2, BG2CNT);
	if ((BG2CNT >> 6) & 1) {
		bg2Mosaic = true;
	}
	else
	{
		bg2Mosaic = false;
	}
	if ((BG2CNT >> 7) & 1) {
		bg2cSize = true;
	}
	else
	{
		bg2cSize = false;
	}
	bg2ScreenBase = bitrange(12, 8, BG2CNT);
	bg2ScreenSize = bitrange(15, 14, BG2CNT);
}

void PPU::readBg3Cnt(MMU & mmu)
{
	uint16_t BG3CNT = mmu.BG3CNT;
	bg3Priority = bitrange(1, 0, BG3CNT);
	bg3CharBase = bitrange(3, 2, BG3CNT);
	if ((BG3CNT >> 6) & 1) {
		bg3Mosaic = true;
	}
	else
	{
		bg3Mosaic = false;
	}
	if ((BG3CNT >> 7) & 1) {
		bg3cSize = true;
	}
	else
	{
		bg3cSize = false;
	}
	bg3ScreenBase = bitrange(12, 8, BG3CNT);
	bg3ScreenSize = bitrange(15, 14, BG3CNT);
}

void PPU::readWinIn(MMU & mmu)
{
	uint16_t WININ = mmu.WININ;
	if (WININ & 1) {
		Win0BG0 = true;
	}
	else
	{
		Win0BG0 = false;
	}
	if (WININ >> 1 & 1) {
		Win0BG1 = true;
	}
	else
	{
		Win0BG1 = false;
	}
	if (WININ >> 2 & 1) {
		Win0BG2 = true;
	}
	else
	{
		Win0BG2 = false;
	}
	if (WININ >> 3 & 1) {
		Win0BG3 = true;
	}
	else
	{
		Win0BG3 = false;
	}
	if (WININ >> 4 & 1) {
		Win0OBJ = true;
	}
	else
	{
		Win0OBJ = false;
	}
	if (WININ >> 5 & 1) {
		Win0Col = true;
	}
	else
	{
		Win0Col = false;
	}


	if (WININ >> 8 & 1) {
		Win1BG0 = true;
	}
	else
	{
		Win1BG0 = false;
	}
	if (WININ >> 9 & 1) {
		Win1BG1 = true;
	}
	else
	{
		Win1BG1 = false;
	}
	if (WININ >> 10 & 1) {
		Win1BG2 = true;
	}
	else
	{
		Win1BG2 = false;
	}
	if (WININ >> 11 & 1) {
		Win1BG3 = true;
	}
	else
	{
		Win1BG3 = false;
	}
	if (WININ >> 12 & 1) {
		Win1OBJ = true;
	}
	else
	{
		Win1OBJ = false;
	}
	if (WININ >> 13 & 1) {
		Win1Col = true;
	}
	else
	{
		Win1Col = false;
	}
}

void PPU::readWinOut(MMU & mmu)
{
	uint16_t WINOUT = mmu.WINOUT;
	if (WINOUT & 1) {
		WinOut0BG0 = true;
	}
	else
	{
		WinOut0BG0 = false;
	}
	if (WINOUT >> 1 & 1) {
		WinOut0BG1 = true;
	}
	else
	{
		WinOut0BG1 = false;
	}
	if (WINOUT >> 2 & 1) {
		WinOut0BG2 = true;
	}
	else
	{
		WinOut0BG2 = false;
	}
	if (WINOUT >> 3 & 1) {
		WinOut0BG3 = true;
	}
	else
	{
		WinOut0BG3 = false;
	}
	if (WINOUT >> 4 & 1) {
		WinOut0OBJ = true;
	}
	else
	{
		WinOut0OBJ = false;
	}
	if (WINOUT >> 5 & 1) {
		WinOut0Col = true;
	}
	else
	{
		WinOut0Col = false;
	}


	if (WINOUT >> 8 & 1) {
		WinOut1BG0 = true;
	}
	else
	{
		WinOut1BG0 = false;
	}
	if (WINOUT >> 9 & 1) {
		WinOut1BG1 = true;
	}
	else
	{
		WinOut1BG1 = false;
	}
	if (WINOUT >> 10 & 1) {
		WinOut1BG2 = true;
	}
	else
	{
		WinOut1BG2 = false;
	}
	if (WINOUT >> 11 & 1) {
		WinOut1BG3 = true;
	}
	else
	{
		WinOut1BG3 = false;
	}
	if (WINOUT >> 12 & 1) {
		WinOut1OBJ = true;
	}
	else
	{
		WinOut1OBJ = false;
	}
	if (WINOUT >> 13 & 1) {
		WinOut1Col = true;
	}
	else
	{
		WinOut1Col = false;
	}
}

void PPU::readGFXEffects(MMU & mmu)
{
	BLDCNT = mmu.BLDCNT;
	BLDALPHA = mmu.BLDALPHA;
	BLDY = mmu.BLDY;
	if (BLDCNT & 1) {
		bg0Target1 = true;
	}
	else
	{
		bg0Target1 = false;
	}
	if ((BLDCNT >> 1) & 1) {
		bg1Target1 = true;
	}
	else
	{
		bg1Target1 = false;
	}
	if ((BLDCNT >> 2) & 1) {
		bg2Target1 = true;
	}
	else
	{
		bg2Target1 = false;
	}
	if ((BLDCNT >> 3) & 1) {
		bg3Target1 = true;
	}
	else
	{
		bg3Target1 = false;
	}
	if ((BLDCNT >> 4) & 1) {
		objTarget1 = true;
	}
	else
	{
		objTarget1 = false;
	}
	if ((BLDCNT >> 5) & 1) {
		bdTarget1 = true;
	}
	else
	{
		bdTarget1 = false;
	}
	if ((BLDCNT >> 8) & 1) {
		bg0Target2 = true;
	}
	else
	{
		bg0Target2 = false;
	}
	if ((BLDCNT >> 9) & 1) {
		bg1Target2 = true;
	}
	else
	{
		bg1Target2 = false;
	}
	if ((BLDCNT >> 10) & 1) {
		bg2Target2 = true;
	}
	else
	{
		bg2Target2 = false;
	}
	if ((BLDCNT >> 11) & 1) {
		bg3Target2 = true;
	}
	else
	{
		bg3Target2 = false;
	}
	if ((BLDCNT >> 12) & 1) {
		objTarget2 = true;
	}
	else
	{
		objTarget2 = false;
	}
	if ((BLDCNT >> 13) & 1) {
		bdTarget2 = true;
	}
	else
	{
		bdTarget2 = false;
	}
	colorEffect = bitrange(7, 6, BLDCNT);
	EVA = bitrange(4, 0, BLDALPHA);
	EVB = bitrange(12, 8, BLDALPHA);
	EVY = bitrange(4, 0, BLDY);



}

PPU::PixelPlacement::PixelPlacement(uint16_t color, int loc, int bg, int priority, int x, int y)
	:
	color(color),
	pLoc(loc),
	bg(bg),
	priority(priority),
	x(x),
	y(y)
	//win0(win0),
	//win1(win1),
	//winout(winout)
{
}
