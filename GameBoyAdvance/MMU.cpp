#include "MMU.h"
#include <fstream>







MMU::MMU()
{
	//EWRAM.resize(0x40000, 0);
	//IWRAM.resize(0x8000, 0);
	//BPRAM.resize(0x400, 0);
	//VRAM.resize(0x18010, 0);
	//OBJA.resize(0x400, 0);
	//SRAM.resize(0x10000, 0);
}

uint32_t MMU::bitrange(int msb, int lsb, uint32_t insn)
{
	return (insn >> lsb) & ~(~0 << (msb - lsb + 1));
}

void MMU::MemoryWrite(uint32_t addr, uint8_t val)
{

	if (addr == 0x6010A90) {
		int h = 1;
	}
	if (addr == 0x6010E44) {
		int h = 1;
	}
	if (addr >= 0x4000060 && addr <= 0x40000A8) {
		int y = 0;
	}
	uint8_t yaga = val;
	if (addr >= 0x00000000 && addr <= 0x00003FFF) {
	}
	if (addr >= 0x02000000 && addr <= 0x02FFFFFF) {
		// On board WRAM
		uint32_t Index = addr - 0x02000000;
		Index %= 0x40000;
		EWRAM[Index] = val;
	}
	if (addr >= 0x03000000 && addr <= 0x03FFFFFF) {
		// On chip WRAM

		uint32_t Index = addr - 0x03000000;
		Index %= 0x8000;

		IWRAM[Index] = val;
	}
	if (addr >= 0x04000000 && addr <= 0x040003FE) {
		// IO Registers
		// LCD IO:
		int ya = 0;
		if (addr >= 0x4000000 && addr < 0x4000002) {
			//2    R / W  DISPCNT   LCD Control
			int y = 1;
		}
		if (addr >= 0x4000002 && addr < 0x4000004) {
			//2    R / W - Undocumented - Green Swap 
		}
		if (addr >= 0x4000004 && addr < 0x4000006) {
			//2    R / W  DISPSTAT  General LCD Status(STAT, LYC) 
		}
		if (addr >= 0x4000006 && addr < 0x4000008) {
			//2    R    VCOUNT    Vertical Counter(LY)
		}
		if (addr >= 0x4000008 && addr < 0x400000A) {
			//2    R / W  BG0CNT    BG0 Control
		}
		if (addr >= 0x400000A && addr < 0x400000C) {
			//2    R / W  BG1CNT    BG1 Control
		}
		if (addr >= 0x400000C && addr < 0x400000E) {
			//2    R / W  BG2CNT    BG2 Control 
		}
		if (addr >= 0x400000E && addr < 0x4000010) {
			//2    R / W  BG3CNT    BG3 Control 
		}
		if (addr >= 0x4000010 && addr < 0x4000012) {
			//2    W    BG0HOFS   BG0 X - Offset
			int y = 0;
		}
		if (addr >= 0x4000012 && addr < 0x4000014) {
			//2    W    BG0VOFS   BG0 Y - Offset
			int y = 0;
		}
		if (addr >= 0x4000014 && addr < 0x4000016) {
			//2    W    BG1HOFS   BG1 X - Offset
			BG1HOFS = val;
		}
		if (addr >= 0x4000016 && addr < 0x4000018) {
			//2    W    BG1VOFS   BG1 Y - Offset
			BG1VOFS = val;
		}
		if (addr >= 0x4000018 && addr < 0x400001A) {
			//2    W    BG2HOFS   BG2 X - Offset
			BG2HOFS = val;
		}
		if (addr >= 0x400001A && addr < 0x400001C) {
			//2    W    BG2VOFS   BG2 Y - Offset
			BG2VOFS = val;
		}
		if (addr >= 0x400001C && addr < 0x400001E) {
			//2    W    BG3HOFS   BG3 X - Offset
		}
		if (addr >= 0x400001E && addr < 0x4000020) {
			//2    W    BG3VOFS   BG3 Y - Offset
		}
		if (addr >= 0x4000020 && addr < 0x4000022) {
			//2    W    BG2PA     BG2 Rotation / Scaling Parameter A(dx)
		}
		if (addr >= 0x4000022 && addr < 0x4000024) {
			//2    W    BG2PB     BG2 Rotation / Scaling Parameter B(dmx)
		}
		if (addr >= 0x4000024 && addr < 0x4000026) {
			//2    W    BG2PC     BG2 Rotation / Scaling Parameter C(dy)
		}
		if (addr >= 0x4000026 && addr < 0x4000028) {
			//2    W    BG2PD     BG2 Rotation / Scaling Parameter D(dmy)
		}
		if (addr >= 0x4000028 && addr < 0x400002C) {
			//4    W    BG2X      BG2 Reference Point X - Coordinate
		}
		if (addr >= 0x400002C && addr < 0x4000030) {
			//4    W    BG2Y      BG2 Reference Point Y - Coordinate
		}
		if (addr >= 0x4000030 && addr < 0x4000032) {
			//2    W    BG3PA     BG3 Rotation / Scaling Parameter A(dx)
		}
		if (addr >= 0x4000032 && addr < 0x4000034) {
			//2    W    BG3PB     BG3 Rotation / Scaling Parameter B(dmx)
		}
		if (addr >= 0x4000034 && addr < 0x4000036) {
			//2    W    BG3PC     BG3 Rotation / Scaling Parameter C(dy)
		}
		if (addr >= 0x4000036 && addr < 0x4000038) {
			//2    W    BG3PD     BG3 Rotation / Scaling Parameter D(dmy)
		}
		if (addr >= 0x4000038 && addr < 0x400003C) {
			//4    W    BG3X      BG3 Reference Point X - Coordinate
		}
		if (addr >= 0x400003C && addr < 0x4000040) {
			//4    W    BG3Y      BG3 Reference Point Y - Coordinate
		}
		if (addr >= 0x4000040 && addr < 0x4000042) {
			//2    W    WIN0H     Window 0 Horizontal Dimensions
			WIN0H = val;
		}
		if (addr >= 0x4000042 && addr < 0x4000044) {
			//2    W    WIN1H     Window 1 Horizontal Dimensions 
			WIN1H = val;
		}
		if (addr >= 0x4000044 && addr < 0x4000046) {
			//2    W    WIN0V     Window 0 Vertical Dimensions 
			WIN0V = val;
		}
		if (addr >= 0x4000046 && addr < 0x4000048) {
			//2    W    WIN1V     Window 1 Vertical Dimensions 
			WIN1V = val;
		}
		if (addr >= 0x4000048 && addr < 0x400004A) {
			//2    R / W  WININ     Inside of Window 0 and 1
			WININ = val;
		}
		if (addr >= 0x400004A && addr < 0x400004C) {
			//2    R / W  WINOUT    Inside of OBJ Window & Outside of Windows 
			WINOUT = val;
		}
		if (addr >= 0x400004C && addr < 0x400004E) {
			//2    W    MOSAIC    Mosaic Size }
			MOSAIC = val;
		}
		if (addr >= 0x4000050 && addr < 0x4000052) {
			//2    R / W  BLDCNT    Color Special Effects Selection
			BLDCNT = val;
		}
		if (addr >= 0x4000052 && addr < 0x4000054) {
			//2    R / W  BLDALPHA  Alpha Blending Coefficients 
			BLDALPHA = val;
		}
		if (addr >= 0x4000054 && addr < 0x4000056) {
			//2    W    BLDY      Brightness(Fade - In / Out) Coefficient 
			BLDY = val;
		}

		if (addr >= 0x4000100 && addr <= 0x400010E) {
			int y = 0;
		}

		if (addr >= 0x4000200 && addr < 0x4000202) {
			//Interrupt Enable Register
			if (addr == 0x4000200) {
				IE = val;
			}
			if (addr == 0x4000201) {
				IE |= val << 8;
			}
		}
		if (addr >= 0x4000202 && addr < 0x4000204) {
			//Interrupt Request Flags / IRQ Acknowledge
			if (addr == 0x4000202) {
				IF &= ~val;
			}
			if (addr == 0x4000203) {
				IF &= ~(val << 8);
			}
		}
		if (addr >= 0x4000204 && addr < 0x4000206) {
			//Game Pak Waitstate Control
			if (addr == 0x4000204) {
				WAITCNT = val;
			}
			if (addr == 0x4000205) {
				WAITCNT |= val << 8;
			}
		}
		if (addr >= 0x4000208 && addr < 0x400020A) {
			//Interrupt Master Enable Register
			if (addr == 0x4000208) {
				IME = val;
			}
			if (addr == 0x4000209) {
				IME |= val << 8;
			}
		}
		if (addr == 0x4000300) {
			POSTFLG = val;
		}
		if (addr == 0x4000301) {
			HALTCNT = val;
		}
	}
	if (addr >= 0x05000000 && addr < 0x06000000) {
		// BG/OBJ Palette Ram

		uint32_t Index = addr - 0x05000000;
		Index %= 0x400;

		BPRAM[Index] = val;
	}
	if (addr >= 0x06000000 && addr < 0x07000000) {
		// VRAM
		uint32_t Index = addr - 0x06000000;
		if (Index == 0x82E0) {
			int y = 0;
		}
		if (Index == 83196) {
			int y = 0;
		}
		if (Index == 0xA500) {
			int y = 0;
		}
		Index %= 0x20000;
		if (Index >= 0x18000) {
			Index -= 0x8000;
		}


		VRAM[Index] = val;
	}
	if (addr >= 0x07000000 && addr < 0x08000000) {
		// OAM - OBJ Attributes
		uint32_t Index = addr - 0x07000000;
		Index %= 0x400;

		OBJA[Index] = val;
	}
	if (addr >= 0x08000000 && addr <= 0x0DFFFFFF) {
		// Game Pak Rom - FlashRom
		uint32_t Index = addr - 0x08000000;
		Index %= 0x2000000;
		if (Index < Rom.size()) {

		}
		else
		{
			EEPROM[Index] = val;
		}
	}
	if (addr >= 0x0E000000 && addr <= 0x0E00FFFF) {
		// Game Pak SRAM
		uint32_t Index = addr - 0x0E000000;
		SRAM[Index] = val;
	}
}

void MMU::MemoryWrite(uint32_t addr, uint16_t val)
{
	if (addr == 0x6010A90) {
		int h = 1;
	}
	if (addr == 0x6010E44) {
		int h = 1;
	}
	int y = 0;
	if (addr == 0x60019A4) {
		int h = 1;
	}
	if (addr == 0x600F0C0) {
		int h = 1;
	}
	if (addr >= 0x4000060 && addr <= 0x40000A8) {
		int y = 0;
	}
	if (addr >= 0x00000000 && addr <= 0x00003FFF) {
	}
	if (addr >= 0x02000000 && addr <= 0x02FFFFFF) {
		// On board WRAM
		uint32_t Index = addr - 0x02000000;
		Index %= 0x40000;
		EWRAM[Index] = bitrange(7, 0, val);
		EWRAM[Index + 1] = bitrange(15, 8, val);
	}
	if (addr >= 0x03000000 && addr <= 0x03FFFFFF) {
		// On chip WRAM

		uint32_t Index = addr - 0x03000000;
		Index %= 0x8000;

		IWRAM[Index] = bitrange(7, 0, val);
		IWRAM[Index + 1] = bitrange(15, 8, val);
	}
	if (addr >= 0x04000000 && addr <= 0x040003FE) {
		// IO Registers
		// LCD IO:
		int d = 0;
		if (addr >= 0x4000000 && addr < 0x4000002) {
			if (val == 16) {
				int huh = 0;
			}
			DISPCNT = val;
			//2    R / W  DISPCNT   LCD Control
		}
		if (addr >= 0x4000004 && addr < 0x4000006) {
			DISPSTAT = val;
			//2    R / W  DISPSTAT  General LCD Status(STAT, LYC) 
		}
		if (addr >= 0x4000006 && addr < 0x4000008) {
			VCOUNT = val;
			//2    R    VCOUNT    Vertical Counter(LY)
		}
		if (addr >= 0x4000008 && addr < 0x400000A) {
			//2    R / W  BG0CNT    BG0 Control
			BG0CNT = val;
		}
		if (addr >= 0x400000A && addr < 0x400000C) {
			//2    R / W  BG1CNT    BG1 Control
			BG1CNT = val;
		}
		if (addr >= 0x400000C && addr < 0x400000E) {
			//2    R / W  BG2CNT    BG2 Control 
			BG2CNT = val;
		}
		if (addr >= 0x400000E && addr < 0x4000010) {
			//2    R / W  BG3CNT    BG3 Control 
			BG3CNT = val;
		}
		if (addr >= 0x4000010 && addr < 0x4000012) {
			//2    W    BG0HOFS   BG0 X - Offset
			BG0HOFS = val;
		}
		if (addr >= 0x4000012 && addr < 0x4000014) {
			//2    W    BG0VOFS   BG0 Y - Offset
			BG0VOFS = val;
		}
		if (addr >= 0x4000014 && addr < 0x4000016) {
			//2    W    BG1HOFS   BG1 X - Offset
			BG1HOFS = val;
		}
		if (addr >= 0x4000016 && addr < 0x4000018) {
			//2    W    BG1VOFS   BG1 Y - Offset
			BG1VOFS = val;
		}
		if (addr >= 0x4000018 && addr < 0x400001A) {
			//2    W    BG2HOFS   BG2 X - Offset
			BG2HOFS = val;
		}
		if (addr >= 0x400001A && addr < 0x400001C) {
			//2    W    BG2VOFS   BG2 Y - Offset
			BG2VOFS = val;
		}
		if (addr >= 0x400001C && addr < 0x400001E) {
			//2    W    BG3HOFS   BG3 X - Offset
			BG3HOFS = val;
		}
		if (addr >= 0x400001E && addr < 0x4000020) {
			//2    W    BG3VOFS   BG3 Y - Offset
			BG3VOFS = val;
		}
		if (addr >= 0x4000020 && addr < 0x4000022) {
			//2    W    BG2PA     BG2 Rotation / Scaling Parameter A(dx)
			BG2PA = val;
		}
		if (addr >= 0x4000022 && addr < 0x4000024) {
			//2    W    BG2PB     BG2 Rotation / Scaling Parameter B(dmx)
			BG2PB = val;
		}
		if (addr >= 0x4000024 && addr < 0x4000026) {
			//2    W    BG2PC     BG2 Rotation / Scaling Parameter C(dy)
			BG2PC = val;
		}
		if (addr >= 0x4000026 && addr < 0x4000028) {
			//2    W    BG2PD     BG2 Rotation / Scaling Parameter D(dmy)
			BG2PD = val;
		}
		if (addr >= 0x4000028 && addr < 0x400002A) {
			//4    W    BG2X      BG2 Reference Point X - Coordinate
			BG2X = val;
		}
		if (addr >= 0x400002A && addr < 0x400002C) {
			//4    W    BG2X      BG2 Reference Point X - Coordinate
			BG2X = val;
		}
		if (addr >= 0x400002C && addr < 0x400002E) {
			//4    W    BG2Y      BG2 Reference Point Y - Coordinate
			BG2Y = val;
		}
		if (addr >= 0x400002E && addr < 0x4000030) {
			//4    W    BG2Y      BG2 Reference Point Y - Coordinate
			BG2Y = val;
		}
		if (addr >= 0x4000030 && addr < 0x4000032) {
			//2    W    BG3PA     BG3 Rotation / Scaling Parameter A(dx)
			BG3PA = val;
		}
		if (addr >= 0x4000032 && addr < 0x4000034) {
			//2    W    BG3PB     BG3 Rotation / Scaling Parameter B(dmx)
			BG2PB = val;
		}
		if (addr >= 0x4000034 && addr < 0x4000036) {
			//2    W    BG3PC     BG3 Rotation / Scaling Parameter C(dy)
			BG3PC = val;
		}
		if (addr >= 0x4000036 && addr < 0x4000038) {
			//2    W    BG3PD     BG3 Rotation / Scaling Parameter D(dmy)
			BG3PD = val;
		}
		if (addr >= 0x4000038 && addr < 0x400003A) {
			//4    W    BG3X      BG3 Reference Point X - Coordinate
			BG3X = val;
		}
		if (addr >= 0x400003A && addr < 0x400003C) {
			//4    W    BG3X      BG3 Reference Point X - Coordinate
			BG3X = val;
		}
		if (addr >= 0x400003C && addr < 0x400003E) {
			//4    W    BG3Y      BG3 Reference Point Y - Coordinate
			BG3Y = val;
		}
		if (addr >= 0x400003E && addr < 0x4000040) {
			//4    W    BG3Y      BG3 Reference Point Y - Coordinate
			BG3Y = val;
		}
		if (addr >= 0x4000040 && addr < 0x4000042) {
			//2    W    WIN0H     Window 0 Horizontal Dimensions
			WIN0H = val;
		}
		if (addr >= 0x4000042 && addr < 0x4000044) {
			//2    W    WIN1H     Window 1 Horizontal Dimensions 
			WIN1H = val;
		}
		if (addr >= 0x4000044 && addr < 0x4000046) {
			//2    W    WIN0V     Window 0 Vertical Dimensions 
			WIN0V = val;
		}
		if (addr >= 0x4000046 && addr < 0x4000048) {
			//2    W    WIN1V     Window 1 Vertical Dimensions 
			WIN1V = val;
		}
		if (addr >= 0x4000048 && addr < 0x400004A) {
			//2    R / W  WININ     Inside of Window 0 and 1
			WININ = val;
		}
		if (addr >= 0x400004A && addr < 0x400004C) {
			//2    R / W  WINOUT    Inside of OBJ Window & Outside of Windows 
			WINOUT = val;
		}
		if (addr >= 0x400004C && addr < 0x400004E) {
			//2    W    MOSAIC    Mosaic Size }
		}
		if (addr >= 0x400004E && addr < 0x4000050) {
			//--Not used 
		}
		if (addr >= 0x4000050 && addr < 0x4000052) {
			//2    R / W  BLDCNT    Color Special Effects Selection
			BLDCNT = val;
		}
		if (addr >= 0x4000052 && addr < 0x4000054) {
			//2    R / W  BLDALPHA  Alpha Blending Coefficients 
			BLDALPHA = val;
		}
		if (addr >= 0x4000054 && addr < 0x4000056) {
			//2    W    BLDY      Brightness(Fade - In / Out) Coefficient 
			BLDY = val;
		}

		if (addr >= 0x4000060 && addr <= 0x40000A8) {
			int y = 0;
		}

		if (addr >= 0x4000080 && addr < 0x4000082) {
			SOUNDCNT_L = val;
		}
		if (addr >= 0x4000082 && addr < 0x4000084) {
			SOUNDCNT_H = val;
		}
		if (addr >= 0x4000084 && addr < 0x4000086) {
			SOUNDCNT_X = val;
		}


		if (addr >= 0x4000088 && addr < 0x400008A) {
			SOUNDBIAS = val;
		}
		if (addr >= 0x4000100 && addr < 0x4000102) {
			TM0CNT_L = val;
		}
		if (addr >= 0x4000102 && addr < 0x4000104) {
			TM0CNT_H = val;
		}
		if (addr >= 0x4000104 && addr < 0x4000106) {
			TM1CNT_L = val;
		}
		if (addr >= 0x4000106 && addr < 0x4000108) {
			TM1CNT_H = val;
		}
		if (addr >= 0x4000108 && addr < 0x400010A) {
			TM2CNT_L = val;
		}
		if (addr >= 0x400010A && addr < 0x400010C) {
			TM2CNT_H = val;
		}
		if (addr >= 0x400010C && addr < 0x400010E) {
			TM3CNT_L = val;
		}
		if (addr >= 0x400010E && addr < 0x4000110) {
			TM3CNT_H = val;
		}
		if (addr >= 0x40000B0 && addr < 0x40000B4) {
			DMA0SAD = val;
		}
		if (addr >= 0x40000B4 && addr < 0x40000B8) {
			DMA0DAD = val;
		}
		if (addr >= 0x40000B8 && addr < 0x40000BA) {
			DMA0CNT_L = val;
		}
		if (addr >= 0x40000BA && addr < 0x40000BC) {
			DMA0CNT_H = val;
		}
		if (addr >= 0x40000BC && addr < 0x40000C0) {
			DMA1SAD = val;
		}
		if (addr >= 0x40000C0 && addr < 0x40000C4) {
			DMA1DAD = val;
		}
		if (addr >= 0x40000C4 && addr < 0x40000C6) {
			DMA1CNT_L = val;
		}
		if (addr >= 0x40000C6 && addr < 0x40000C8) {
			DMA1CNT_H = val;
		}
		if (addr >= 0x40000C8 && addr < 0x40000CC) {
			DMA2SAD = val;
		}
		if (addr >= 0x40000CC && addr < 0x40000D0) {
			DMA2DAD = val;
		}
		if (addr >= 0x40000D0 && addr < 0x40000D2) {
			DMA2CNT_L = val;
		}
		if (addr >= 0x40000D2 && addr < 0x40000D4) {
			DMA2CNT_H = val;
		}
		if (addr >= 0x40000D4 && addr < 0x40000D8) {
			DMA3SAD = val;
		}
		if (addr >= 0x40000D8 && addr < 0x40000DC) {
			DMA3DAD = val;
		}
		if (addr >= 0x40000DC && addr < 0x40000DE) {
			DMA3CNT_L = val;
		}
		if (addr >= 0x40000DE && addr < 0x40000E0) {
			DMA3CNT_H = val;
		}
		if (addr >= 0x4000130 && addr < 0x4000132) {
			KEYCNT = val;
		}
		if (addr >= 0x4000200 && addr < 0x4000202) {
			//Interrupt Enable Register
			IE = val;
		}
		if (addr >= 0x4000202 && addr < 0x4000204) {
			//Interrupt Request Flags / IRQ Acknowledge
			IF &= ~val;
		}
		if (addr >= 0x4000204 && addr < 0x4000206) {
			//Game Pak Waitstate Control
			WAITCNT = val;
		}
		if (addr >= 0x4000208 && addr < 0x400020A) {
			//Interrupt Master Enable Register
			IME = val;
		}
		if (addr == 0x4000300) {
			POSTFLG = val;
		}
		if (addr == 0x4000301) {
			HALTCNT = val;
		}
	}
	if (addr >= 0x05000000 && addr < 0x06000000) {
		// BG/OBJ Palette Ram

		uint32_t Index = addr - 0x05000000;
		Index %= 0x400;

		BPRAM[Index] = bitrange(7, 0, val);
		BPRAM[Index + 1] = bitrange(15, 8, val);

	}
	if (addr >= 0x06000000 && addr < 0x07000000) {
		// VRAM
		if (val > 0) {
			int lmk = 0;
		}
		//addr = addr % 0x06020000;
		uint32_t Index = addr - 0x06000000;
		if (Index == 0x82E0) {
			int y = 0;
		}
		if (Index == 0xA6E0) {
			int y = 0;
		}
		Index %= 0x20000;
		if (Index >= 0x18000) {
			Index -= 0x8000;
		}


		VRAM[Index] = bitrange(7, 0, val);
		VRAM[Index + 1] = bitrange(15, 8, val);
	}
	if (addr >= 0x07000000 && addr < 0x08000000) {
		// OAM - OBJ Attributes

		uint32_t Index = addr - 0x07000000;
		Index %= 0x400;

		OBJA[Index] = bitrange(7, 0, val);
		OBJA[Index + 1] = bitrange(15, 8, val);
	}
	if (addr >= 0x08000000 && addr <= 0x0DFFFFFF) {
		// Game Pak Rom - FlashRom
		uint32_t Index = addr - 0x08000000;
		Index %= 0x2000000;
		if (Index < Rom.size()) {

		}
		else
		{
			EEPROM[Index] = bitrange(7, 0, val);
			EEPROM[Index + 1] = bitrange(15, 8, val);
		}
	}
	if (addr >= 0x0E000000 && addr <= 0x0E00FFFF) {
		// Game Pak SRAM
		uint32_t Index = addr - 0x0E000000;
		SRAM[Index] = bitrange(7, 0, val);
		SRAM[Index + 1] = bitrange(15, 8, val);
	}

}

void MMU::MemoryWrite(uint32_t addr, uint32_t val)
{
	if (addr == 0x6010A90) {
		int h = 1;
	}
	if (addr == 0x6010E44) {
		int h = 1;
	}
	if (addr == 0x60019A4) {
		int h = 1;
	}
	if (addr >= 0x4000060 && addr <= 0x40000A8) {
		int y = 0;
	}
	if (addr >= 0x00000000 && addr <= 0x00003FFF) {
	}
	if (addr == 0x600F0C0) {
		int h = 1;
	}
	if (addr >= 0x02000000 && addr <= 0x02FFFFFF) {
		// On board WRAM

		uint32_t Index = addr - 0x02000000;
		Index %= 0x40000;

		EWRAM[Index] = bitrange(7, 0, val);
		EWRAM[Index + 1] = bitrange(15, 8, val);
		EWRAM[Index + 2] = bitrange(23, 16, val);
		EWRAM[Index + 3] = bitrange(31, 24, val);
	}
	if (addr >= 0x03000000 && addr <= 0x03FFFFFF) {
		// On chip WRAM

		uint32_t Index = addr - 0x03000000;
		Index %= 0x8000;

		IWRAM[Index] = bitrange(7, 0, val);
		IWRAM[Index + 1] = bitrange(15, 8, val);
		IWRAM[Index + 2] = bitrange(23, 16, val);
		IWRAM[Index + 3] = bitrange(31, 24, val);
	}
	if (addr >= 0x04000000 && addr <= 0x040003FE) {
		// IO Registers
		// LCD IO:
		if (addr >= 0x4000000 && addr < 0x4000002) {
			DISPCNT = val;
			//2    R / W  DISPCNT   LCD Control
		}
		if (addr >= 0x4000004 && addr < 0x4000006) {
			DISPSTAT = val;
			//2    R / W  DISPSTAT  General LCD Status(STAT, LYC) 
		}
		if (addr >= 0x4000006 && addr < 0x4000008) {
			VCOUNT = val;
			//2    R    VCOUNT    Vertical Counter(LY)
		}
		if (addr >= 0x4000008 && addr < 0x400000A) {
			//2    R / W  BG0CNT    BG0 Control
			BG0CNT = val;
		}
		if (addr >= 0x400000A && addr < 0x400000C) {
			//2    R / W  BG1CNT    BG1 Control
			BG1CNT = val;
		}
		if (addr >= 0x400000C && addr < 0x400000E) {
			//2    R / W  BG2CNT    BG2 Control 
			BG2CNT = val;
		}
		if (addr >= 0x400000E && addr < 0x4000010) {
			//2    R / W  BG3CNT    BG3 Control 
			BG3CNT = val;
		}
		if (addr >= 0x4000010 && addr < 0x4000014) {
			//2    W    BG0HOFS   BG0 X - Offset
			if (val != 0) {
				int w = 0;
			}
			BG0HOFS = bitrange(15, 0, val);
			BG0VOFS = bitrange(31, 16, val);
		}
		if (addr >= 0x4000014 && addr < 0x4000016) {
			//2    W    BG1HOFS   BG1 X - Offset
			BG1HOFS = val;
		}
		if (addr >= 0x4000016 && addr < 0x4000018) {
			//2    W    BG1VOFS   BG1 Y - Offset
			BG1VOFS = val;
		}
		if (addr >= 0x4000018 && addr < 0x400001A) {
			//2    W    BG2HOFS   BG2 X - Offset
			BG2HOFS = val;
		}
		if (addr >= 0x400001A && addr < 0x400001C) {
			//2    W    BG2VOFS   BG2 Y - Offset
			BG2VOFS = val;
		}
		if (addr >= 0x400001C && addr < 0x400001E) {
			//2    W    BG3HOFS   BG3 X - Offset
			BG3HOFS = val;
		}
		if (addr >= 0x400001E && addr < 0x4000020) {
			//2    W    BG3VOFS   BG3 Y - Offset
			BG3VOFS = val;
		}
		if (addr >= 0x4000020 && addr < 0x4000022) {
			//2    W    BG2PA     BG2 Rotation / Scaling Parameter A(dx)
		}
		if (addr >= 0x4000022 && addr < 0x4000024) {
			//2    W    BG2PB     BG2 Rotation / Scaling Parameter B(dmx)
		}
		if (addr >= 0x4000024 && addr < 0x4000026) {
			//2    W    BG2PC     BG2 Rotation / Scaling Parameter C(dy)
		}
		if (addr >= 0x4000026 && addr < 0x4000028) {
			//2    W    BG2PD     BG2 Rotation / Scaling Parameter D(dmy)
		}
		if (addr >= 0x4000028 && addr < 0x400002C) {
			//4    W    BG2X      BG2 Reference Point X - Coordinate
			BG2X = val;
		}
		if (addr >= 0x400002C && addr < 0x4000030) {
			//4    W    BG2Y      BG2 Reference Point Y - Coordinate
			BG2Y = val;
		}
		if (addr >= 0x4000030 && addr < 0x4000032) {
			//2    W    BG3PA     BG3 Rotation / Scaling Parameter A(dx)
		}
		if (addr >= 0x4000032 && addr < 0x4000034) {
			//2    W    BG3PB     BG3 Rotation / Scaling Parameter B(dmx)
		}
		if (addr >= 0x4000034 && addr < 0x4000036) {
			//2    W    BG3PC     BG3 Rotation / Scaling Parameter C(dy)
		}
		if (addr >= 0x4000036 && addr < 0x4000038) {
			//2    W    BG3PD     BG3 Rotation / Scaling Parameter D(dmy)
		}
		if (addr >= 0x4000038 && addr < 0x400003C) {
			//4    W    BG3X      BG3 Reference Point X - Coordinate
			BG3X = val;
		}
		if (addr >= 0x400003C && addr < 0x4000040) {
			//4    W    BG3Y      BG3 Reference Point Y - Coordinate
			BG3Y = val;
		}
		if (addr >= 0x4000040 && addr < 0x4000042) {
			//2    W    WIN0H     Window 0 Horizontal Dimensions
			WIN0H = val;
		}
		if (addr >= 0x4000042 && addr < 0x4000044) {
			//2    W    WIN1H     Window 1 Horizontal Dimensions 
			WIN1H = val;
		}
		if (addr >= 0x4000044 && addr < 0x4000046) {
			//2    W    WIN0V     Window 0 Vertical Dimensions 
			WIN0V = val;
		}
		if (addr >= 0x4000046 && addr < 0x4000048) {
			//2    W    WIN1V     Window 1 Vertical Dimensions 
			WIN1V = val;
		}
		if (addr >= 0x4000048 && addr < 0x400004A) {
			//2    R / W  WININ     Inside of Window 0 and 1
			WININ = val;
		}
		if (addr >= 0x400004A && addr < 0x400004C) {
			//2    R / W  WINOUT    Inside of OBJ Window & Outside of Windows 
			WINOUT = val;
		}
		if (addr >= 0x400004C && addr < 0x400004E) {
			//2    W    MOSAIC    Mosaic Size }
		}
		if (addr >= 0x400004E && addr < 0x4000050) {
			//--Not used 
		}
		if (addr >= 0x4000050 && addr < 0x4000052) {
			//2    R / W  BLDCNT    Color Special Effects Selection
		}
		if (addr >= 0x4000052 && addr < 0x4000054) {
			//2    R / W  BLDALPHA  Alpha Blending Coefficients 
		}
		if (addr >= 0x4000054 && addr < 0x4000056) {
			//2    W    BLDY      Brightness(Fade - In / Out) Coefficient 
		}
		if (addr >= 0x4000080 && addr < 0x4000082) {
			SOUNDCNT_L = val;
		}
		if (addr >= 0x4000082 && addr < 0x4000084) {
			SOUNDCNT_H = val;
		}
		if (addr >= 0x4000084 && addr < 0x4000086) {
			SOUNDCNT_X = val;
		}
		if (addr >= 0x4000088 && addr < 0x400008A) {
			SOUNDBIAS = val;
		}
		if (addr >= 0x4000100 && addr < 0x4000104) {
			TM0CNT_L = bitrange(15, 0, val);
			TM0CNT_H = bitrange(31, 16, val);
		}
		if (addr >= 0x4000104 && addr < 0x4000108) {
			TM1CNT_L = bitrange(15, 0, val);
			TM1CNT_H = bitrange(31, 16, val);
		}
		if (addr >= 0x4000108 && addr < 0x400010C) {
			TM2CNT_L = bitrange(15, 0, val);
			TM2CNT_H = bitrange(31, 16, val);
		}
		if (addr >= 0x400010C && addr < 0x4000110) {
			TM3CNT_L = bitrange(15, 0, val);
			TM3CNT_H = bitrange(31, 16, val);
		}
		if (addr >= 0x40000B0 && addr < 0x40000B4) {
			DMA0SAD = val;
		}
		if (addr >= 0x40000B4 && addr < 0x40000B8) {
			DMA0DAD = val;
		}
		if (addr >= 0x40000B8 && addr < 0x40000BC) {
			DMA0CNT_L = bitrange(15, 0, val);
			DMA0CNT_H = bitrange(31, 16, val);
		}
		if (addr >= 0x40000BC && addr < 0x40000C0) {
			DMA1SAD = val;
		}
		if (addr >= 0x40000C0 && addr < 0x40000C4) {
			DMA1DAD = val;
		}
		if (addr >= 0x40000C4 && addr < 0x40000C8) {
			DMA1CNT_L = bitrange(15, 0, val);
			DMA1CNT_H = bitrange(31, 16, val);
		}
		if (addr >= 0x40000C8 && addr < 0x40000CC) {
			DMA2SAD = val;
		}
		if (addr >= 0x40000CC && addr < 0x40000D0) {
			DMA2DAD = val;
		}
		if (addr >= 0x40000D0 && addr < 0x40000D4) {
			DMA2CNT_L = bitrange(15, 0, val);
			DMA2CNT_H = bitrange(31, 16, val);
		}
		if (addr >= 0x40000D4 && addr < 0x40000D8) {
			DMA3SAD = val;
		}
		if (addr >= 0x40000D8 && addr < 0x40000DC) {
			DMA3DAD = val;
		}
		if (addr >= 0x40000DC && addr < 0x40000E0) {
			DMA3CNT_L = bitrange(15, 0, val);
			DMA3CNT_H = bitrange(31, 16, val);
		}
		if (addr >= 0x4000200 && addr < 0x4000202) {
			//Interrupt Enable Register
			IE = val;
		}
		if (addr >= 0x4000202 && addr < 0x4000204) {
			//Interrupt Request Flags / IRQ Acknowledge
			IF &= ~val;
		}
		if (addr >= 0x4000204 && addr < 0x4000206) {
			//Game Pak Waitstate Control
			WAITCNT = val;
		}
		if (addr >= 0x4000208 && addr < 0x400020C) {
			//Interrupt Master Enable Register
			IME = val;
		}
		if (addr == 0x4000300) {
			POSTFLG = val;
		}
	}
	if (addr >= 0x05000000 && addr < 0x06000000) {
		// BG/OBJ Palette Ram

		uint32_t Index = addr - 0x05000000;
		Index %= 0x400;

		BPRAM[Index] = bitrange(7, 0, val);
		BPRAM[Index + 1] = bitrange(15, 8, val);
		BPRAM[Index + 2] = bitrange(23, 16, val);
		BPRAM[Index + 3] = bitrange(31, 24, val);

	}
	if (addr >= 0x06000000 && addr < 0x07000000) {
		// VRAM
		//addr = addr % 0x06020000;
		uint32_t Index = addr - 0x06000000;
		if (Index == 0x82E0) {
			int y = 0;
		}
		if (Index == 0xA6E0) {
			int y = 0;
		}
		if (Index > 94208) {
			int h = 0;
		}
		Index %= 0x20000;
		if (Index >= 0x18000) {
			Index -= 0x8000;
		}


		VRAM[Index] = bitrange(7, 0, val);
		VRAM[Index + 1] = bitrange(15, 8, val);
		VRAM[Index + 2] = bitrange(23, 16, val);
		VRAM[Index + 3] = bitrange(31, 24, val);
	}
	if (addr >= 0x07000000 && addr < 0x08000000) {
		// OAM - OBJ Attributes

		uint32_t Index = addr - 0x07000000;
		Index %= 0x400;

		OBJA[Index] = bitrange(7, 0, val);
		OBJA[Index + 1] = bitrange(15, 8, val);
		OBJA[Index + 2] = bitrange(23, 16, val);
		OBJA[Index + 3] = bitrange(31, 24, val);
	}
	if (addr >= 0x08000000 && addr <= 0x0DFFFFFF) {
		// Game Pak Rom - FlashRom
		uint32_t Index = addr - 0x08000000;
		Index %= 0x2000000;
		if (Index < Rom.size()) {

		}
		else
		{
			EEPROM[Index] = bitrange(7, 0, val);
			EEPROM[Index + 1] = bitrange(15, 8, val);
			EEPROM[Index + 2] = bitrange(23, 16, val);
			EEPROM[Index + 3] = bitrange(31, 24, val);
		}

	}
	if (addr >= 0x0E000000 && addr <= 0x0E00FFFF) {
		// Game Pak SRAM
		uint32_t Index = addr - 0x0E000000;
		SRAM[Index] = bitrange(7, 0, val);
		SRAM[Index + 1] = bitrange(15, 8, val);
		SRAM[Index + 2] = bitrange(23, 16, val);
		SRAM[Index + 3] = bitrange(31, 24, val);
	}

}

uint8_t MMU::MemoryRead(uint32_t addr)
{
	if (addr >= 0x00000000 && addr <= 0x00003FFF) {
		return Bios[addr];
	}
	else if (addr >= 0x02000000 && addr < 0x03000000) {
		// On board WRAM
		uint32_t Index = addr - 0x02000000;
		Index %= 0x40000;
		return EWRAM[Index];
	}
	else if (addr >= 0x03000000 && addr < 0x04000000) {
		// On chip WRAM
		uint32_t Index = addr - 0x03000000;
		Index %= 0x8000;
		return IWRAM[Index];
	}
	else if (addr >= 0x04000000 && addr <= 0x040003FE) {
		// IO Registers
		// LCD IO:
		int yaya = 0;
		int address = addr;
		if (addr >= 0x4000060 && addr <= 0x40000A8) {
			int y = 0;
		}
		if (addr >= 0x4000000 && addr < 0x4000002) {
			//2    R / W  DISPCNT   LCD Control 
			if (addr == 0x4000000) {
				return bitrange(7, 0, DISPCNT);
			}
			if (addr == 0x4000001) {
				return bitrange(15, 8, DISPCNT);
			}
		}
		if (addr >= 0x4000002 && addr < 0x4000004) {
			//2    R / W - Undocumented - Green Swap 
		}
		if (addr >= 0x4000004 && addr < 0x4000006) {
			//2    R / W  DISPSTAT  General LCD Status(STAT, LYC) 
			if (addr == 0x4000004) {
				return bitrange(7, 0, DISPSTAT);
			}
			if (addr == 0x4000005) {
				return bitrange(15, 8, DISPSTAT);
			}
		}
		if (addr >= 0x4000006 && addr < 0x4000008) {
			//2    R    VCOUNT    Vertical Counter(LY)
			if (addr == 0x4000006) {
				return bitrange(7, 0, VCOUNT);
			}
			if (addr == 0x4000007) {
				return bitrange(15, 8, VCOUNT);
			}
		}
		if (addr >= 0x4000008 && addr < 0x400000A) {
			//2    R / W  BG0CNT    BG0 Control
		}
		if (addr >= 0x400000A && addr < 0x400000C) {
			//2    R / W  BG1CNT    BG1 Control
		}
		if (addr >= 0x400000C && addr < 0x400000E) {
			//2    R / W  BG2CNT    BG2 Control 
		}
		if (addr >= 0x400000E && addr < 0x4000010) {
			//2    R / W  BG3CNT    BG3 Control 
		}
		if (addr >= 0x4000010 && addr < 0x4000012) {
			//2    W    BG0HOFS   BG0 X - Offset
		}
		if (addr >= 0x4000012 && addr < 0x4000014) {
			//2    W    BG0VOFS   BG0 Y - Offset
		}
		if (addr >= 0x4000014 && addr < 0x4000016) {
			//2    W    BG1HOFS   BG1 X - Offset
		}
		if (addr >= 0x4000016 && addr < 0x4000018) {
			//2    W    BG1VOFS   BG1 Y - Offset
		}
		if (addr >= 0x4000018 && addr < 0x400001A) {
			//2    W    BG2HOFS   BG2 X - Offset
		}
		if (addr >= 0x400001A && addr < 0x400001C) {
			//2    W    BG2VOFS   BG2 Y - Offset
		}
		if (addr >= 0x400001C && addr < 0x400001E) {
			//2    W    BG3HOFS   BG3 X - Offset
		}
		if (addr >= 0x400001E && addr < 0x4000020) {
			//2    W    BG3VOFS   BG3 Y - Offset
		}
		if (addr >= 0x4000020 && addr < 0x4000022) {
			//2    W    BG2PA     BG2 Rotation / Scaling Parameter A(dx)
		}
		if (addr >= 0x4000022 && addr < 0x4000024) {
			//2    W    BG2PB     BG2 Rotation / Scaling Parameter B(dmx)
		}
		if (addr >= 0x4000024 && addr < 0x4000026) {
			//2    W    BG2PC     BG2 Rotation / Scaling Parameter C(dy)
		}
		if (addr >= 0x4000026 && addr < 0x4000028) {
			//2    W    BG2PD     BG2 Rotation / Scaling Parameter D(dmy)
		}
		if (addr >= 0x4000028 && addr < 0x400002C) {
			//4    W    BG2X      BG2 Reference Point X - Coordinate
		}
		if (addr >= 0x400002C && addr < 0x4000030) {
			//4    W    BG2Y      BG2 Reference Point Y - Coordinate
		}
		if (addr >= 0x4000030 && addr < 0x4000032) {
			//2    W    BG3PA     BG3 Rotation / Scaling Parameter A(dx)
		}
		if (addr >= 0x4000032 && addr < 0x4000034) {
			//2    W    BG3PB     BG3 Rotation / Scaling Parameter B(dmx)
		}
		if (addr >= 0x4000034 && addr < 0x4000036) {
			//2    W    BG3PC     BG3 Rotation / Scaling Parameter C(dy)
		}
		if (addr >= 0x4000036 && addr < 0x4000038) {
			//2    W    BG3PD     BG3 Rotation / Scaling Parameter D(dmy)
		}
		if (addr >= 0x4000038 && addr < 0x400003C) {
			//4    W    BG3X      BG3 Reference Point X - Coordinate
		}
		if (addr >= 0x400003C && addr < 0x4000040) {
			//4    W    BG3Y      BG3 Reference Point Y - Coordinate
		}
		if (addr >= 0x4000040 && addr < 0x4000042) {
			//2    W    WIN0H     Window 0 Horizontal Dimensions
			int y = 0;
		}
		if (addr >= 0x4000042 && addr < 0x4000044) {
			//2    W    WIN1H     Window 1 Horizontal Dimensions 
		}
		if (addr >= 0x4000044 && addr < 0x4000046) {
			//2    W    WIN0V     Window 0 Vertical Dimensions 
		}
		if (addr >= 0x4000046 && addr < 0x4000048) {
			//2    W    WIN1V     Window 1 Vertical Dimensions 
		}
		if (addr >= 0x4000048 && addr < 0x400004A) {
			//2    R / W  WININ     Inside of Window 0 and 1
		}
		if (addr >= 0x400004A && addr < 0x400004C) {
			//2    R / W  WINOUT    Inside of OBJ Window & Outside of Windows 
		}
		if (addr >= 0x400004C && addr < 0x400004E) {
			//2    W    MOSAIC    Mosaic Size }
		}
		if (addr >= 0x400004E && addr < 0x4000050) {
			//--Not used 
		}
		if (addr >= 0x4000050 && addr < 0x4000052) {
			//2    R / W  BLDCNT    Color Special Effects Selection
			if (addr == 0x4000050) {
				return bitrange(7, 0, BLDCNT);
			}
			if (addr == 0x4000051) {
				return bitrange(15, 8, BLDCNT);
			}
		}
		if (addr >= 0x4000052 && addr < 0x4000054) {
			//2    R / W  BLDALPHA  Alpha Blending Coefficients 
			if (addr == 0x4000052) {
				return bitrange(7, 0, BLDALPHA);
			}
			if (addr == 0x4000053) {
				return bitrange(15, 8, BLDALPHA);
			}
		}
		if (addr >= 0x4000054 && addr < 0x4000056) {
			//2    W    BLDY      Brightness(Fade - In / Out) Coefficient 
			if (addr == 0x4000054) {
				return bitrange(7, 0, BLDY);
			}
			if (addr == 0x4000055) {
				return bitrange(15, 8, BLDY);
			}
		}

		if (addr >= 0x4000060 && addr < 0x40000A8) {
			int y = 0;
		}

		if (addr >= 0x4000088 && addr < 0x400008A) {
			if (addr == 0x4000088) {
				return bitrange(7, 0, SOUNDBIAS);
			}
			if (addr == 0x4000089) {
				return bitrange(15, 8, SOUNDBIAS);
			}
		}
		if (addr >= 0x4000100 && addr < 0x4000102) {
			if (addr == 0x4000100) {
				return bitrange(7, 0, Timer0Val);
			}
			if (addr == 0x4000101) {
				return bitrange(15, 8, Timer0Val);
			}
		}
		if (addr >= 0x4000102 && addr < 0x4000104) {
			if (addr == 0x4000102) {
				return bitrange(7, 0, TM0CNT_H);
			}
			if (addr == 0x4000103) {
				return bitrange(15, 8, TM0CNT_H);
			}
		}
		if (addr >= 0x4000104 && addr < 0x4000106) {
			if (addr == 0x4000104) {
				return bitrange(7, 0, Timer1Val);
			}
			if (addr == 0x4000105) {
				return bitrange(15, 8, Timer1Val);
			}
		}
		if (addr >= 0x4000106 && addr < 0x4000108) {
			if (addr == 0x4000106) {
				return bitrange(7, 0, TM1CNT_H);
			}
			if (addr == 0x4000107) {
				return bitrange(15, 8, TM1CNT_H);
			}
		}
		if (addr >= 0x4000108 && addr < 0x400010A) {
			if (addr == 0x4000108) {
				return bitrange(7, 0, Timer2Val);
			}
			if (addr == 0x4000109) {
				return bitrange(15, 8, Timer2Val);
			}
		}
		if (addr >= 0x400010A && addr < 0x400010C) {
			if (addr == 0x400010A) {
				return bitrange(7, 0, TM2CNT_H);
			}
			if (addr == 0x400010B) {
				return bitrange(15, 8, TM2CNT_H);
			}
		}
		if (addr >= 0x400010C && addr < 0x400010E) {
			if (addr == 0x400010C) {
				return bitrange(7, 0, Timer3Val);
			}
			if (addr == 0x400010D) {
				return bitrange(15, 8, Timer3Val);
			}
		}
		if (addr >= 0x400010E && addr < 0x4000110) {
			if (addr == 0x400010E) {
				return bitrange(7, 0, TM3CNT_H);
			}
			if (addr == 0x400010F) {
				return bitrange(15, 8, TM3CNT_H);
			}
		}
		if (addr >= 0x40000B0 && addr < 0x40000B4) {
			return DMA0SAD;
		}
		if (addr >= 0x40000B4 && addr < 0x40000B8) {
			return DMA0DAD;
		}
		if (addr >= 0x40000B8 && addr < 0x40000BA) {
			if (addr == 0x40000B8) {
				return bitrange(7, 0, DMA0CNT_L);
			}
			if (addr == 0x40000B9) {
				return bitrange(15, 8, DMA0CNT_L);
			}
		}
		if (addr >= 0x40000BA && addr < 0x40000BC) {
			if (addr == 0x40000BA) {
				return bitrange(7, 0, DMA0CNT_H);
			}
			if (addr == 0x40000BB) {
				return bitrange(15, 8, DMA0CNT_H);
			}
		}
		if (addr >= 0x40000BC && addr < 0x40000C0) {
			return DMA1SAD;
		}
		if (addr >= 0x40000C0 && addr < 0x40000C4) {
			return DMA1DAD;
		}
		if (addr >= 0x40000C4 && addr < 0x40000C6) {
			if (addr == 0x40000C4) {
				return bitrange(7, 0, DMA1CNT_L);
			}
			if (addr == 0x40000C5) {
				return bitrange(15, 8, DMA1CNT_L);
			}
		}
		if (addr >= 0x40000C6 && addr < 0x40000C8) {
			if (addr == 0x40000C6) {
				return bitrange(7, 0, DMA1CNT_H);
			}
			if (addr == 0x40000C7) {
				return bitrange(15, 8, DMA1CNT_H);
			}
		}
		if (addr >= 0x40000C8 && addr < 0x40000CC) {
			return DMA2SAD;
		}
		if (addr >= 0x40000CC && addr < 0x40000D0) {
			return DMA2DAD;
		}
		if (addr >= 0x40000D0 && addr < 0x40000D2) {
			if (addr == 0x40000D0) {
				return bitrange(7, 0, DMA2CNT_L);
			}
			if (addr == 0x40000D1) {
				return bitrange(15, 8, DMA2CNT_L);
			}
		}
		if (addr >= 0x40000D2 && addr < 0x40000D4) {
			if (addr == 0x40000D2) {
				return bitrange(7, 0, DMA2CNT_H);
			}
			if (addr == 0x40000D3) {
				return bitrange(15, 8, DMA2CNT_H);
			}
		}
		if (addr >= 0x40000D4 && addr < 0x40000D8) {
			return DMA3SAD;
		}
		if (addr >= 0x40000D8 && addr < 0x40000DC) {
			return DMA3DAD;
		}
		if (addr >= 0x40000DC && addr < 0x40000DE) {
			if (addr == 0x40000DC) {
				return bitrange(7, 0, DMA3CNT_L);
			}
			if (addr == 0x40000DD) {
				return bitrange(15, 8, DMA3CNT_L);
			}
		}
		if (addr >= 0x40000DE && addr < 0x40000E0) {
			if (addr == 0x40000DE) {
				return bitrange(7, 0, DMA3CNT_H);
			}
			if (addr == 0x40000DF) {
				return bitrange(15, 8, DMA3CNT_H);
			}
		}
		if (addr >= 0x4000130 && addr < 0x4000132) {
			//KEYINPUT - Key Status (R)
			if (addr == 0x4000130) {
				return bitrange(7, 0, KEYINPUT);
			}
			if (addr == 0x4000131) {
				return bitrange(15, 8, KEYINPUT);
			}
		}
		if (addr >= 0x4000200 && addr < 0x4000202) {
			//Interrupt Enable Register
			if (addr == 0x4000200) {
				return bitrange(7, 0, IE);
			}
			if (addr == 0x4000201) {
				return bitrange(15, 8, IE);
			}
		}
		if (addr >= 0x4000202 && addr < 0x4000204) {
			//Interrupt Request Flags / IRQ Acknowledge
			if (addr == 0x4000202) {
				return bitrange(7, 0, IF);
			}
			if (addr == 0x4000203) {
				return bitrange(15, 8, IF);
			}
		}
		if (addr >= 0x4000204 && addr < 0x4000206) {
			//Game Pak Waitstate Control
			if (addr == 0x4000204) {
				return bitrange(7, 0, WAITCNT);
			}
			if (addr == 0x4000205) {
				return bitrange(15, 8, WAITCNT);
			}
		}
		if (addr >= 0x4000208 && addr < 0x400020A) {
			//Interrupt Master Enable Register
			if (addr == 0x4000208) {
				return bitrange(7, 0, IME);
			}
			if (addr == 0x4000209) {
				return bitrange(15, 8, IME);
			}
		}
		if (addr >= 0x400020A && addr < 0x4000300) {
			return 0;
		}
		if (addr == 0x4000300) {
			return POSTFLG;
		}
		if (addr == 0x4000301) {
			return HALTCNT;
		}
	}
	else if (addr >= 0x05000000 && addr <= 0x050003FF) {
		// BG/OBJ Palette Ram
		uint32_t Index = addr - 0x05000000;
		return BPRAM[Index];
	}
	else if (addr >= 0x06000000 && addr < 0x07000000) {
		// VRAM
		uint32_t Index = addr - 0x06000000;
		Index %= 0x20000;
		if (Index >= 0x18000) {
			Index -= 0x8000;
		}

		return VRAM[Index];
	}
	else if (addr >= 0x07000000 && addr <= 0x070003FF) {
		// OAM - OBJ Attributes
		uint32_t Index = addr - 0x07000000;
		Index %= 0x400;
		return OBJA[Index];
	}
	else if (addr >= 0x08000000 && addr < 0x0E000000) {
		// Game Pak Rom - FlashRom
		uint32_t Index = addr - 0x08000000;
		Index %= 0x2000000;
		if (Index < Rom.size()) {
			return Rom[Index];
		}
		else
		{
			Index -= Rom.size();
			return EEPROM[Index];
		}
	}
	else if (addr >= 0x0E000000 && addr <= 0x0E00FFFF) {
		// Game Pak SRAM
		uint32_t Index = addr - 0x0E000000;
		return SRAM[Index];
		int ya = 0;
	}
	else
	{
		int huh = 0;
		return 0;
	}
}

uint32_t MMU::MemoryReadWord(uint32_t addr)
{
	int ya = 1;
	if (addr >= 0x00000000 && addr <= 0x00003FFF) {
		return Bios[addr + 3] << 24 | Bios[addr + 2] << 16 | Bios[addr + 1] << 8 | Bios[addr];
	}
	else if (addr >= 0x02000000 && addr < 0x03000000) {
		// On board WRAM
		uint32_t Index = addr - 0x02000000;
		Index %= 0x40000;
		return EWRAM[Index + 3] << 24 | EWRAM[Index + 2] << 16 | EWRAM[Index + 1] << 8 | EWRAM[Index];
	}
	else if (addr >= 0x03000000 && addr < 0x04000000) {
		// On chip WRAM
		uint32_t Index = addr - 0x03000000;
		Index %= 0x8000;
		return IWRAM[Index + 3] << 24 | IWRAM[Index + 2] << 16 | IWRAM[Index + 1] << 8 | IWRAM[Index];
	}
	else if (addr >= 0x04000000 && addr <= 0x040003FE) {
		// IO Registers
		// LCD IO:
		int yaya = 0;
		int address = addr;
		if (addr >= 0x4000060 && addr <= 0x40000A8) {
			int y = 0;
		}
		if (addr >= 0x4000000 && addr < 0x4000002) {
			//2    R / W  DISPCNT   LCD Control 
			return static_cast<uint32_t>(DISPCNT);
		}
		if (addr >= 0x4000002 && addr < 0x4000004) {
			//2    R / W - Undocumented - Green Swap 
		}
		if (addr >= 0x4000004 && addr < 0x4000006) {
			//2    R / W  DISPSTAT  General LCD Status(STAT, LYC) 
			return DISPSTAT;
		}
		if (addr >= 0x4000006 && addr < 0x4000008) {
			//2    R    VCOUNT    Vertical Counter(LY)
			return VCOUNT;
		}
		if (addr >= 0x4000008 && addr < 0x400000A) {
			//2    R / W  BG0CNT    BG0 Control
			return BG0CNT;
		}
		if (addr >= 0x400000A && addr < 0x400000C) {
			//2    R / W  BG1CNT    BG1 Control
			return BG1CNT;
		}
		if (addr >= 0x400000C && addr < 0x400000E) {
			//2    R / W  BG2CNT    BG2 Control 
			return BG2CNT;
		}
		if (addr >= 0x400000E && addr < 0x4000010) {
			//2    R / W  BG3CNT    BG3 Control 
			return BG3CNT;
		}
		if (addr >= 0x4000010 && addr < 0x4000012) {
			//2    W    BG0HOFS   BG0 X - Offset
			return BG0HOFS;
		}
		if (addr >= 0x4000012 && addr < 0x4000014) {
			//2    W    BG0VOFS   BG0 Y - Offset
			return BG0VOFS;
		}
		if (addr >= 0x4000014 && addr < 0x4000016) {
			//2    W    BG1HOFS   BG1 X - Offset
			return BG1HOFS;
		}
		if (addr >= 0x4000016 && addr < 0x4000018) {
			//2    W    BG1VOFS   BG1 Y - Offset
			return BG1VOFS;
		}
		if (addr >= 0x4000018 && addr < 0x400001A) {
			//2    W    BG2HOFS   BG2 X - Offset
			return BG2HOFS;
		}
		if (addr >= 0x400001A && addr < 0x400001C) {
			//2    W    BG2VOFS   BG2 Y - Offset
			return BG2VOFS;
		}
		if (addr >= 0x400001C && addr < 0x400001E) {
			//2    W    BG3HOFS   BG3 X - Offset
			return BG3HOFS;
		}
		if (addr >= 0x400001E && addr < 0x4000020) {
			//2    W    BG3VOFS   BG3 Y - Offset
			return BG3VOFS;
		}
		if (addr >= 0x4000020 && addr < 0x4000022) {
			//2    W    BG2PA     BG2 Rotation / Scaling Parameter A(dx)
		}
		if (addr >= 0x4000022 && addr < 0x4000024) {
			//2    W    BG2PB     BG2 Rotation / Scaling Parameter B(dmx)
		}
		if (addr >= 0x4000024 && addr < 0x4000026) {
			//2    W    BG2PC     BG2 Rotation / Scaling Parameter C(dy)
		}
		if (addr >= 0x4000026 && addr < 0x4000028) {
			//2    W    BG2PD     BG2 Rotation / Scaling Parameter D(dmy)
		}
		if (addr >= 0x4000028 && addr < 0x400002C) {
			//4    W    BG2X      BG2 Reference Point X - Coordinate
		}
		if (addr >= 0x400002C && addr < 0x4000030) {
			//4    W    BG2Y      BG2 Reference Point Y - Coordinate
		}
		if (addr >= 0x4000030 && addr < 0x4000032) {
			//2    W    BG3PA     BG3 Rotation / Scaling Parameter A(dx)
		}
		if (addr >= 0x4000032 && addr < 0x4000034) {
			//2    W    BG3PB     BG3 Rotation / Scaling Parameter B(dmx)
		}
		if (addr >= 0x4000034 && addr < 0x4000036) {
			//2    W    BG3PC     BG3 Rotation / Scaling Parameter C(dy)
		}
		if (addr >= 0x4000036 && addr < 0x4000038) {
			//2    W    BG3PD     BG3 Rotation / Scaling Parameter D(dmy)
		}
		if (addr >= 0x4000038 && addr < 0x400003C) {
			//4    W    BG3X      BG3 Reference Point X - Coordinate
		}
		if (addr >= 0x400003C && addr < 0x4000040) {
			//4    W    BG3Y      BG3 Reference Point Y - Coordinate
		}
		if (addr >= 0x4000040 && addr < 0x4000042) {
			//2    W    WIN0H     Window 0 Horizontal Dimensions
			return WIN0H;
		}
		if (addr >= 0x4000042 && addr < 0x4000044) {
			//2    W    WIN1H     Window 1 Horizontal Dimensions 
			return WIN1H;
		}
		if (addr >= 0x4000044 && addr < 0x4000046) {
			//2    W    WIN0V     Window 0 Vertical Dimensions 
			return WIN0V;
		}
		if (addr >= 0x4000046 && addr < 0x4000048) {
			//2    W    WIN1V     Window 1 Vertical Dimensions 
			return WIN1V;
		}
		if (addr >= 0x4000048 && addr < 0x400004A) {
			//2    R / W  WININ     Inside of Window 0 and 1
			return WININ;
		}
		if (addr >= 0x400004A && addr < 0x400004C) {
			//2    R / W  WINOUT    Inside of OBJ Window & Outside of Windows 
			return WINOUT;
		}
		if (addr >= 0x400004C && addr < 0x400004E) {
			//2    W    MOSAIC    Mosaic Size }
		}
		if (addr >= 0x400004E && addr < 0x4000050) {
			//--Not used 
		}
		if (addr >= 0x4000050 && addr < 0x4000052) {
			//2    R / W  BLDCNT    Color Special Effects Selection
		}
		if (addr >= 0x4000052 && addr < 0x4000054) {
			//2    R / W  BLDALPHA  Alpha Blending Coefficients 
		}
		if (addr >= 0x4000054 && addr < 0x4000056) {
			//2    W    BLDY      Brightness(Fade - In / Out) Coefficient 
		}

		if (addr >= 0x4000060 && addr < 0x40000A8) {
			int y = 0;
		}

		if (addr >= 0x4000088 && addr < 0x400008A) {
			return SOUNDBIAS;
		}
		if (addr >= 0x4000100 && addr < 0x4000104) {
			return TM0CNT_H << 16 | Timer0Val;
		}
		if (addr >= 0x4000104 && addr < 0x4000108) {
			return TM1CNT_H << 16 | Timer1Val;
		}
		if (addr >= 0x4000108 && addr < 0x400010C) {
			return TM2CNT_H << 16 | Timer2Val;
		}
		if (addr >= 0x400010C && addr < 0x4000110) {
			return TM3CNT_H << 16 | Timer3Val;
		}
		if (addr >= 0x4000130 && addr < 0x4000132) {
			//KEYINPUT - Key Status (R)
			return KEYINPUT;
		}
		if (addr >= 0x40000B0 && addr < 0x40000B4) {
			return DMA0SAD;
		}
		if (addr >= 0x40000B4 && addr < 0x40000B8) {
			return DMA0DAD;
		}
		if (addr >= 0x40000B8 && addr < 0x40000BC) {
			return DMA0CNT_H << 16 | DMA0CNT_L;
		}
		if (addr >= 0x40000BC && addr < 0x40000C0) {
			return DMA1SAD;
		}
		if (addr >= 0x40000C0 && addr < 0x40000C4) {
			return DMA1DAD;
		}
		if (addr >= 0x40000C4 && addr < 0x40000C8) {
			return DMA1CNT_H << 16 | DMA1CNT_L;
		}
		if (addr >= 0x40000C8 && addr < 0x40000CC) {
			return DMA2SAD;
		}
		if (addr >= 0x40000CC && addr < 0x40000D0) {
			return DMA2DAD;
		}
		if (addr >= 0x40000D0 && addr < 0x40000D4) {
			return DMA2CNT_H << 16 | DMA2CNT_L;
		}
		if (addr >= 0x40000D4 && addr < 0x40000D8) {
			return DMA3SAD;
		}
		if (addr >= 0x40000D8 && addr < 0x40000DC) {
			return DMA3DAD;
		}
		if (addr >= 0x40000DC && addr < 0x40000E0) {
			return DMA3CNT_H << 16 | DMA3CNT_L;
		}
		if (addr >= 0x4000200 && addr < 0x4000202) {
			//Interrupt Enable Register
			return IE;
		}
		if (addr >= 0x4000202 && addr < 0x4000204) {
			//Interrupt Request Flags / IRQ Acknowledge
			return IF;
		}
		if (addr >= 0x4000204 && addr < 0x4000206) {
			//Game Pak Waitstate Control
			return WAITCNT;
		}
		if (addr >= 0x4000208 && addr < 0x400020A) {
			//Interrupt Master Enable Register
			return IME;
		}
	}
	else if (addr >= 0x05000000 && addr <= 0x050003FF) {
		// BG/OBJ Palette Ram
		uint32_t Index = addr - 0x05000000;
		return BPRAM[Index + 3] << 24 | BPRAM[Index + 2] << 16 | BPRAM[Index + 1] << 8 | BPRAM[Index];
	}
	else if (addr >= 0x06000000 && addr < 0x07000000) {
		// VRAM
		uint32_t Index = addr - 0x06000000;
		Index %= 0x20000;
		if (Index >= 0x18000) {
			Index -= 0x8000;
		}


		return VRAM[Index + 3] << 24 | VRAM[Index + 2] << 16 | VRAM[Index + 1] << 8 | VRAM[Index];
	}
	else if (addr >= 0x07000000 && addr <= 0x070003FF) {
		// OAM - OBJ Attributes
		uint32_t Index = addr - 0x07000000;
		Index %= 0x400;


		return OBJA[Index + 3] << 24 | OBJA[Index + 2] << 16 | OBJA[Index + 1] << 8 | OBJA[Index];
	}
	else if (addr >= 0x08000000 && addr < 0x0E000000) {
		// Game Pak Rom - FlashRom
		uint32_t Index = addr - 0x08000000;
		Index %= 0x2000000;
		if (Index < Rom.size()) {
			return Rom[Index + 3] << 24 | Rom[Index + 2] << 16 | Rom[Index + 1] << 8 | Rom[Index];
		}
		else
		{
			return EEPROM[Index + 3] << 24 | EEPROM[Index + 2] << 16 | EEPROM[Index + 1] << 8 | EEPROM[Index];
		}
	}
	else if (addr >= 0x0E000000 && addr <= 0x0E00FFFF) {
		// Game Pak SRAM
		int ya = 0;
	}
	else
	{
		int huh = 0;
		return 0;
	}
}


void MMU::Load()
{
	std::ifstream rom;
	std::streampos size;
	char* buffer;

	rom.open("arm.gba", std::ios::ate | std::ios::binary);
	if (rom.is_open()) {
		size = rom.tellg();
		buffer = new char[size];
		//Rom.resize(size, 0);
		rom.seekg(0, std::ios::beg);
		rom.read(buffer, size);
		rom.close();

		for (int i = 0; i < size; i++)
		{
			Rom[i] = buffer[i];
		}
		delete[] buffer;
	}
	rom.open("gba_bios.bin", std::ios::ate | std::ios::binary);
	if (rom.is_open()) {
		size = rom.tellg();
		buffer = new char[size];
		rom.seekg(0, std::ios::beg);
		rom.read(buffer, size);
		rom.close();

		for (int i = 0; i < size; i++)
		{
			Bios[i] = buffer[i];
		}
		delete[] buffer;
	}
}
