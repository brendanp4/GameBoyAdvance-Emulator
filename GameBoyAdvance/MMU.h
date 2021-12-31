#pragma once
#include <cstdint>
#include <vector>
#include <array>

class MMU
{
	// Memory mapping

	// General Internal Memory

	// 0x00000000 - 0x00003FFF   BIOS - System ROM(16 KBytes)
	// 0x00004000 - 0x01FFFFFF   Not used
	// 0x02000000 - 0x0203FFFF   WRAM - On - board Work RAM(256 KBytes) 2 Wait
	// 0x02040000 - 0x02FFFFFF   Not used
	// 0x03000000 - 0x03007FFF   WRAM - In - chip Work RAM(32 KBytes)
	// 0x03008000 - 0x03FFFFFF   Not used
	// 0x04000000 - 0x040003FE   I / O Registers
	// 0x04000400 - 0x04FFFFFF   Not used

	// Internal Display Memory

	// 0x05000000 - 0x050003FF   BG / OBJ Palette RAM(1 Kbyte)
	// 0x05000400 - 0x05FFFFFF   Not used
	// 0x06000000 - 0x06017FFF   VRAM - Video RAM(96 KBytes)
	// 0x06018000 - 0x06FFFFFF   Not used
	// 0x07000000 - 0x070003FF   OAM - OBJ Attributes(1 Kbyte)
	// 0x07000400 - 0x07FFFFFF   Not used

	// External Memory(Game Pak)

	// 0x08000000 - 0x09FFFFFF   Game Pak ROM / FlashROM(max 32MB) - Wait State 0
	// 0x0A000000 - 0x0BFFFFFF   Game Pak ROM / FlashROM(max 32MB) - Wait State 1
	// 0x0C000000 - 0x0DFFFFFF   Game Pak ROM / FlashROM(max 32MB) - Wait State 2
	// 0x0E000000 - 0x0E00FFFF   Game Pak SRAM(max 64 KBytes) - 8bit Bus width
	// 0x0E010000 - 0x0FFFFFFF   Not used

	// Unused Memory Area

	// 0x10000000 - 0xFFFFFFFF   Not used(upper 4bits of address bus unused)

public:
	uint8_t Bios[0x3FFF];
	std::vector<uint8_t> Rom;
	//std::vector<uint8_t> EWRAM;
	//std::vector<uint8_t> IWRAM;
	//std::vector<uint8_t> BPRAM;
	//std::vector<uint8_t> VRAM;
	//std::vector<uint8_t> OBJA;
	//std::vector<uint8_t> SRAM;
	std::vector<uint8_t> EEPROM;


	std::array<uint8_t, 0x40000> EWRAM;
	std::array<uint8_t, 0x8000> IWRAM;
	std::array<uint8_t, 0x400> BPRAM;
	std::array<uint8_t, 0x18010> VRAM;
	std::array<uint8_t, 0x400> OBJA;
	std::array<uint8_t, 0x10000> SRAM;


	//std::vector<uint8_t> EEPROM;

	// I/O Registers

	// GFX
	uint16_t DISPCNT = 0;
	uint16_t DISPSTAT = 0;
	uint16_t VCOUNT = 0;
	uint16_t BG0CNT = 0;
	uint16_t BG1CNT = 0;
	uint16_t BG2CNT = 0;
	uint16_t BG3CNT = 0;
	uint16_t BG0HOFS = 0;
	uint16_t BG0VOFS = 0;
	uint16_t BG1HOFS = 0;
	uint16_t BG1VOFS = 0;
	uint16_t BG2HOFS = 0;
	uint16_t BG2VOFS = 0;
	uint16_t BG3HOFS = 0;
	uint16_t BG3VOFS = 0;

	uint16_t BG2PA = 0;
	uint16_t BG2PB = 0;
	uint16_t BG2PC = 0;
	uint16_t BG2PD = 0;
	uint32_t BG2X = 0;
	uint32_t BG2Y = 0;

	uint16_t BG3PA = 0;
	uint16_t BG3PB = 0;
	uint16_t BG3PC = 0;
	uint16_t BG3PD = 0;
	uint32_t BG3X = 0;
	uint32_t BG3Y = 0;

	uint16_t WIN0H = 0;
	uint16_t WIN1H = 0;
	uint16_t WIN0V = 0;
	uint16_t WIN1V = 0;
	uint16_t WININ = 0;
	uint16_t WINOUT = 0;

	uint16_t MOSAIC = 0;
	uint16_t BLDCNT = 0;
	uint16_t BLDALPHA = 0;
	uint16_t BLDY = 0;

	// Sound

	uint16_t SOUNDCNT_L = 0;
	uint16_t SOUNDCNT_H = 0;
	uint16_t SOUNDCNT_X = 0;

	uint16_t SOUNDBIAS = 0x200;

	// Timers
	uint16_t TM0CNT_L = 0;
	uint16_t TM0CNT_H = 0;
	uint16_t Timer0Val = 0;

	uint16_t TM1CNT_L = 0;
	uint16_t TM1CNT_H = 0;
	uint16_t Timer1Val = 0;

	uint16_t TM2CNT_L = 0;
	uint16_t TM2CNT_H = 0;
	uint16_t Timer2Val = 0;

	uint16_t TM3CNT_L = 0;
	uint16_t TM3CNT_H = 0;
	uint16_t Timer3Val = 0;

	// Keypad
	uint16_t KEYINPUT = 0;
	uint16_t KEYCNT = 0;

	// DMA
	uint32_t DMA0SAD = 0;
	uint32_t DMA0DAD = 0;
	uint16_t DMA0CNT_L = 0;
	uint16_t DMA0CNT_H = 0;
	uint32_t DMA1SAD = 0;
	uint32_t DMA1DAD = 0;
	uint16_t DMA1CNT_L = 0;
	uint16_t DMA1CNT_H = 0;
	uint32_t DMA2SAD = 0;
	uint32_t DMA2DAD = 0;
	uint16_t DMA2CNT_L = 0;
	uint16_t DMA2CNT_H = 0;
	uint32_t DMA3SAD = 0;
	uint32_t DMA3DAD = 0;
	uint16_t DMA3CNT_L = 0;
	uint16_t DMA3CNT_H = 0;

	//Interrupt Control
	uint8_t POSTFLG = 0;
	uint8_t HALTCNT = 0;
	uint16_t IE = 0;
	uint16_t IF = 0;
	uint16_t IME = 0;
	uint16_t WAITCNT = 0;



public:
	MMU();
	uint32_t bitrange(int msb, int lsb, uint32_t insn);
	void MemoryWrite(uint32_t addr, uint8_t val);
	void MemoryWrite(uint32_t addr, uint16_t val);
	void MemoryWrite(uint32_t addr, uint32_t val);
	uint8_t MemoryRead(uint32_t addr);
	uint32_t MemoryReadWord(uint32_t addr);
	void Load();

};

