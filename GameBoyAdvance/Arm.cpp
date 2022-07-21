#include "Arm.h"
#include <fstream>
#include <bitset>
#include <iostream>



Arm::Arm()
{
	SetState();
	for (int i = 0; i <= 30; i++) {
		registers[i] = 0;
	}
	registers[1] = 0x000000EA;
	registers[18] = 0x03007F00;
	registers[22] = 0x03007FA0;
	registers[19] = 0x03007FE0;
	registers[30] = 0x08000000;


	//CPSR = 0x6000001f;
}

void Arm::WriteToLogA(int nCount)
{
	std::ofstream oFile;
	std::string output = "\n-----------------------------------------------\n";;
	output += "Frame: " + std::to_string(nCount);
	output += "\n" + std::to_string(Ainsn) + "\n";
	output += "\nRegister 0: " + std::to_string(registers[0]);
	output += "\nRegister 1: " + std::to_string(registers[1]);
	output += "\nRegister 2: " + std::to_string(registers[2]);
	output += "\nRegister 3: " + std::to_string(registers[3]);
	output += "\nRegister 4: " + std::to_string(registers[4]);
	output += "\nRegister 5: " + std::to_string(registers[5]);
	output += "\nRegister 6: " + std::to_string(registers[6]);
	output += "\nRegister 7: " + std::to_string(registers[7]);
	output += "\nRegister 8: " + std::to_string(registers[8]);
	output += "\nRegister 9: " + std::to_string(registers[9]);
	output += "\nRegister 10: " + std::to_string(registers[10]);
	output += "\nRegister 11: " + std::to_string(registers[11]);
	output += "\nRegister 12: " + std::to_string(registers[12]);
	output += "\nRegister 13: " + std::to_string(registers[13]);
	output += "\nRegister 14: " + std::to_string(registers[14]);
	output += "\nRegister 15: " + std::to_string(registers[15]);
	output += "\nRegister 16: " + std::to_string(registers[16]);
	output += "\nRegister 17: " + std::to_string(registers[17]);
	output += "\nRegister 18: " + std::to_string(registers[18]);
	output += "\nRegister 19: " + std::to_string(registers[19]);
	output += "\nRegister 20: " + std::to_string(registers[20]);
	output += "\nRegister 21: " + std::to_string(registers[21]);
	output += "\nRegister 22: " + std::to_string(registers[22]);
	output += "\nRegister 23: " + std::to_string(registers[23]);
	output += "\nRegister 24: " + std::to_string(registers[24]);
	output += "\nRegister 25: " + std::to_string(registers[25]);
	output += "\nRegister 26: " + std::to_string(registers[26]);
	output += "\nRegister 27: " + std::to_string(registers[27]);
	output += "\nRegister 28: " + std::to_string(registers[28]);
	output += "\nRegister 29: " + std::to_string(registers[29]);
	output += "\nRegister 30: " + std::to_string(registers[30]);
	output += "\nCPSR: " + std::to_string(CPSR);
	output += "\n-----------------------------------------------";
	output += "\n\n";
	oFile.open("decode.txt", std::ios::out | std::ios::app);
	oFile << output;
	oFile.close();
}

void Arm::WriteToLogB(int nCount)
{
	std::ofstream oFile;
	std::string output = "\n-----------------------------------------------\n";;
	output += "Frame: " + std::to_string(nCount);
	output += "\n" + std::to_string(Ainsn) + "\n";
	output += "\nRegister 0: " + std::to_string(registers[0]);
	output += "\nRegister 1: " + std::to_string(registers[1]);
	output += "\nRegister 2: " + std::to_string(registers[2]);
	output += "\nRegister 3: " + std::to_string(registers[3]);
	output += "\nRegister 4: " + std::to_string(registers[4]);
	output += "\nRegister 5: " + std::to_string(registers[5]);
	output += "\nRegister 6: " + std::to_string(registers[6]);
	output += "\nRegister 7: " + std::to_string(registers[7]);
	output += "\nRegister 8: " + std::to_string(registers[8]);
	output += "\nRegister 9: " + std::to_string(registers[9]);
	output += "\nRegister 10: " + std::to_string(registers[10]);
	output += "\nRegister 11: " + std::to_string(registers[11]);
	output += "\nRegister 12: " + std::to_string(registers[12]);
	output += "\nRegister 13: " + std::to_string(registers[13]);
	output += "\nRegister 14: " + std::to_string(registers[14]);
	output += "\nRegister 15: " + std::to_string(registers[15]);
	output += "\nRegister 16: " + std::to_string(registers[16]);
	output += "\nRegister 17: " + std::to_string(registers[17]);
	output += "\nRegister 18: " + std::to_string(registers[18]);
	output += "\nRegister 19: " + std::to_string(registers[19]);
	output += "\nRegister 20: " + std::to_string(registers[20]);
	output += "\nRegister 21: " + std::to_string(registers[21]);
	output += "\nRegister 22: " + std::to_string(registers[22]);
	output += "\nRegister 23: " + std::to_string(registers[23]);
	output += "\nRegister 24: " + std::to_string(registers[24]);
	output += "\nRegister 25: " + std::to_string(registers[25]);
	output += "\nRegister 26: " + std::to_string(registers[26]);
	output += "\nRegister 27: " + std::to_string(registers[27]);
	output += "\nRegister 28: " + std::to_string(registers[28]);
	output += "\nRegister 29: " + std::to_string(registers[29]);
	output += "\nRegister 30: " + std::to_string(registers[30]);
	output += "\nCPSR: " + std::to_string(CPSR);
	output += "\n-----------------------------------------------";
	output += "\n\n";
	oFile.open("decodeArm.txt", std::ios::out | std::ios::app);
	oFile << output;
	oFile.close();
}

void Arm::Load(MMU& mmu)
{
	std::ifstream rom;
	std::streampos size;
	char* buffer;

	rom.open("C:\\Users\\brend\\Desktop\\roms\\obj_demo.gba", std::ios::ate | std::ios::binary);
	if (rom.is_open()) {
		size = rom.tellg();
		buffer = new char[size];
		mmu.Rom.resize(size, 0);
		rom.seekg(0, std::ios::beg);
		rom.read(buffer, size);
		rom.close();

		for (int i = 0; i < size; i++)
		{
			mmu.Rom[i] = buffer[i];
		}
		delete[] buffer;
	}
	rom.open("C:\\Users\\brend\\Desktop\\roms\\gba_bios.bin", std::ios::ate | std::ios::binary);
	if (rom.is_open()) {
		size = rom.tellg();
		buffer = new char[size];
		rom.seekg(0, std::ios::beg);
		rom.read(buffer, size);
		rom.close();

		for (int i = 0; i < size; i++)
		{
			mmu.Bios[i] = buffer[i];
		}
		delete[] buffer;
	}
	int romSize = mmu.Rom.size();
	int eep = 0x2000000 - romSize;
	//int eep = 0x1FFF;
	mmu.EEPROM.resize(eep, 0xFF);
	GenerateTable(mmu);
	loaded = true;
}

void Arm::Cycle(PPU& ppu, MMU& mmu)
{
	if (frame % 25000 == 0) {
		fifoEmpty = true;
	}
	else
	{
		fifoEmpty = false;
	}

	bExecute = true;
	SetMode();
	SetState();
	ReadCpsr();
	increment = true;


	if (frame == 1083680) {
		Dummy();
	}

	if (frame % 4 == 0) {
		if (registers[0] == 0x69696969) {
			Dummy();
		}
		if (state == State::Arm) {


			Ainsn = FetchArm(mmu);

			DecodeArm(mmu);
			//Decode(Ainsn, ppu, mmu);

			if (registers[30] % 2 != 0) {
				std::cerr << "\nArm\nInvalid PC\nRegister 30: " << registers[30] << std::endl;
				registers[30] = registers[30] & 0xFFFFFFFC;
			}

			if (increment) {
				registers[30] += 4;
			}
		}
		else
		{

			//Tinsn = FetchThumb(mmu);

			DecodeThumb(FetchThumb(mmu), mmu);
			if (registers[30] % 2 != 0) {
				std::cerr << "\nThumb\nInvalid PC\nRegister 30: " << registers[30] << std::endl;
				registers[30] = registers[30] & 0xFFFFFFFC;
			}
			SetState();
			if (increment) {
				if (state == State::Arm) {
					registers[30] += 4;
				}
				else
				{
					registers[30] += 2;
				}
			}
		}

		DMATransfer(mmu, ppu);

	}

	
	Timer(mmu);
	Interrupt(mmu, ppu);

	

	frame++;
}

void Arm::AlignAddr(uint32_t & addr)
{
	if (addr % 4 != 0) {
		addr -= 1;
		AlignAddr(addr);
	}
}


void Arm::SetState()
{
	if ((CPSR >> 5) & 1) {
		state = State::Thumb;
	}
	else
	{
		state = State::Arm;
	}
}

uint32_t Arm::FetchArm(MMU& mmu)
{
	//uint32_t PC = registers[30];
	//return (mmu.MemoryRead(registers[30] + 3) << 24) | (mmu.MemoryRead(registers[30] + 2) << 16) | (mmu.MemoryRead(registers[30] + 1) << 8) | mmu.MemoryRead(registers[30]);
	uint32_t ARM_INSTRUCTION = mmu.MemoryReadWord(registers[30]);
	return ARM_INSTRUCTION;
}

uint16_t Arm::FetchThumb(MMU& mmu)
{
	//uint32_t PC = registers[30];
	//return (mmu.MemoryRead(registers[30] + 1) << 8) | mmu.MemoryRead(registers[30]);
	return static_cast<uint16_t>(bitrange(15, 0, mmu.MemoryReadWord(registers[30])));
}

void Arm::ReadCpsr()
{
	if ((CPSR >> 31) & 1) {
		N = true;
	}
	else
	{
		N = false;
	}
	if ((CPSR >> 30) & 1) {
		Z = true;
	}
	else
	{
		Z = false;
	}
	if ((CPSR >> 29) & 1) {
		C = true;
	}
	else
	{
		C = false;
	}
	if ((CPSR >> 28) & 1) {
		V = true;
	}
	else
	{
		V = false;
	}
}

void Arm::Interrupt(MMU & mmu, PPU & ppu)
{

	if (mmu.VCOUNT == 80) {
		Dummy();
	}
	bool InterruptRequest = false;

	if (!(CPSR >> 7 & 1)) {
		if (mmu.IME & 1) {
			Dummy();
			if (!(mmu.IF & 1) && (mmu.IE & 1)) {
				//VBLANK Interrupt
				if (ppu.vBlankIrq && ppu.vBlankEntered) {
					ppu.vBlankEntered = false;
					SetBit(mmu.IF, 0);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 1 & 1) && (mmu.IE >> 1 & 1)) {
				//HBLANK Interrupt
				if (ppu.hBlankIrq && ppu.hBlank) {
					ppu.hBlank = false;
					SetBit(mmu.IF, 1);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 2 & 1) && (mmu.IE >> 2 & 1)) {
				//VCOUNT Interrupt
				if (ppu.vCountIrq && ppu.vCount) {
					ppu.vCount = false;
					SetBit(mmu.IF, 2);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 3 & 1) && (mmu.IE >> 3 & 1)) {
				//Timer 0 Overflow Interrupt
				if (Timer0Irq) {
					Timer0Irq = false;
					SetBit(mmu.IF, 3);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 4 & 1) && (mmu.IE >> 4 & 1)) {
				//Timer 1 Overflow Interrupt
				if (Timer1Irq) {
					Timer1Irq = false;
					SetBit(mmu.IF, 4);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 5 & 1) && (mmu.IE >> 5 & 1)) {
				//Timer 2 Overflow Interrupt
				if (Timer2Irq) {
					Timer2Irq = false;
					SetBit(mmu.IF, 5);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 6 & 1) && (mmu.IE >> 6 & 1)) {
				//Timer 3 Overflow Interrupt
				if (Timer3Irq) {
					Timer3Irq = false;
					SetBit(mmu.IF, 6);
					InterruptRequest = true;
				}
			}
			if ((mmu.IF >> 7 & 1) && (mmu.IE >> 7 & 1)) {
				//Serial Interrupt
			}
			if (!(mmu.IF >> 8 & 1) && (mmu.IE >> 8 & 1)) {
				//DMA 0 Interrupt
				if (DMA0irq) {
					DMA0irq = false;
					SetBit(mmu.IF, 8);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 9 & 1) && (mmu.IE >> 9 & 1)) {
				//DMA 1 Interrupt
				if (DMA1irq) {
					DMA1irq = false;
					SetBit(mmu.IF, 9);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 10 & 1) && (mmu.IE >> 10 & 1)) {
				//DMA 2 Interrupt
				if (DMA2irq) {
					DMA2irq = false;
					SetBit(mmu.IF, 10);
					InterruptRequest = true;
				}
			}
			if (!(mmu.IF >> 11 & 1) && (mmu.IE >> 11 & 1)) {
				//DMA 3 Interrupt
				if (DMA3irq) {
					DMA3irq = false;
					SetBit(mmu.IF, 11);
					InterruptRequest = true;
				}
			}
			if ((mmu.IF >> 12 & 1) && (mmu.IE >> 12 & 1)) {
				//Keypad Interrupt
			}
			if ((mmu.IF >> 13 & 1) && (mmu.IE >> 13 & 1)) {
				//Game Pak (external IRQ source)
			}

			if (InterruptRequest) {
				CallInterrupt();
			}


		}
	}
}

void Arm::CallInterrupt()
{
	if (state == State::Arm) {
		registers[28] = registers[30];
	}
	else
	{
		registers[28] = registers[30];
	}


	irqd = true;

	SPSR_irq = CPSR;
	uint32_t mask = 0x000000FF;
	CPSR &= ~mask;
	CPSR |= 0b10010;
	ClearBit(CPSR, 5);
	SetBit(CPSR, 7);
	registers[30] = 0x18;
	increment = false;
	//curInsnAsm = "IRQ";
}

void Arm::SetBit(uint32_t & val, int bit)
{
	val |= (1 << bit);
}

void Arm::SetBit(uint16_t & val, int bit)
{
	val |= (1 << bit);
}

void Arm::ClearBit(uint32_t & val, int bit)
{
	val &= ~(1 << bit);
}

void Arm::ClearBit(uint16_t & val, int bit)
{
	val &= ~(1 << bit);
}

bool Arm::Condition(uint32_t insn)
{
	switch (bitrange(31, 28, insn))
	{
	case 0x0:
		if (Z) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x1:
		if (!Z) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x2:
		if (C) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x3:
		if (!C) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x4:
		if (N) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x5:
		if (!N) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x6:
		if (V) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x7:
		if (!V) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x8:
		if (C && !Z) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0x9:
		if (!C || Z) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0xA:
		if (N == V) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0xB:
		if (N != V) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0xC:
		if (!Z && N == V) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0xD:
		if (Z || N != V) {
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 0xE:
		return true;
		break;
	case 0xF:
		return false;
		break;
	}
}

void Arm::SetMode()
{
	switch (CPSR & 0b00000000000000000000000000011111)
	{
	case 0b10000:
		mode = Mode::User;
		break;
	case 0b10001:
		mode = Mode::Fiq;
		break;
	case 0b10010:
		mode = Mode::Irq;
		break;
	case 0b10011:
		mode = Mode::Supervisor;
		break;
	case 0b10111:
		mode = Mode::Abort;
		break;
	case 0b11011:
		mode = Mode::Undefined;
		break;
	case 0b11111:
		mode = Mode::System;
		break;
	default:
		mode = Mode::User;
		break;
	}
}



void Arm::DMATransfer(MMU & mmu, PPU & ppu)
{
	if (!((mmu.DMA0CNT_H >> 15) & 1)) {
		needs_reload0 = true;
	}
	if (!((mmu.DMA1CNT_H >> 15) & 1)) {
		needs_reload1 = true;
	}
	if (!((mmu.DMA2CNT_H >> 15) & 1)) {
		needs_reload2 = true;
	}
	if (!((mmu.DMA3CNT_H >> 15) & 1)) {
		needs_reload3 = true;
	}

	//DMA0
	if (mmu.DMA0CNT_H >> 15 & 1) {
		Dummy();
		//DMA Transfer enabled
		bool word = false;
		bool repeat = false;
		bool drq = false; //DMA3 only
		bool irq = false;
		int destCnt = 0;
		int srcCnt = 0;
		int startTiming = 0;
		uint32_t wordCount = 0;
		uint32_t startAddr = 0;
		uint32_t destAddr = 0;
		bool dInc = false;
		bool dDec = false;
		bool dFix = false;
		bool dIR = false;
		bool sInc = false;
		bool sDec = false;
		bool sFix = false;
		//DMA Transfer enabled

		if (word) {
			if (mmu.DMA0SAD % 4 != 0) {
				AlignAddr(mmu.DMA0SAD);
			}
			if (mmu.DMA0DAD % 4 != 0) {
				AlignAddr(mmu.DMA0DAD);
			}
		}
		else
		{
			if (mmu.DMA0SAD % 2 != 0) {
				mmu.DMA0SAD -= 1;
			}
			if (mmu.DMA0DAD % 2 != 0) {
				mmu.DMA0DAD -= 1;
			}
		}

		if (dma_0_addr_src == 0 || needs_reload0) {
			dma_0_addr_src = mmu.DMA0SAD;
		}
		if (dma_0_addr_dest == 0 || needs_reload0) {
			dma_0_addr_dest = mmu.DMA0DAD;
		}





		//startAddr = mmu.DMA0SAD;
		//destAddr = mmu.DMA0DAD;

		wordCount = mmu.DMA0CNT_L;
		startTiming = bitrange(13, 12, mmu.DMA0CNT_H);
		bool bStart = false;
		switch (startTiming)
		{
		case 0:
			// Immediate
			bStart = true;
			break;
		case 1:
			// vBlank
			if (ppu.vBlank) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		case 2:
			// hBlank
			if (ppu.hBlank && ppu.curScanLine < 161) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		case 3:
			// Prohibited DMA0
			break;
		}
		//if (startAddr % 4 != 0) {
		//	Dummy();
		//}
		//if (destAddr % 4 != 0) {
		//	Dummy();
		//}
		if (wordCount == 0) {
			wordCount = 0x4000;
		}
		//if (startAddr % 2 != 0) {
		//	startAddr -= 1;
		//}
		//if (destAddr % 2 != 0) {
		//	destAddr -= 1;
		//}

		//AlignAddr(startAddr);
		//AlignAddr(destAddr);


		if (dma_0_addr_src == 0 && dma_0_addr_dest == 0) {
			bStart = false;
		}
		destCnt = bitrange(6, 5, mmu.DMA0CNT_H);
		srcCnt = bitrange(8, 7, mmu.DMA0CNT_H);
		if (mmu.DMA3CNT_H >> 9 & 1) {
			repeat = true;
		}
		if (mmu.DMA3CNT_H >> 10 & 1) {
			word = true;
		}
		if (mmu.DMA3CNT_H >> 11 & 1) {
			drq = true;
		}
		if (bStart) {
			for (uint32_t i = 0; i < wordCount; i++) {
				// Transfer
				if (word) {

					uint32_t val = mmu.MemoryReadWord(dma_0_addr_src);
					mmu.MemoryWrite(dma_0_addr_dest, val);
				}
				else
				{
					uint16_t val = mmu.MemoryRead(dma_0_addr_src + 1) << 8 | mmu.MemoryRead(dma_0_addr_src);
					mmu.MemoryWrite(dma_0_addr_dest, val);
				}
				switch (destCnt)
				{
				case 0:
					//
					dInc = true;
					if (word) dma_0_addr_dest += 4;
					else dma_0_addr_dest += 2;
					break;
				case 1:
					//
					dDec = true;
					if (word) dma_0_addr_dest -= 4;
					else dma_0_addr_dest -= 2;
					break;
				case 2:
					//
					dFix = true;
					break;
				case 3:
					//
					dIR = true;
					if (word) dma_0_addr_dest += 4;
					else dma_0_addr_dest += 2;
					break;
				}
				switch (srcCnt)
				{
				case 0:
					//
					sInc = true;
					if (word) dma_0_addr_src += 4;
					else dma_0_addr_src += 2;
					break;
				case 1:
					//
					sDec = true;
					if (word) dma_0_addr_src -= 4;
					else dma_0_addr_src -= 2;
					break;
				case 2:
					//
					sFix = true;
					break;
				}
			}
			if (repeat && dIR) {
				dma_0_addr_dest = mmu.DMA0DAD;
			}
			if (mmu.DMA0CNT_H >> 14 & 1) {
				DMA0irq = true;
			}
			if (!repeat) {
				ClearBit(mmu.DMA0CNT_H, 15);
			}
			for (int i = 0; i < 5; i++) {
				ClearBit(mmu.DMA0CNT_H, i);
			}
			ppu.hBlank = false;
			needs_reload0 = false;
		}

	}
	//DMA1
	if (mmu.DMA1CNT_H >> 15 & 1) {
		Dummy();
		//DMA Transfer enabled
		//DMA Transfer enabled
		bool fifo = false;
		bool word = false;
		bool repeat = false;
		bool drq = false; //DMA3 only
		bool irq = false;
		int destCnt = 0;
		int srcCnt = 0;
		int startTiming = 0;
		uint32_t wordCount = 0;
		uint32_t startAddr = 0;
		uint32_t destAddr = 0;
		bool dInc = false;
		bool dDec = false;
		bool dFix = false;
		bool dIR = false;
		bool sInc = false;
		bool sDec = false;
		bool sFix = false;
		//DMA Transfer enabled
		bool bStart = false;


		startTiming = bitrange(13, 12, mmu.DMA1CNT_H);
		switch (startTiming)
		{
		case 0:
			// Immediate
			bStart = true;
			break;
		case 1:
			// vBlank
			if (ppu.vBlank) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		case 2:
			// hBlank
			if (ppu.hBlank && ppu.curScanLine < 161) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		case 3:
			// FIFO mode
			fifo = true;
			if (fifoEmpty) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		}


		if (mmu.DMA1SAD % 4 != 0) {
			AlignAddr(mmu.DMA1SAD);
		}
		if (mmu.DMA1DAD % 4 != 0) {
			AlignAddr(mmu.DMA1DAD);
		}

		if (dma_1_addr_src == 0 || needs_reload1) {
			dma_1_addr_src = mmu.DMA1SAD;
		}
		if (dma_1_addr_dest == 0 || needs_reload1) {
			dma_1_addr_dest = mmu.DMA1DAD;
		}


		//startAddr = mmu.DMA1SAD;
		//destAddr = mmu.DMA1DAD;
		wordCount = mmu.DMA1CNT_L;
		if (wordCount == 0) {
			wordCount = 0x4000;
		}
		if (fifo) {
			wordCount = 4;
		}
		//if (startAddr % 4 != 0) {
		//	Dummy();
		//}
		//if (destAddr % 4 != 0) {
		//	Dummy();
		//}
		//if (startAddr % 2 != 0) {
		//	startAddr -= 1;
		//}
		//if (destAddr % 2 != 0) {
		//	destAddr -= 1;
		//}




		if (dma_1_addr_src == 0 && dma_1_addr_dest == 0) {
			bStart = false;
		}
		destCnt = bitrange(6, 5, mmu.DMA1CNT_H);
		srcCnt = bitrange(8, 7, mmu.DMA1CNT_H);
		if (mmu.DMA1CNT_H >> 9 & 1) {
			repeat = true;
		}
		if (mmu.DMA1CNT_H >> 10 & 1) {
			word = true;
		}
		if (mmu.DMA1CNT_H >> 11 & 1) {
			drq = true;
		}

		if (bStart) {
			for (uint32_t i = 0; i < wordCount; i++) {
				// Transfer
				if (word) {

					uint32_t val = mmu.MemoryReadWord(dma_1_addr_src);
					mmu.MemoryWrite(dma_1_addr_dest, val);
				}
				else
				{
					uint16_t val = mmu.MemoryRead(dma_1_addr_src + 1) << 8 | mmu.MemoryRead(dma_1_addr_src);
					mmu.MemoryWrite(dma_1_addr_dest, val);
				}
				if (!fifo) {
					switch (destCnt)
					{
					case 0:
						//
						dInc = true;
						if (word) dma_1_addr_dest += 4;
						else dma_1_addr_dest += 2;
						break;
					case 1:
						//
						dDec = true;
						if (word) dma_1_addr_dest -= 4;
						else dma_1_addr_dest -= 2;
						break;
					case 2:
						//
						dFix = true;
						break;
					case 3:
						//
						dIR = true;
						if (word) dma_1_addr_dest += 4;
						else dma_1_addr_dest += 2;
						break;
					}
				}
				switch (srcCnt)
				{
				case 0:
					//
					sInc = true;
					if (word) dma_1_addr_src += 4;
					else dma_1_addr_src += 2;
					break;
				case 1:
					//
					sDec = true;
					if (word) dma_1_addr_src -= 4;
					else dma_1_addr_src -= 2;
					break;
				case 2:
					//
					sFix = true;
					break;
				}
			}
			if (repeat && dIR) {
				dma_1_addr_dest = mmu.DMA1DAD;
			}
			if (mmu.DMA1CNT_H >> 14 & 1) {
				DMA1irq = true;
			}
			if (!repeat) {
				ClearBit(mmu.DMA1CNT_H, 15);
			}
			for (int i = 0; i < 5; i++) {
				ClearBit(mmu.DMA1CNT_H, i);
			}
			ppu.hBlank = false;
			needs_reload1 = false;
		}

	}
	//DMA2
	if (mmu.DMA2CNT_H >> 15 & 1) {
		Dummy();
		//DMA Transfer enabled
		//DMA Transfer enabled
		bool fifo = false;
		bool word = false;
		bool repeat = false;
		bool drq = false; //DMA3 only
		bool irq = false;
		int destCnt = 0;
		int srcCnt = 0;
		int startTiming = 0;
		uint32_t wordCount = 0;
		uint32_t startAddr = 0;
		uint32_t destAddr = 0;
		bool dInc = false;
		bool dDec = false;
		bool dFix = false;
		bool dIR = false;
		bool sInc = false;
		bool sDec = false;
		bool sFix = false;
		//DMA Transfer enabled
		bool bStart = false;


		startTiming = bitrange(13, 12, mmu.DMA2CNT_H);
		switch (startTiming)
		{
		case 0:
			// Immediate
			bStart = true;
			break;
		case 1:
			// vBlank
			if (ppu.vBlank) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		case 2:
			// hBlank
			if (ppu.hBlank && ppu.curScanLine < 161) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		case 3:
			// FIFO
			fifo = true;
			if (fifoEmpty) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		}

		if (mmu.DMA2SAD % 4 != 0) {
			AlignAddr(mmu.DMA2SAD);
		}
		if (mmu.DMA2DAD % 4 != 0) {
			AlignAddr(mmu.DMA2DAD);
		}

		if (dma_2_addr_src == 0 || needs_reload2) {
			dma_2_addr_src = mmu.DMA2SAD;
		}
		if (dma_2_addr_dest == 0 || needs_reload2) {
			dma_2_addr_dest = mmu.DMA2DAD;
		}


		//startAddr = mmu.DMA2SAD;
		//destAddr = mmu.DMA2DAD;
		wordCount = mmu.DMA2CNT_L;
		if (wordCount == 0) {
			wordCount = 0x4000;
		}
		if (fifo) {
			wordCount = 4;
		}
		//if (startAddr % 4 != 0) {
		//	Dummy();
		//}
		//if (destAddr % 4 != 0) {
		//	Dummy();
		//}
		//if (startAddr % 2 != 0) {
		//	startAddr -= 1;
		//}
		//if (destAddr % 2 != 0) {
		//	destAddr -= 1;
		//}

		if (dma_2_addr_src == 0 && dma_2_addr_dest == 0) {
			bStart = false;
		}

		destCnt = bitrange(6, 5, mmu.DMA2CNT_H);
		srcCnt = bitrange(8, 7, mmu.DMA2CNT_H);
		if (mmu.DMA2CNT_H >> 9 & 1) {
			repeat = true;
		}
		if (mmu.DMA2CNT_H >> 10 & 1) {
			word = true;
		}
		if (mmu.DMA2CNT_H >> 11 & 1) {
			drq = true;
		}
		if (bStart) {
			for (uint32_t i = 0; i < wordCount; i++) {
				// Transfer
				if (word) {

					uint32_t val = mmu.MemoryReadWord(dma_2_addr_src);
					mmu.MemoryWrite(dma_2_addr_dest, val);
				}
				else
				{
					uint16_t val = mmu.MemoryRead(dma_2_addr_src + 1) << 8 | mmu.MemoryRead(dma_2_addr_src);
					mmu.MemoryWrite(dma_2_addr_dest, val);
				}
				if (!fifo) {
					switch (destCnt)
					{
					case 0:
						//
						dInc = true;
						if (word) dma_2_addr_dest += 4;
						else dma_2_addr_dest += 2;
						break;
					case 1:
						//
						dDec = true;
						if (word) dma_2_addr_dest -= 4;
						else dma_2_addr_dest -= 2;
						break;
					case 2:
						//
						dFix = true;
						break;
					case 3:
						//
						dIR = true;
						if (word) dma_2_addr_dest += 4;
						else dma_2_addr_dest += 2;
						break;
					}
				}
				switch (srcCnt)
				{
				case 0:
					//
					sInc = true;
					if (word) dma_2_addr_src += 4;
					else dma_2_addr_src += 2;
					break;
				case 1:
					//
					sDec = true;
					if (word) dma_2_addr_src -= 4;
					else dma_2_addr_src -= 2;
					break;
				case 2:
					//
					sFix = true;
					break;
				}
			}

			if (repeat && dIR) {
				dma_2_addr_dest = mmu.DMA2DAD;
			}
			if (mmu.DMA2CNT_H >> 14 & 1) {
				DMA2irq = true;
			}
			if (!repeat) {
				ClearBit(mmu.DMA2CNT_H, 15);
			}
			for (int i = 0; i < 5; i++) {
				ClearBit(mmu.DMA2CNT_H, i);
			}
			ppu.hBlank = false;

			needs_reload2 = false;
		}

	}
	//DMA3
	if (mmu.DMA3CNT_H >> 15 & 1) {
		Dummy();
		bool needsReload = true;

		//DMA local variables
		bool repeat = false;
		if (mmu.DMA3CNT_H >> 9 & 1) {
			repeat = true;
		}
		bool word = false;
		if (mmu.DMA3CNT_H >> 10 & 1) {
			word = true;
		}
		bool drq = false; //DMA3 only
		bool irq = false;
		int destCnt = 0;
		int srcCnt = 0;
		int startTiming = bitrange(13, 12, mmu.DMA3CNT_H);
		uint32_t wordCount = 0;
		uint32_t startAddr = 0;
		uint32_t destAddr = 0;
		bool dInc = false;
		bool dDec = false;
		bool dFix = false;
		bool dIR = false;
		bool sInc = false;
		bool sDec = false;
		bool sFix = false;

		bool eeprom_access = false;
		bool eeprom_read = false;
		bool eeprom_write = false;
		uint64_t eeprom_data = 0;
		uint16_t eeprom_val = 0;
		uint32_t eeprom_src = 0;

		//DMA Transfer enabled
		bool bStart = false;
		switch (startTiming)
		{
		case 0:
			// Immediate
			bStart = true;
			break;
		case 1:
			// vBlank
			if (ppu.vBlank) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		case 2:
			// hBlank
			if (ppu.hBlank && ppu.curScanLine < 161) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		case 3:
			// Video Capture
			if (ppu.curScanLine >= 2 && ppu.curScanLine < 162) {
				bStart = true;
			}
			else
			{
				bStart = false;
			}
			break;
		}


		//Set source and destination address

		//Memory Alignment
		if (word) {
			if (mmu.DMA3SAD % 4 != 0) {
				AlignAddr(mmu.DMA3SAD);
			}
			if (mmu.DMA3DAD % 4 != 0) {
				AlignAddr(mmu.DMA3DAD);
			}
		}
		else
		{
			if (mmu.DMA3SAD % 2 != 0) {
				mmu.DMA3SAD -= 1;
			}
			if (mmu.DMA3DAD % 2 != 0) {
				mmu.DMA3DAD -= 1;
			}
		}

		if (dma_3_addr_src == 0 || needs_reload3) {
			dma_3_addr_src = mmu.DMA3SAD;
		}
		if (dma_3_addr_dest == 0 || needs_reload3) {
			dma_3_addr_dest = mmu.DMA3DAD;
		}

		if (mmu.DMA3DAD == 0x600F0C0 || mmu.DMA3DAD == 0x600F0C2) {
			Dummy();
		}
		if (dma_3_addr_dest == 0x600F0C0 || dma_3_addr_dest == 0x600F0C2) {
			Dummy();
		}

		Dummy();

		if (dma_3_addr_dest >= 0xD000000) {
			eeprom_access = true;
		}
		if (dma_3_addr_src >= 0xD000000) {
			eeprom_access = true;
		}


		//destAddr = mmu.DMA3DAD;


		//Align addresses
		//if (dma_3_addr_src % 4 != 0) {
		//	AlignAddr(dma_3_addr_src);
		//}
		//if (mmu.DMA3DAD % 4 != 0) {
		//	AlignAddr(mmu.DMA3DAD);
		//}
		//if (dma_3_addr_src % 2 != 0) {
		//	startAddr -= 1;
		//}
		//if (destAddr % 2 != 0) {
		//	destAddr -= 1;
		//}

		//Set word count
		wordCount = bitrange(13, 0, mmu.DMA3CNT_L);
		if (wordCount == 0) {
			wordCount = 0x10000;
		}

		if (dma_3_addr_src == 0 && dma_3_addr_dest == 0) {
			bStart = false;
		}

		if (eeprom_access && (wordCount == 17 || wordCount == 81)) {
			eeprom_address = 0;
		}

		if (eeprom_access && wordCount == 17) {
			//std::cout << "EEPROM Set Read Address\nSRC Address: " << dma_3_addr_src << "\nDEST Address: " << dma_3_addr_dest << "\n\n";
		}
		if (eeprom_access && wordCount == 68) {
			//std::cout << "EEPROM Read Data\nSRC Address: " << dma_3_addr_src << "\nDEST Address: " << dma_3_addr_dest << "\n\n";
		}
		if (eeprom_access && wordCount == 81) {
			//std::cout << "EEPROM Write Data\nSRC Address: " << dma_3_addr_src << "\nDEST Address: " << dma_3_addr_dest << "\n\n";
			//epWrites++;
			//if (epWrites == 3) {
			//	Dummy();
			//}
		}

		//if (eeprom_access) {
		//	if (wordCount != 17 && wordCount != 68 && wordCount != 81) {
		//		Dummy();
		//	}
		//}

		destCnt = bitrange(6, 5, mmu.DMA3CNT_H);
		srcCnt = bitrange(8, 7, mmu.DMA3CNT_H);

		int testNum = 0;

		if (mmu.DMA3CNT_H >> 11 & 1) {
			drq = true;
		}
		Dummy();
		if (bStart) {
			dmas++;
			eeprom_src = dma_3_addr_src;
			for (uint32_t i = 0; i < wordCount; i++) {
				// Transfer
				if (word) {

					uint32_t val = mmu.MemoryReadWord(dma_3_addr_src);
					mmu.MemoryWrite(dma_3_addr_dest, val);
				}
				else
				{
					if (eeprom_access) {
						Dummy();
						switch (wordCount)
						{
						case 17:
							Dummy();
							//EEPROM Set Read Address
							if (i >= 2 && i <= 15) {
								if (i == 11) {
									Dummy();
								}
								uint16_t val = mmu.MemoryRead(dma_3_addr_src + 1) << 8 | mmu.MemoryRead(dma_3_addr_src);
								eeprom_address = eeprom_address << 1;
								eeprom_address |= (val & 1);
							}
							else if(i == 16)
							{
								eeprom_address *= 8;
								eeprom_address += 0xD000000;
							}
							break;
						case 68:
							//EEPROM Read Data
						{
							Dummy();
							if (i < 4) {
								//eeprom_address++;
								//dma_3_addr_dest -= 1;
							}
							if (i >= 4) {
								testNum++;
								int index = (i - 4) % 16;
								uint16_t val = mmu.MemoryRead(eeprom_address + 1) << 8 | mmu.MemoryRead(eeprom_address);
								
								uint16_t nVal = (val >> index) & 1;
								
								//val = val & 1;
								mmu.MemoryWrite(dma_3_addr_dest, nVal);
								if (i > 4 && ((i - 3) % 16) == 0) {
									eeprom_address += 2;
								}
							}
							
							break;
						}
						case 81:
							//EEPROM Write Data
							Dummy();
							if (i >= 2 && i <= 15) {
								//testNum++;
								if (i == 11) {
									Dummy();
								}
								uint16_t val = mmu.MemoryRead(dma_3_addr_src + 1) << 8 | mmu.MemoryRead(dma_3_addr_src);
								eeprom_address = eeprom_address << 1;
								eeprom_address |= (val & 1);
								//dma_3_addr_src -= 2;
							}
							else if (i >= 16) {
								//testNum++;
								Dummy();
								int index = (i - 16) % 16;
								if (i == 16) {
									eeprom_address *= 8;
									eeprom_address += 0xD000000;
								}

								uint16_t val = (mmu.MemoryRead(dma_3_addr_src + 1) << 8 | mmu.MemoryRead(dma_3_addr_src));
								eeprom_val = eeprom_val << 1;
								eeprom_val |= (val & 1);

								//if (i != 80) {
								//	mmu.MemoryWrite(eeprom_address, val);
								//}

								//eeprom_val = 0;
								//eeprom_address += 2;

								if (i > 20 && (((i - 15) % 16) == 0) && i != 80) {
									eeprom_val = ReverseBits(eeprom_val);
									mmu.MemoryWrite(eeprom_address, eeprom_val);
									eeprom_val = 0;
									eeprom_address += 2;
									eeprom_src += 2;
								}

							}
							break;
						}
					}
					else
					{
						uint16_t val = mmu.MemoryRead(dma_3_addr_src + 1) << 8 | mmu.MemoryRead(dma_3_addr_src);
						mmu.MemoryWrite(dma_3_addr_dest, val);
					}
					
				}
				switch (destCnt)
				{
				case 0:
					if (word) dma_3_addr_dest += 4;
					else dma_3_addr_dest += 2;
					break;
				case 1:
					if (word) dma_3_addr_dest -= 4;
					else dma_3_addr_dest -= 2;
					break;
				case 2:
					dFix = true;
					break;
				case 3:
					dIR = true;
					if (word) dma_3_addr_dest += 4;
					else dma_3_addr_dest += 2;
					break;
				}
				switch (srcCnt)
				{
				case 0:
					if (word) dma_3_addr_src += 4;
					else dma_3_addr_src += 2;
					break;
				case 1:
					if (word) dma_3_addr_src -= 4;
					else dma_3_addr_src -= 2;
					break;
				case 2:
					//
					sFix = true;
					break;
				}
			}
			if (mmu.DMA3CNT_H >> 14 & 1) {
				DMA3irq = true;
			}
			if (repeat && dIR) {
				dma_3_addr_dest = mmu.DMA3DAD;
			}
			if (!repeat) {
				//dma_3_addr_src = 0;
				ClearBit(mmu.DMA3CNT_H, 15);
			}
			for (int i = 0; i < 5; i++) {
				ClearBit(mmu.DMA3CNT_H, i);
			}
			ClearBit(mmu.DISPSTAT, 1);
			ppu.hBlank = false;

			needs_reload3 = false;
		}

	}
}

void Arm::Timer(MMU & mmu)
{
	Timer0Cnt = mmu.TM0CNT_H;
	Timer1Cnt = mmu.TM1CNT_H;
	Timer2Cnt = mmu.TM2CNT_H;
	Timer3Cnt = mmu.TM3CNT_H;

	Timer0ReloadVal = mmu.TM0CNT_L;
	Timer1ReloadVal = mmu.TM1CNT_L;
	Timer2ReloadVal = mmu.TM2CNT_L;
	Timer3ReloadVal = mmu.TM3CNT_L;


	if (Timer0Cnt >> 7 & 1) {
		if (Timer0 == 0) {
			Timer0 = Timer0ReloadVal;
		}
		Timer0Irq = false;
		switch (bitrange(1, 0, Timer0Cnt))
		{
		case 0:
			//Frequency F/1
			Timer0++;
			break;
		case 1:
			//Frequency F/64
			if (frame % 64 == 0) {
				Timer0++;
			}
			break;
		case 2:
			//Frequency F/256
			if (frame % 256 == 0) {
				Timer0++;
			}
			break;
		case 3:
			//Frequency F/1024
			if (frame % 1024 == 0) {
				Timer0++;
			}
			break;
		}

		if (Timer0 == 0xFFFF) {
			if (Timer0Cnt >> 6 & 1) {
				Timer0Irq = true;
			}
			Timer0OverFlow = true;
			Timer0 = Timer0ReloadVal;
		}
		else
		{
			Timer0OverFlow = false;
		}


	}
	else
	{
		Timer0 = Timer0ReloadVal;
	}

	if (Timer1Cnt >> 7 & 1) {
		if (Timer1 == 0) {
			Timer1 = Timer1ReloadVal;
		}
		Timer1Irq = false;
		if (Timer1Cnt >> 2 & 1) {
			if (Timer0OverFlow) {
				Timer1++;
			}
		}
		else
		{
			switch (bitrange(1, 0, Timer1Cnt))
			{
			case 0:
				//Frequency F/1
				Timer1++;
				break;
			case 1:
				//Frequency F/64
				if (frame % 64 == 0) {
					Timer1++;
				}
				break;
			case 2:
				//Frequency F/256
				if (frame % 256 == 0) {
					Timer1++;
				}
				break;
			case 3:
				//Frequency F/1024
				if (frame % 1024 == 0) {
					Timer1++;
				}
				break;
			}
		}

		if (Timer1 == 0xFFFF) {
			if (Timer1Cnt >> 6 & 1) {
				Timer1Irq = true;
			}
			Timer1OverFlow = true;
			Timer1 = Timer1ReloadVal;
		}
		else
		{
			Timer1OverFlow = false;
		}
	}
	else
	{
		Timer1 = Timer1ReloadVal;
	}

	if (Timer2Cnt >> 7 & 1) {
		if (Timer2 == 0) {
			Timer2 = Timer2ReloadVal;
		}
		Timer2Irq = false;
		if (Timer2Cnt >> 2 & 1) {
			if (Timer1OverFlow) {
				Timer2++;
			}
		}
		else
		{
			switch (bitrange(1, 0, Timer2Cnt))
			{
			case 0:
				//Frequency F/1
				Timer2++;
				break;
			case 1:
				//Frequency F/64
				if (frame % 64 == 0) {
					Timer2++;
				}
				break;
			case 2:
				//Frequency F/256
				if (frame % 256 == 0) {
					Timer2++;
				}
				break;
			case 3:
				//Frequency F/1024
				if (frame % 1024 == 0) {
					Timer2++;
				}
				break;
			}
		}

		if (Timer2 == 0xFFFF) {
			if (Timer2Cnt >> 6 & 1) {
				Timer2Irq = true;
			}
			Timer2OverFlow = true;
			Timer2 = Timer2ReloadVal;
		}
		else
		{
			Timer2OverFlow = false;
		}
	}
	else
	{
		Timer2 = Timer2ReloadVal;
	}

	if (Timer3Cnt >> 7 & 1) {
		if (Timer3 == 0) {
			Timer3 = Timer3ReloadVal;
		}
		Timer3Irq = false;
		if (Timer3Cnt >> 2 & 1) {
			if (Timer2OverFlow) {
				Timer3++;
			}
		}
		else
		{
			switch (bitrange(1, 0, Timer3Cnt))
			{
			case 0:
				//Frequency F/1
				Timer3++;
				break;
			case 1:
				//Frequency F/64
				if (frame % 64 == 0) {
					Timer3++;
				}
				break;
			case 2:
				//Frequency F/256
				if (frame % 256 == 0) {
					Timer3++;
				}
				break;
			case 3:
				//Frequency F/1024
				if (frame % 1024 == 0) {
					Timer3++;
				}
				break;
			}
		}

		if (Timer3 == 0xFFFF) {
			if (Timer3Cnt >> 6 & 1) {
				Timer3Irq = true;
			}
			Timer3OverFlow = true;
			Timer3 = Timer1ReloadVal;
		}
		else
		{
			Timer3OverFlow = false;
		}
	}
	else
	{
		Timer3 = Timer3ReloadVal;
	}


	mmu.Timer0Val = Timer0;
	mmu.Timer1Val = Timer1;
	mmu.Timer2Val = Timer2;
	mmu.Timer3Val = Timer3;
}

bool Arm::Overflow(uint32_t val1, uint32_t val2, uint32_t result, bool add)
{
	bool overflowed = false;
	bool neg1 = ((val1 >> 31) & 1);
	bool neg2 = ((val2 >> 31) & 1);
	bool negR = ((result >> 31) & 1);
	if (add) {
		if (!neg1 && !neg2 && negR) {
			overflowed = true;
		}
		if (neg1 && neg2 && !negR) {
			overflowed = true;
		}
	}
	else
	{
		if (!neg1 && neg2 && negR) {
			overflowed = true;
		}
		if (neg1 && !neg2 && !negR) {
			overflowed = true;
		}
	}
	return overflowed;
}

void Arm::DecodeThumb(uint16_t insn, MMU& mmu)
{
	if (registers[0] == 0x4BDE398F && registers[1] == 0x9E2FA863 && CPSR == 0xB0000000) {
		Dummy();
	}
	if (registers[1] == 0xFF) {
		Dummy();
	}
	Dummy();
	if (bitrange(15, 13, insn) == 0b000) {
		//Thumb 1 Shift
		int Rd = bitrange(2, 0, insn);
		int Rs = bitrange(5, 3, insn);
		int Rn = bitrange(8, 6, insn);
		uint32_t imm = bitrange(8, 6, insn);
		uint32_t offset = bitrange(10, 6, insn);
		switch (bitrange(12, 11, insn))
		{
		case 0b00:
			// LSL
		{
			uint64_t test = static_cast<uint64_t>(registers[Rs]);
			registers[Rd] = registers[Rs] << offset;
			//curInsnAsm = "LSL";
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}

			test = test << offset;
			if ((test >> 32) & 1) {
				SetBit(CPSR, 29);
			}
			else
			{
				ClearBit(CPSR, 29);
			}
		}
		break;
		case 0b01:
			// LSR
		{
			if (registers[1] == 0x21736590 && registers[2] == 0x20) {
				Dummy();
			}
			uint32_t test = registers[Rd];
			bool carry = false;
			registers[Rd] = registers[Rs] >> offset;
			//curInsnAsm = "LSR";
			if (offset == 0) {
				registers[Rd] = 0;
				if (registers[Rs] >> 31 & 1) {
					carry = true;
				}
			}
			if (offset >= 32) {
				registers[Rd] = 0;
			}
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (((test >> (offset - 1)) & 1) || carry) {
				SetBit(CPSR, 29);
			}
			else
			{
				ClearBit(CPSR, 29);
			}
			if (offset == 0) {
				//if (C) {
				//	SetBit(CPSR, 29);
				//}
				//else
				//{
				//	ClearBit(CPSR, 29);
				//}
			}
			break;
		}
		case 0b10:
			// ASR
		{
			bool carry = false;
			uint32_t test = registers[Rd];
			uint32_t op2 = registers[Rs];
			if (offset == 32) {
				Dummy();
			}
			registers[Rd] = arithmeticRight(registers[Rs], offset);
			if (offset == 0 || offset == 32) {
				registers[Rd] = 0;
				//registers[Rd] = ~0;
				if (test >> 31 & 1) {
					carry = true;
					registers[Rd] = ~0;
				}
			}
			if (offset > 32) {
				registers[Rd] = 0;
				//if (registers[Rs] >> 31 & 1) {
				//	carry = true;
				//}
			}
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (((test >> (offset - 1)) & 1) || carry) {
				SetBit(CPSR, 29);
			}
			else
			{
				ClearBit(CPSR, 29);
			}
			//curInsnAsm = "ASR";
			break;
		}
		case 0b11:
			// Thumb 2 Add/Subtract
			switch (bitrange(10, 9, insn))
			{
			case 0b00:
				// Add register
			{
				uint64_t tst = uint64_t(registers[Rs]) + uint64_t(registers[Rn]);
				uint32_t Op1 = registers[Rs];
				uint32_t Op2 = registers[Rn];
				uint32_t result = Op1 + Op2;
				bool poss = false;
				if (registers[Rs] > 0 || registers[Rn] > 0) {
					poss = true;
				}
				registers[Rd] = registers[Rs] + registers[Rn];
				//curInsnAsm = "ADD";
				if (registers[Rd] >> 31 & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				if (registers[Rd] == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}

				if ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)) {
					SetBit(CPSR, 29);
				}
				else
				{
					ClearBit(CPSR, 29);
				}
				if (Overflow(Op1, Op2, result, true)) {
					SetBit(CPSR, 28);
				}
				else
				{
					ClearBit(CPSR, 28);
				}
			}
			break;
			case 0b01:
				// Sub register
			{
				uint32_t Op1 = registers[Rs];
				uint32_t Op2 = registers[Rn];
				uint32_t result = Op1 - Op2;
				registers[Rd] = registers[Rs] - registers[Rn];
				SetBit(CPSR, 29);
				//curInsnAsm = "SUB";
				if (registers[Rd] >> 31 & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				if (registers[Rd] == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if (Op2 > Op1) {
					ClearBit(CPSR, 29);
				}
				if (Overflow(Op1, Op2, result, false)) {
					SetBit(CPSR, 28);
				}
				else
				{
					ClearBit(CPSR, 28);
				}
				break;
			}
			case 0b10:
				// Add immediate
			{
				uint64_t tst = uint64_t(registers[Rs]) + uint64_t(imm);
				uint32_t Op1 = registers[Rs];
				uint32_t Op2 = imm;
				uint32_t result = Op1 + Op2;
				bool poss = false;
				if (registers[Rs] > 0 || imm > 0) {
					poss = true;
				}
				registers[Rd] = registers[Rs] + imm;
				//curInsnAsm = "ADD";
				if (registers[Rd] >> 31 & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				if (registers[Rd] == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)) {
					SetBit(CPSR, 29);
				}
				else
				{
					ClearBit(CPSR, 29);
				}
				if (Overflow(Op1, Op2, result, true)) {
					SetBit(CPSR, 28);
				}
				else
				{
					ClearBit(CPSR, 28);
				}
			}
			break;
			case 0b11:
				// Sub immediate
			{
				uint32_t Op1 = registers[Rs];
				uint32_t Op2 = imm;
				uint32_t result = Op1 - Op2;
				SetBit(CPSR, 29);
				registers[Rd] = registers[Rs] - imm;
				//curInsnAsm = "SUB";
				if (registers[Rd] >> 31 & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				if (registers[Rd] == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if (Op2 > Op1) {
					ClearBit(CPSR, 29);
				}
				if (Overflow(Op1, Op2, result, false)) {
					SetBit(CPSR, 28);
				}
				else
				{
					ClearBit(CPSR, 28);
				}
				break;
			}
			break;
			}
		}
	}
	if (bitrange(15, 13, insn) == 0b001) {
		//Thumb 3 - move/compare/add/subtract immediate
		int Rd = bitrange(10, 8, insn);
		uint32_t imm = bitrange(7, 0, insn);
		switch (bitrange(12, 11, insn))
		{
		case 0b00:
			// MOV
			if (registers[0] == 0x4BDE398F && registers[1] == 0x9E2FA863) {
				Dummy();
			}
			registers[Rd] = imm;
			if (imm == 0xE0) {
				Dummy();
			}
			//curInsnAsm = "MOV";
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}

			break;
		case 0b01:
			// CMP
		{
			SetBit(CPSR, 29);
			if (imm > registers[Rd]) {
				ClearBit(CPSR, 29);
			}
			uint32_t result = registers[Rd] - imm;
			//curInsnAsm = "CMP";
			if (result >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (result == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (Overflow(registers[Rd], imm, registers[Rd], false)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
			break;
		}
		case 0b10:
			// ADD
		{
			uint64_t tst = uint64_t(registers[Rd]) + uint64_t(imm);
			uint32_t Op1 = registers[Rd];
			bool poss = false;
			if (registers[Rd] > 0 || imm > 0) {
				poss = true;
			}
			registers[Rd] = registers[Rd] + imm;
			//curInsnAsm = "ADD";
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)) {
				SetBit(CPSR, 29);
			}
			else
			{
				ClearBit(CPSR, 29);
			}

			if (Overflow(Op1, imm, registers[Rd], true)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
		}
		break;
		case 0b11:
			// SUB
		{
			uint32_t Op1 = registers[Rd];
			uint32_t Op2 = imm;
			SetBit(CPSR, 29);
			registers[Rd] = registers[Rd] - imm;
			//curInsnAsm = "SUB";
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (Op2 > Op1) {
				ClearBit(CPSR, 29);
			}
			if (Overflow(Op1, imm, registers[Rd], false)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
			break;
		}
		}
	}
	if (bitrange(15, 10, insn) == 0b010000) {
		//Thumb 4 - ALU Operations
		int Rd = bitrange(2, 0, insn);
		int Rs = bitrange(5, 3, insn);
		switch (bitrange(9, 6, insn))
		{
		case 0x0:
			// AND
			registers[Rd] = registers[Rd] & registers[Rs];
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			//curInsnAsm = "AND";
			break;
		case 0x1:
			// EOR
			registers[Rd] = registers[Rd] ^ registers[Rs];
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			//curInsnAsm = "EOR";
			break;
		case 0x2:
			// LSL
		{
			uint64_t test = static_cast<uint64_t>(registers[Rd]);
			uint32_t offset = registers[Rs] & 0xff;
			if (offset > 0) {
				registers[Rd] <<= offset;
				if (offset > 31) {
					registers[Rd] = 0;
				}
				if (registers[Rd] >> 31 & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				if (registers[Rd] == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}

				test = test << (registers[Rs] & 0xff);
				if ((test >> 32) & 1) {
					SetBit(CPSR, 29);
				}
				else
				{
					ClearBit(CPSR, 29);
				}
			}
			//curInsnAsm = "LSL";
			break;
		}
		case 0x3:
			// LSR
		{
			bool carry = false;
			uint32_t test = registers[Rd];
			uint32_t offset = registers[Rs] & 0xff;
			registers[Rd] = registers[Rd] >> offset;
			if (offset == 0) {
				if (C) {
					carry = true;
				}
			}
			else if (offset >= 32) {
				if ((test >> (offset - 1)) & 1) {
					carry = true;
				}
				registers[Rd] = 0;
				if (offset > 32) {
					carry = false;
				}
			}
			else {
				if ((test >> (offset - 1)) & 1) {
					carry = true;
				}
			}

			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (carry) {
				SetBit(CPSR, 29);
			}
			else
			{
				ClearBit(CPSR, 29);
			}
			//curInsnAsm = "LSR";
			break;
		}
		case 0x4:
			// ASR
		{
			bool carry = false;
			uint32_t test = registers[Rd];
			uint32_t offset = registers[Rs] & 0xff;
			if (offset == 32) {
				Dummy();
			}
			if (offset == 0) {
				if (C) {
					carry = true;
				}
			}
			else if (offset >= 32) {
				if (registers[Rd] >> 31 & 1) {
					registers[Rd] = ~0;
					carry = true;
				}
				else
				{
					registers[Rd] = 0;
					carry = false;
				}
			}
			else
			{
				if ((registers[Rd] >> (offset - 1)) & 1) {
					carry = true;
				}
				registers[Rd] = arithmeticRight(registers[Rd], offset);
			}

			// Set flags

			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (offset > 0) {
				if (carry) {
					SetBit(CPSR, 29);
				}
				else
				{
					ClearBit(CPSR, 29);
				}
			}
			//curInsnAsm = "ASR";
			break;
		}
		case 0x5:
			// ADC
		{
			int c = 0;
			if ((CPSR >> 29) & 1) {
				c = 1;
			}
			uint64_t tst = uint64_t(registers[Rd]) + uint64_t(registers[Rs]) + c;
			uint32_t Op1 = registers[Rd];
			uint32_t Op2 = registers[Rs];
			uint32_t result = Op1 + Op2 + c;
			bool poss = false;
			if (registers[Rs] > 0 || registers[Rd] > 0) {
				poss = true;
			}
			registers[Rd] = registers[Rd] + registers[Rs] + c;
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)) {
				SetBit(CPSR, 29);
			}
			else
			{
				ClearBit(CPSR, 29);
			}
			if (Overflow(Op1, Op2, result, true)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
			//curInsnAsm = "ADC";
			break;
		}
		case 0x6:
			// SBC
		{
			int c = 1;
			if ((CPSR >> 29) & 1) {
				c = 0;
			}
			uint32_t Op1 = registers[Rd];
			uint32_t Op2 = registers[Rs];
			uint32_t result = Op1 - Op2 - c;
			SetBit(CPSR, 29);
			registers[Rd] = registers[Rd] - registers[Rs] - c;
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (Op2 > Op1) {
				ClearBit(CPSR, 29);
			}
			if (Overflow(Op1, Op2, result, false)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
			//curInsnAsm = "SBC";
			break;
		}
		case 0x7:
			// ROR
		{
			bool carry = false;
			uint32_t test = registers[Rd];
			uint32_t shiftAmnt = registers[Rs] & 0xff;
			if (shiftAmnt > 32) {
				shiftAmnt %= 32;
			}
			if (registers[1] == 66) {
				Dummy();
			}
			registers[Rd] = rotateRight(registers[Rd], shiftAmnt);
			if (shiftAmnt == 32) {
				registers[Rd] = test;
				if (test >> 31 & 1) {
					carry = true;
				}
			}
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (shiftAmnt > 0) {
				if (((test >> (shiftAmnt - 1)) & 1) || carry) {
					SetBit(CPSR, 29);
				}
				else
				{
					ClearBit(CPSR, 29);
				}
			}
			//curInsnAsm = "ROR";
			break;
		}
		case 0x8:
			// TST
		{
			//curInsnAsm = "TST";
			uint32_t result = registers[Rd] & registers[Rs];
			if (result >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (result == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
		}
		break;
		case 0x9:
			// NEG
			registers[Rd] = 0 - registers[Rs];
			SetBit(CPSR, 29);
			if (registers[Rs] > 0) {
				ClearBit(CPSR, 29);
			}
			if (Overflow(0, registers[Rs], registers[Rd], false)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			//curInsnAsm = "NEG";
			break;
		case 0xA:
			// CMP
		{
			//curInsnAsm = "CMP";
			uint32_t result = registers[Rd] - registers[Rs];
			SetBit(CPSR, 29);
			if (Overflow(registers[Rd], registers[Rs], result, false)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
			if (registers[Rs] > registers[Rd]) {
				ClearBit(CPSR, 29);
			}
			if (result >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (result == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			break;
		}
		case 0xB:
			// CMN
		{
			if (registers[0] == 0x00A830C8) {
				Dummy();
			}
			uint64_t tst = static_cast<uint64_t>(registers[Rd]) + static_cast<uint64_t>(registers[Rs]);
			bool poss = false;
			if (registers[Rs] > 0 || registers[Rd] > 0) {
				poss = true;
			}
			//curInsnAsm = "CMN";
			uint32_t result = registers[Rd] + registers[Rs];
			SetBit(CPSR, 29);
			if (Overflow(registers[Rd], registers[Rs], result, true)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
			if ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)) {
				SetBit(CPSR, 29);
			}
			else
			{
				ClearBit(CPSR, 29);
			}
			if (result >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (result == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
		}
		break;
		case 0xC:
			// ORR
			registers[Rd] = registers[Rd] | registers[Rs];
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			//curInsnAsm = "ORR";
			break;
		case 0xD:
			// MUL
			registers[Rd] = registers[Rd] * registers[Rs];
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			//curInsnAsm = "MUL";
			break;
		case 0xE:
			// BIC
			registers[Rd] = registers[Rd] & ~(registers[Rs]);
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			//curInsnAsm = "BIC";
			break;
		case 0xF:
			// MVN
			registers[Rd] = ~(registers[Rs]);
			if (registers[Rd] >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (registers[Rd] == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			//curInsnAsm = "MVN";
			break;
		}
	}
	if (bitrange(15, 10, insn) == 0b010001) {
		//Thumb 5 - Hi register operations/branch exchange
		int Rd = bitrange(2, 0, insn);
		int Rs = bitrange(5, 3, insn);
		if ((insn >> 7) & 1) {
			Rd += 8;
		}
		if ((insn >> 6) & 1) {
			Rs += 8;
		}
		uint32_t Op1 = registers[WhatRegister(Rd)];
		uint32_t Op2 = registers[WhatRegister(Rs)];
		if (Rd == 15) {
			Op1 += 4;
			increment = false;
		}
		if (Rs == 15) {
			Op2 += 4;
		}
		switch (bitrange(9, 8, insn))
		{
		case 0b00:
			// ADD
		{
			registers[WhatRegister(Rd)] = Op1 + Op2;
			//curInsnAsm = "ADD";
		}
		break;
		case 0b01:
			// CMP
		{
			SetBit(CPSR, 29);
			if (Op2 > Op1) {
				ClearBit(CPSR, 29);
			}
			uint32_t result = Op1 - Op2;
			//curInsnAsm = "CMP";
			if (result >> 31 & 1) {
				SetBit(CPSR, 31);
			}
			else
			{
				ClearBit(CPSR, 31);
			}
			if (result == 0) {
				SetBit(CPSR, 30);
			}
			else
			{
				ClearBit(CPSR, 30);
			}
			if (Overflow(registers[WhatRegister(Rd)], registers[WhatRegister(Rs)], result, false)) {
				SetBit(CPSR, 28);
			}
			else
			{
				ClearBit(CPSR, 28);
			}
			break;
		}
		case 0b10:
			// MOV
			if (registers[0] == 0x4BDE398F && registers[1] == 0x9E2FA863) {
				Dummy();
			}
			registers[WhatRegister(Rd)] = Op2;
			if (Rd == 15) {
				increment = false;
			}
			//curInsnAsm = "MOV";
			break;
		case 0b11:
			// BX/BLX
			uint32_t address = registers[WhatRegister(Rs)];
			if (registers[WhatRegister(Rs)] % 2 == 0) {
				// Switch to arm mode
				ClearBit(CPSR, 5);
				increment = false;
			}
			else
			{
				// Stay in thumb mode
				address -= 1;
			}

			if ((insn >> 7) & 1) {
				// BLX
				registers[WhatRegister(14)] = registers[30];
				registers[30] = address;
				increment = false;
				//curInsnAsm = "BLX";
			}
			else
			{
				// BX
				registers[30] = address;
				increment = false;
				//curInsnAsm = "BX";
			}
			if (registers[WhatRegister(Rs)] % 2 == 0) {
				//increment = true;
				Dummy();
			}
			break;
		}
	}
	if (bitrange(15, 11, insn) == 0b01001) {
		//Thumb 6 - load PC-relative
		int Rd = bitrange(10, 8, insn);
		uint32_t offset = bitrange(7, 0, insn);
		offset *= 4;
		uint32_t address = (registers[30] & ~2) + offset + 4;
		registers[Rd] = mmu.MemoryReadWord(address);
		//curInsnAsm = "LDR PC-Relative";
	}
	if (bitrange(15, 12, insn) == 0b0101) {
		//Thumb 7 & 8 - load/store with register offset / load/store sign-extended byte/halfword
		int Rd = bitrange(2, 0, insn);
		int Rb = bitrange(5, 3, insn);
		int Ro = bitrange(8, 6, insn);

		uint32_t address = registers[Rb] + registers[Ro];

		if (registers[30] == 0xB90) {
			Dummy();
		}

		if (address == 0x6010A90) {
			Dummy();
		}

		if ((insn >> 9) & 1) {
			// load/store sign-extended byte/halfword
			switch (bitrange(11, 10, insn))
			{
			case 0b00:
				// STRH 
			{

				if (registers[30] == 2960 && address >= 0x6000000) {
					Dummy();
				}

				uint16_t val = bitrange(15, 0, registers[Rd]);
				mmu.MemoryWrite(address, val);
				//curInsnAsm = "STRH";
				break;
			}
			case 0b01:
				// LDSB 
				//curInsnAsm = "LDSB";
				registers[Rd] = mmu.MemoryRead(address);
				if (registers[Rd] >> 7 & 1) {
					registers[Rd] |= 0xFFFFFF00;
				}
				Dummy();
				break;
			case 0b10:
				// LDRH 
			{
				//curInsnAsm = "LDRH";
				int rotAmnt = 0;
				if (address % 2 != 0) {
					address -= 1;
					rotAmnt = 8;
				}
				registers[Rd] = mmu.MemoryRead(address + 1) << 8 | mmu.MemoryRead(address);
				registers[Rd] = rotateRight(registers[Rd], rotAmnt);
				Dummy();
			}
				break;
			case 0b11:
				// LDSH 
			{
				//curInsnAsm = "LDSH";
				int rotAmnt = 0;
				if (address % 2 != 0) {
					address -= 1;
					rotAmnt = 8;
				}
				bool sign = false;
				registers[Rd] = mmu.MemoryRead(address + 1) << 8 | mmu.MemoryRead(address);
				if (registers[Rd] >> 15 & 1) {
					sign = true;
				}
				registers[Rd] = rotateRight(registers[Rd], rotAmnt);
				if (sign) {
					if (rotAmnt == 0) {
						registers[Rd] |= 0xFFFF0000;
					}
					else
					{
						registers[Rd] |= 0xFFFFFF00;
					}
				}
				
			}
				break;
			}
		}
		else
		{
			// load/store with register offset
			switch (bitrange(11, 10, insn))
			{
			case 0b00:
				// STR  
				mmu.MemoryWrite(address, registers[Rd]);
				//curInsnAsm = "STR";
				break;
			case 0b01:
				// STRB
			{
				uint8_t val = static_cast<uint8_t>(bitrange(7, 0, registers[Rd]));
				mmu.MemoryWrite(address, val);
				//curInsnAsm = "STRB";
				break;
			}
			case 0b10:
				// LDR 
			{
				int rotAmnt = bitrange(1, 0, address) * 8;
				AlignAddr(address);
				registers[Rd] = mmu.MemoryReadWord(address);
				registers[Rd] = rotateRight(registers[Rd], rotAmnt);
				//curInsnAsm = "LDR";
			}
			break;
			case 0b11:
				// LDRB 
				registers[Rd] = mmu.MemoryRead(address);
				//curInsnAsm = "LDRB";
				break;
			}
		}

	}
	if (bitrange(15, 13, insn) == 0b011) {
		//Thumb 9 - load/store with immediate offset
		int Rd = bitrange(2, 0, insn);
		int Rb = bitrange(5, 3, insn);
		uint8_t offset = bitrange(10, 6, insn);
		uint32_t address = registers[Rb];

		switch (bitrange(12, 11, insn))
		{
		case 0b00:
			// STR
			address += (offset << 2);
			mmu.MemoryWrite(address, registers[Rd]);
			//curInsnAsm = "STR";
			break;
		case 0b01:
			// LDR
		{
			address += (offset << 2);
			int rotAmnt = bitrange(1, 0, address) * 8;
			AlignAddr(address);
			if (rotAmnt > 0) {
				Dummy();
			}
			registers[Rd] = mmu.MemoryReadWord(address);
			registers[Rd] = rotateRight(registers[Rd], rotAmnt);
			//curInsnAsm = "LDR";
		}
		break;
		case 0b10:
		{
			// STRB
			address += offset;
			uint8_t val = static_cast<uint8_t>(bitrange(7, 0, registers[Rd]));
			mmu.MemoryWrite(address, val);
			//curInsnAsm = "STRB";
		}
		break;
		case 0b11:
			// LDRB
			address += offset;
			registers[Rd] = mmu.MemoryRead(address);
			//curInsnAsm = "LDRB";
			break;
		}

	}
	if (bitrange(15, 12, insn) == 0b1000) {
		//Thumb 10 - load/store halfword
		int Rd = bitrange(2, 0, insn);
		int Rb = bitrange(5, 3, insn);
		uint32_t offset = bitrange(10, 6, insn) * 2;
		uint32_t address = registers[Rb] + offset;

		if (address >= 0x0D000000) {
			Dummy();
		}
		

		if (insn >> 11 & 1) {
			// LDR
			if (registers[30] == 0x8000686) {
				Dummy();
			}
			if (address % 2 != 0) {
				Dummy();
			}
			if (registers[1] == 0xFF) {
				Dummy();
			}
			//curInsnAsm = "LDRH";
			int rotAmnt = 0;
			if (address % 2 != 0) {
				address -= 1;
				rotAmnt = 8;
			}
			registers[Rd] = mmu.MemoryRead(address + 1) << 8 | mmu.MemoryRead(address);
			registers[Rd] = rotateRight(registers[Rd], rotAmnt);
			//if (address >= 0x0D000000) {
			//	registers[Rd] = 1;
			//}
		}
		else
		{
			// STR
			//curInsnAsm = "STRH";
			mmu.MemoryWrite(address, static_cast<uint16_t>(bitrange(15, 0, registers[Rd])));
		}
		Dummy();
	}
	if (bitrange(15, 12, insn) == 0b1001) {
		//Thumb 11 - load/store SP-relative
		int Rd = bitrange(10, 8, insn);
		uint32_t offset = bitrange(7, 0, insn) * 4;
		uint32_t address = registers[WhatRegister(13)] + offset;
		if (insn >> 11 & 1) {
			// LDR
			//curInsnAsm = "LDR SP Relative";
			int rotAmnt = bitrange(1, 0, address) * 8;
			AlignAddr(address);
			registers[Rd] = mmu.MemoryReadWord(address);
			registers[Rd] = rotateRight(registers[Rd], rotAmnt);
		}
		else
		{
			// STR
			//curInsnAsm = "STR SP Relative";
			mmu.MemoryWrite(address, registers[Rd]);
		}
		Dummy();
	}
	if (bitrange(15, 12, insn) == 0b1010) {
		//Thumb 12 - get relative address
		int Rd = bitrange(10, 8, insn);
		uint16_t offset = bitrange(7, 0, insn);
		offset *= 4;
		if ((insn >> 11) & 1) {
			// SP
			registers[Rd] = registers[WhatRegister(13)] + offset;
		}
		else
		{
			// PC
			registers[Rd] = ((registers[30] + 4) & ~(2)) + offset;
		}
	}
	if (bitrange(15,  8, insn) == 0b10110000) {
		//Thumb 13 - add offset to stack pointer
		uint32_t offset = bitrange(6, 0, insn) * 4;
		if (insn >> 7 & 1) {
			// ADD
			registers[WhatRegister(13)] -= offset;
			//curInsnAsm = "SUB SP";
		}
		else
		{
			// SUB
			registers[WhatRegister(13)] += offset;
			//curInsnAsm = "ADD SP";
		}
	}
	if (bitrange(15, 12, insn) == 0b1011 && bitrange(10, 9, insn) == 0b10) {
		//Thumb 14 - push/pop registers
		uint32_t address = registers[WhatRegister(13)];
		uint32_t finishedAddr = 0;
		int Rlist = 0;
		bool ran = false;

		for (int i = 0; i < 9; i++) {
			if (insn >> i & 1) {
				Rlist++;
			}
		}

		for (int i = 0; i < 9; i++) {
			if (insn >> i & 1) {
				if (insn >> 11 & 1) {
					//POP(LDMIA)
					if (i == 8) {
						i = 15;
						increment = false;
					}
					registers[WhatRegister(i)] = mmu.MemoryReadWord(address);
					address += 4;
					finishedAddr = address;
					//curInsnAsm = "POP";
				}
				else
				{
					if (!ran) {
						finishedAddr = address - ((Rlist) * 4);
						address = finishedAddr;
						ran = true;
					}
					else
					{
						address += 4;
					}
					//PUSH(STMDB)
					if (i == 8) {
						i = 14;
					}

					mmu.MemoryWrite(address, registers[WhatRegister(i)]);
					//curInsnAsm = "PUSH";
				}
			}
		}
		registers[WhatRegister(13)] = finishedAddr;
	}
	if (bitrange(15, 12, insn) == 0b1100) {
		//Thumb 15
		if (registers[30] == 0x800074C && registers[2] == 0x6014280) {
			Dummy();
		}
		if (registers[30] == 0x800074C) {
			Dummy();
		}
		int Rb = bitrange(10, 8, insn);
		uint32_t address = registers[Rb];
		bool empty = true;
		bool write = true;
		uint32_t ogAddress = address;
		bool first = false;
		bool special = false;
		for (int i = 0; i < 8; i++) {
			if (insn >> i & 1) {
				if (insn >> 11 & 1) {
					//LDMIA
					registers[i] = mmu.MemoryReadWord(address);
					address += 4;
					//curInsnAsm = "LDMIA";
					if (Rb == i) {
						write = false;
					}
				}
				else
				{
					//STMIA
					if (address == 0x7000000) {
						Dummy();
					}
					mmu.MemoryWrite(address, registers[i]);
					address += 4;
					//curInsnAsm = "STMIA";
					if (Rb == i) {
						if (empty) {
							first = true;
							mmu.MemoryWrite(address, ogAddress);
						}
						else
						{
							write = true;
							ogAddress = address - 4;
							special = true;
						}
					}
				}
				empty = false;
			}
		}
		if (special) {
			mmu.MemoryWrite(ogAddress, address);
		}
		if (empty) {
			if (insn >> 11 & 1) {
				//LDMIA
				registers[30] = mmu.MemoryReadWord(address);
				address += 4;
				//curInsnAsm = "LDMIA";
				increment = false;
			}
			else
			{
				//STMIA
				mmu.MemoryWrite(address, registers[30] + 6);
				address += 4;
				//curInsnAsm = "STMIA";
			}
			registers[Rb] += 0x40;
		}
		else if(write)
		{
			registers[Rb] = address;
		}
	}
	if (bitrange(15, 12, insn) == 0b1101) {
		//Thumb 16 - Conditional Branch
		int16_t offset = bitrange(7, 0, insn) * 2;
		if (offset & 0x100) {
			offset |= 0xFE00;
		}
		int8_t tst = bitrange(7, 0, insn);
		switch (bitrange(11, 8, insn))
		{
		case 0x0:
			// BEQ
			if (Z) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BEQ";
			}
			break;
		case 0x1:
			// BNE
			if (!Z) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BNE";
			}
			break;
		case 0x2:
			// BCS
			if (C) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BCS";
			}
			break;
		case 0x3:
			// BCC
			if (!C) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BCC";
			}
			break;
		case 0x4:
			// BMI
			if (N) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BMI";
			}
			break;
		case 0x5:
			// BPL
			if (!N) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BPL";
			}
			break;
		case 0x6:
			// BVS
			if (V) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BVS";
			}
			break;
		case 0x7:
			// BVC
			if (!V) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BVC";
			}
			break;
		case 0x8:
			// BHI
			if (C && !Z) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BHI";
			}
			break;
		case 0x9:
			// BLS
			if (!C || Z) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BLS";
			}
			break;
		case 0xA:
			// BGE
			if (N == V) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BGE";
			}
			break;
		case 0xB:
			// BLT
			if (N != V) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BLT";
			}
			break;
		case 0xC:
			// BGT
			if (!Z && N == V) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BGT";
			}
			break;
		case 0xD:
			// BLE
			if (Z || N != V) {
				registers[30] += offset + 4;
				increment = false;
				//curInsnAsm = "BLE";
			}
			break;
		case 0xE:
			// UNDEFINED
			break;
		case 0xF:
			// SWI
			q = true;
			Dummy();
			registers[25] = registers[30] + 2;
			SPSR_svc = CPSR;
			uint32_t mask = 0x000000FF;
			CPSR &= ~mask;
			CPSR |= 0b10011;
			registers[30] = 0x8;
			increment = false;
			//curInsnAsm = "SWI";
			break;

		}
	}
	if (bitrange(15, 11, insn) == 0b11100) {
		//Thumb 18 - Unconditional Branch
		int16_t offset = bitrange(10, 0, insn) * 2;
		if (offset & 0x800) {
			offset |= 0xF800;
		}
		registers[30] += offset + 4;
		increment = false;
		//curInsnAsm = "B";
	}
	if (bitrange(15, 11, insn) == 0b11110) {
		//Thumb 19 - Long Branch and Link

		uint32_t retAddr = registers[30] + 2;
		uint32_t upper = bitrange(10, 0, insn);
		registers[WhatRegister(14)] = registers[30] + 4;

		registers[30] += 2;
		uint32_t insn2 = FetchThumb(mmu);
		registers[30] -= 2;

		uint32_t lower = bitrange(10, 0, insn2);
		int32_t target = (upper << 12) | (lower << 1);
		if (target & 0x400000) {
			target |= 0xFF800000;
		}

		if ((insn2 >> 12) & 1) {

			registers[30] = registers[WhatRegister(14)] + target;
			registers[WhatRegister(14)] = retAddr + 3;
			//curInsnAsm = "BL";
		}
		else
		{
			//curInsnAsm = "BLX";
		}




		//registers[30] += 2;
		//uint32_t insn2 = FetchThumb(mmu);
		//registers[30] -= 2;
		//uint32_t upper = bitrange(10, 0, insn);
		//uint32_t lower = bitrange(10, 0, insn2);
		//uint32_t target = (upper << 12) | (lower << 1);
		//
		//
		//if ((insn2 >> 12) & 1) {
		//	registers[WhatRegister(14)] = registers[30];
		//	registers[30] += target + 4;
		//	//curInsnAsm = "BL";
		//}
		//else
		//{
		//	//curInsnAsm = "BLX";
		//}
		increment = false;

	}
	if (registers[30] % 2 != 0) {
		AlignAddr(registers[30]);
		Dummy();
	}
}

void Arm::Dummy()
{
}

int Arm::WhatRegister(uint32_t input)
{
	switch (input)
	{
	case 0:
		return 0;
		break;
	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	case 3:
		return 3;
		break;
	case 4:
		return 4;
		break;
	case 5:
		return 5;
		break;
	case 6:
		return 6;
		break;
	case 7:
		return 7;
		break;
	case 8:
		if (mode == Mode::Fiq) {
			return 9;
		}
		else
		{
			return 8;
		}
		break;
	case 9:
		if (mode == Mode::Fiq) {
			return 11;
		}
		else
		{
			return 10;
		}
		break;
	case 10:
		if (mode == Mode::Fiq) {
			return 13;
		}
		else
		{
			return 12;
		}
		break;
	case 11:
		if (mode == Mode::Fiq) {
			return 15;
		}
		else
		{
			return 14;
		}
		break;
	case 12:
		if (mode == Mode::Fiq) {
			return 17;
		}
		else
		{
			return 16;
		}
		break;
	case 13:
		switch (mode)
		{
		case Arm::Mode::User:
			return 18;
			break;
		case Arm::Mode::Fiq:
			return 23;
			break;
		case Arm::Mode::Irq:
			return 22;
			break;
		case Arm::Mode::Supervisor:
			return 19;
			break;
		case Arm::Mode::Abort:
			return 20;
			break;
		case Arm::Mode::Undefined:
			return 21;
			break;
		case Arm::Mode::System:
			return 18;
			break;
		}
		break;
	case 14:
		switch (mode)
		{
		case Arm::Mode::User:
			return 24;
			break;
		case Arm::Mode::Fiq:
			return 29;
			break;
		case Arm::Mode::Irq:
			return 28;
			break;
		case Arm::Mode::Supervisor:
			return 25;
			break;
		case Arm::Mode::Abort:
			return 26;
			break;
		case Arm::Mode::Undefined:
			return 27;
			break;
		case Arm::Mode::System:
			return 24;
			break;
		}
		break;
	case 15:
		return 30;
		break;
	}
}

void Arm::SetFlags(uint32_t Rd, bool carry, bool overflow, bool setV)
{
	//N - Sign flag
	if ((Rd >> 31) & 1) {
		SetBit(CPSR, 31);
	}
	else
	{
		ClearBit(CPSR, 31);
	}
	//Z - Zero flag
	if (Rd == 0) {
		SetBit(CPSR, 30);
	}
	else
	{
		ClearBit(CPSR, 30);
	}
	//C - Carry flag
	if (carry) {
		SetBit(CPSR, 29);
	}
	else
	{
		ClearBit(CPSR, 29);
	}
	//V - Overflow flag
	if (setV) {
		if (overflow) {
			SetBit(CPSR, 28);
		}
		else
		{
			ClearBit(CPSR, 28);
		}
	}
}

void Arm::SetFlagsSub(uint32_t Rd, bool carry, bool overflow, bool setV)
{
	if ((Rd >> 31) & 1) {
		SetBit(CPSR, 31);
	}
	else
	{
		ClearBit(CPSR, 31);
	}
	if (Rd == 0) {
		SetBit(CPSR, 30);

	}
	else
	{
		ClearBit(CPSR, 30);
	}
	if (carry) {
		ClearBit(CPSR, 29);
	}
	if (setV) {
		if (overflow) {
			SetBit(CPSR, 28);
		}
		else
		{
			ClearBit(CPSR, 28);
		}
	}
}

uint32_t Arm::bitrange(int msb, int lsb, uint16_t insn)
{

	return (insn >> lsb) & ~(~0 << (msb - lsb + 1));
}

uint32_t Arm::bitrange(int msb, int lsb, uint32_t insn)
{

	return (insn >> lsb) & ~(~0 << (msb - lsb + 1));
}

uint32_t Arm::bitrange(int msb, int lsb, uint64_t insn)
{
	return (insn >> lsb) & ~(~0 << (msb - lsb + 1));
}

void Arm::DecodeArm(MMU & mmu)
{
	if (Condition(Ainsn)) {
		
		if (registers[8] == 32 && mode == Mode::Fiq) {
			Dummy();
		}
		
		uint32_t upper = ((Ainsn >> 20) & 0xFF) << 4;
		uint32_t lower = (Ainsn & 0xF0) >> 4;
		uint32_t nInsn = upper | lower;
		if (ArmFunc[nInsn] != NULL) {
			(this->*ArmFunc[nInsn])(mmu);
		}
		else {
			//std::cerr << "Error: Instruction " << Ainsn << std::endl;
			//std::cerr << "Register 30: " << registers[30] << std::endl << std::endl;
		}
	}
}

void Arm::Decode(uint32_t insn, PPU& ppu, MMU& mmu)
{
	if (Condition(insn)) {

		//uint16_t nInsn = (((insn >> 20) & 0xFF) << 4) | (insn & 0xF);
		//(this->*ArmFunc[nInsn])(mmu);
		numm++;
		//if (numm >= 278500) {
		//	WriteToLogA(numm);
		//}
		//if (numm == 285000) {
		//	Dummy();
		//}
		//if (numm == 278806) {
		//	Dummy();
		//}
		//if (numm == 278798) {
		//	Dummy();
		//}
		//if (numm >= 275000 && numm <= 280000) {
		//	WriteToLogA(numm);
		//}
		//if (numm == 280000) {
		//	Dummy();
		//}
		if (numm == 278817) {
			Dummy();
		}
		bool S = false;
		if ((insn >> 20) & 1) {
			S = true;
		}
		else
		{
			S = false;
		}
		bool ok = true;
		// Branch instructions

		// Branch & Branch w/ link
		if (bitrange(27, 25, insn) == 0b101) {
			if ((insn >> 24) & 1) {
				BL(insn);
				//curInsnAsm = "BL";
			}
			else
			{
				B(insn);
				//curInsnAsm = "B";
			}
		}
		// Branch & Exchange
		if (bitrange(27, 8, insn) == 0b00010010111111111111) {
			int x = bitrange(3, 0, insn);
			uint32_t Rn = registers[WhatRegister(x)];
			if (x == 15) {
				Rn += 4;
			}
			if (Rn % 2 != 0) {
				Rn -= 1;
				SetBit(CPSR, 5);
			}
			else
			{
				ClearBit(CPSR, 5);
			}
			// BX
			if (bitrange(7, 4, insn) == 0b0001) {
				registers[30] = Rn;
				increment = false;
				ok = false;
				//curInsnAsm = "BX";
			}
			// BLX
			if (bitrange(7, 4, insn) == 0b0011) {
				registers[WhatRegister(14)] = registers[30] + 4;
				registers[30] = Rn;
				increment = false;
				ok = false;
				//curInsnAsm = "BXL";
			}
		}

		// Software interrupt
		if (bitrange(27, 24, insn) == 0b1111) {
			//SWI
			registers[25] = registers[30] + 4;
			SPSR_svc = CPSR;
			uint32_t mask = 0x000000FF;
			CPSR &= ~mask;
			CPSR |= 0b10011;
			registers[30] = 0x8;
			increment = false;
			//curInsnAsm = "SWI";
		}


		// Data processing ALU
		if (bitrange(27, 26, insn) == 0b00 && ok) {
			if ((bitrange(25, 23, insn) == 0b010) && (bitrange(21, 20, insn) == 0b00) && bitrange(19, 16, insn) == 0b1111) {

				int Rd = WhatRegister(bitrange(15, 12, insn));
				// MRS
				if ((insn >> 22) & 1) {
					// SPSR
					switch (mode)
					{
					case Arm::Mode::User:
						break;
					case Arm::Mode::Fiq:
						registers[Rd] = SPSR_fiq;
						break;
					case Arm::Mode::Irq:
						registers[Rd] = SPSR_irq;
						break;
					case Arm::Mode::Supervisor:
						registers[Rd] = SPSR_svc;
						break;
					case Arm::Mode::Abort:
						registers[Rd] = SPSR_abt;
						break;
					case Arm::Mode::Undefined:
						registers[Rd] = SPSR_und;
						break;
					case Arm::Mode::System:
						registers[Rd] = CPSR;
						break;
					}
				}
				else
				{
					// CPSR
					registers[Rd] = CPSR;
				}

				//curInsnAsm = "MRS";
			}
			else if ((bitrange(25, 23, insn) == 0b110) && (bitrange(21, 20, insn) == 0b10)) {
				// MSR Immediate
				uint32_t mask = 0;
				if ((insn >> 19) & 1) {
					mask |= 0xFF000000;
				}
				if ((insn >> 18) & 1) {
					mask |= 0x00FF0000;
				}
				if ((insn >> 17) & 1) {
					mask |= 0x0000FF00;
				}
				if ((insn >> 16) & 1) {
					mask |= 0x000000FF;
				}
				uint32_t imm = bitrange(7, 0, insn);
				uint16_t shiftAmnt = (bitrange(11, 8, insn)) * 2;
				imm = rotateRight(imm, shiftAmnt);
				if ((insn >> 22) & 1) {
					// SPSR_MODE
					switch (mode)
					{
					case Arm::Mode::User:
						break;
					case Arm::Mode::Fiq:
						SPSR_fiq = imm;
						break;
					case Arm::Mode::Irq:
						SPSR_irq = imm;
						break;
					case Arm::Mode::Supervisor:
						SPSR_svc = imm;
						break;
					case Arm::Mode::Abort:
						SPSR_abt = imm;
						break;
					case Arm::Mode::Undefined:
						SPSR_und = imm;
						break;
					case Arm::Mode::System:
						break;
					}
				}
				else
				{
					//CPSR
					uint32_t old_CPSR = CPSR;
					CPSR &= ~mask;
					CPSR |= imm;
					switch (bitrange(4, 0, CPSR))
					{
					case 0b10001:
						//FIQ
						SPSR_fiq = old_CPSR;
						break;
					case 0b10010:
						//IRQ
						SPSR_irq = old_CPSR;
						break;
					case 0b10011:
						//SWI
						SPSR_svc = old_CPSR;
						break;
					case 0b10111:
						//ABT
						SPSR_abt = old_CPSR;
						break;
					case 0b11011:
						//UND
						SPSR_und = old_CPSR;
						break;
					default:
						break;
					}
				}
				//curInsnAsm = "MSR";
			}
			else if ((bitrange(25, 23, insn) == 0b010) && (bitrange(21, 20, insn) == 0b10)) {
				// MSR Register
				int Rm = WhatRegister(bitrange(3, 0, insn));
				uint32_t val = registers[Rm];
				uint32_t mask = 0;
				if ((insn >> 19) & 1) {
					mask |= 0xFF000000;
				}
				if ((insn >> 18) & 1) {
					mask |= 0x00FF0000;
				}
				if ((insn >> 17) & 1) {
					mask |= 0x0000FF00;
				}
				if ((insn >> 16) & 1) {
					mask |= 0x000000FF;
				}
				if ((insn >> 22) & 1) {
					// SPSR_MODE
					Dummy();
					switch (mode)
					{
					case Arm::Mode::User:
						break;
					case Arm::Mode::Fiq:
						SPSR_fiq = val;
						break;
					case Arm::Mode::Irq:
						SPSR_irq = val;
						break;
					case Arm::Mode::Supervisor:
						SPSR_svc = val;
						break;
					case Arm::Mode::Abort:
						SPSR_abt = val;
						break;
					case Arm::Mode::Undefined:
						SPSR_und = val;
						break;
					case Arm::Mode::System:
						break;
					}
				}
				else
				{
					//CPSR
					uint32_t old_CPSR = CPSR;
					CPSR &= ~mask;
					CPSR |= val;
					switch (bitrange(4, 0, CPSR))
					{
					case 0b10001:
						//FIQ
						SPSR_fiq = old_CPSR;
						break;
					case 0b10010:
						//IRQ
						SPSR_irq = old_CPSR;
						//registers[WhatRegister(14)] = registers[30] + 4;
						//registers[30] = 0x18;
						//increment = false;
						break;
					case 0b10011:
						//SWI
						SPSR_svc = old_CPSR;
						break;
					case 0b10111:
						//ABT
						SPSR_abt = old_CPSR;
						break;
					case 0b11011:
						//UND
						SPSR_und = old_CPSR;
						break;
					}
				}
				//curInsnAsm = "MSR";
			}
			else

			{
				bool updateC = true;
				bool valid = true;
				uint32_t Op2;
				bool carry = false;
				int Rn = WhatRegister(bitrange(19, 16, insn));
				int Rd = WhatRegister(bitrange(15, 12, insn));
				uint32_t Op1 = registers[Rn];

				if ((insn >> 25) & 1) {
					// Op2 Immediate
					Op2 = bitrange(7, 0, insn);
					uint16_t shiftAmnt = bitrange(11, 8, insn) * 2;
					if (Op2 > 0 && (Op2 >> (shiftAmnt - 1) & 1)) {
						carry = true;
					}
					Op2 = rotateRight(Op2, shiftAmnt);
					if (Rn == 30) {
						Op1 += 8;
					}
				}
				else {
					// Op2 Register
					int Rm = WhatRegister(bitrange(3, 0, insn));
					Op2 = registers[Rm];
					bool inc = false;

					if ((insn >> 4) & 1) {
						if ((insn >> 7) & 1) {
							valid = false;
						}
						// Shift by register
						int Rs = WhatRegister(bitrange(11, 8, insn));
						uint8_t shiftAmnt = bitrange(7, 0, registers[Rs]);
						if (Rm == 30) {
							Op2 += 12;
						}
						if (Rn == 30) {
							Op1 += 12;
						}
						if (Rm == 30 && Rd == 3) {
							Dummy();
						}
						if (registers[0] == 0x10679948 && registers[1] == 0xACD0ABD1) {
							Dummy();
						}
						if (registers[1] == 0xA5Fb3DAF && registers[2] == 6) {
							Dummy();
						}

						switch (bitrange(6, 5, insn))
						{
						case 0:
						{
							// LSL
							if (shiftAmnt == 0) {
								if (C) {
									carry = true;
								}
								else
								{
									carry = false;
								}
							}
							else if (shiftAmnt > 0) {
								uint64_t test = static_cast<uint64_t>(Op2);
								test = test << shiftAmnt;
								if ((test >> 32) & 1) {
									carry = true;
								}
								if (shiftAmnt < 32) {
									Op2 = Op2 << shiftAmnt;
								}
								else
								{
									Op2 = bitrange(31, 0, test);
								}
							}

							break;
						}
						case 1:
						{
							// LSR
							if (shiftAmnt == 0) {
								if (C) {
									carry = true;
								}
								else
								{
									carry = false;
								}
							}
							else if (shiftAmnt == 32) {
								if ((Op2 >> 31) & 1) {
									carry = true;
								}
								Op2 = 0;
							}
							else if (shiftAmnt > 32) {
								if ((Op2 >> (shiftAmnt - 1)) & 1) {
									carry = true;
								}
								carry = false;
								Op2 = 0;
							}
							else
							{
								uint32_t test = Op2;
								if ((Op2 >> (shiftAmnt - 1)) & 1) {
									carry = true;
								}
								Op2 = Op2 >> shiftAmnt;
							}
							break;
						}
						case 2:
						{
							Dummy();
							// ASR

							//if (shiftAmnt == 0 || shiftAmnt == 32) {
							//	if ((Op2 >> 31) & 1)
							//	{
							//		Op2 = 0;
							//		carry = true;
							//		Op2 = ~Op2;
							//	}
							//	if (C) {
							//		carry = true;
							//	}
							//}
							if (shiftAmnt == 0) {
								if (C) {
									carry = true;
								}
								else
								{
									carry = false;
								}
							}
							else if (shiftAmnt >= 32) {
								if (Op2 >> 31 & 1) {
									Op2 = 0;
									Op2 = ~Op2;
									carry = true;
								}
								else
								{
									Op2 = 0;
								}
							}
							else
							{
								Dummy();
								uint64_t test = static_cast<uint64_t>(Op2);
								uint32_t ya = Op2 >> (shiftAmnt - 1);
								if ((Op2 >> (shiftAmnt - 1)) & 1) {
									carry = true;
								}
								Op2 = arithmeticRight(Op2, shiftAmnt);
							}
							break;
						}
						case 3:
							// ROR
							if (shiftAmnt == 0 && Op2 == 1) {
								Dummy();
							}
							if (shiftAmnt >= 32) {
								uint32_t ye = shiftAmnt - 32;
								uint32_t tst = Op2;
								if ((tst >> (shiftAmnt - 1) & 1)) {
									carry = true;
								}
								if (ye > 0) {
									Op2 = rotateRight(Op2, ye);
								}
							}
							else if (shiftAmnt == 0) {

								//uint32_t tst = Op2;
								//if ((CPSR >> 29) & 1) {
								//	shiftAmnt = 1;
								//	if ((tst >> 0) & 1) {
								//		carry = true;
								//	}
								//	Op2 = rotateRight(Op2, shiftAmnt);
								//	SetBit(Op2, 31);
								//}
								//else
								//{
								//	carry = true;
								//	//shiftAmnt = 1;
								//	//Op2 = rotateRight(Op2, shiftAmnt);
								//	//ClearBit(Op2, 31);
								//}
								if (C) {
									carry = true;
								}
								else
								{
									carry = false;
								}

							}
							else
							{
								uint32_t tst = Op2;
								if ((tst >> (shiftAmnt - 1) & 1)) {
									carry = true;
								}
								Op2 = rotateRight(Op2, shiftAmnt);
							}
							break;
						}
					}
					else
					{
						// Shift by immediate
						uint8_t shiftAmnt = bitrange(11, 7, insn);
						if (Rm == 30) {
							Op2 += 8;
						}
						if (Rn == 30) {
							Op1 += 8;
						}
						switch (bitrange(6, 5, insn))
						{
						case 0:
						{
							// LSL
							if (shiftAmnt == 0) {
								carry = C;
							}


							uint64_t test = static_cast<uint64_t>(Op2);
							test = test << shiftAmnt;
							if ((test >> 32) & 1) {
								carry = true;
							}
							Op2 = Op2 << shiftAmnt;
							break;
						}
						case 1:
						{
							// LSR
							if (shiftAmnt == 0 || shiftAmnt == 32) {
								if ((Op2 >> 31) & 1)
								{
									carry = true;
								}
								Op2 = 0;
							}
							else if (shiftAmnt > 32) {
								if ((Op2 >> (shiftAmnt - 1)) & 1) {
									carry = true;
								}
								Op2 = 0;
							}
							else
							{
								uint32_t test = Op2;
								if ((Op2 >> (shiftAmnt - 1)) & 1) {
									carry = true;
								}
								Op2 = Op2 >> shiftAmnt;
							}

							break;
						}
						case 2:
						{
							// ASR
							if (shiftAmnt == 0 || shiftAmnt == 32) {
								Dummy();
								if ((Op2 >> 31) & 1)
								{
									Op2 = 0;
									carry = true;
									Op2 = ~Op2;
								}
								else
								{
									Op2 = 0;
								}
							}
							else if (shiftAmnt > 32) {
								if ((Op2 >> (shiftAmnt - 1)) & 1) {
									carry = true;
								}
								Op2 = 0;
							}
							else {
								uint32_t test = Op2;
								if ((Op2 >> (shiftAmnt - 1)) & 1) {
									carry = true;
								}
								Op2 = arithmeticRight(Op2, shiftAmnt);
							}
							break;
						}
						case 3:
							// ROR
							if (shiftAmnt == 0) {
								uint32_t tst = Op2;
								if ((CPSR >> 29) & 1) {
									shiftAmnt = 1;
									if ((tst >> 0) & 1) {
										carry = true;
									}
									Op2 = rotateRight(Op2, shiftAmnt);
									SetBit(Op2, 31);
								}
								else
								{
									shiftAmnt = 1;
									if ((tst >> 0) & 1) {
										carry = true;
									}
									Op2 = rotateRight(Op2, shiftAmnt);
									ClearBit(Op2, 31);
								}


							}
							else
							{
								uint32_t tst = Op2;
								if ((tst >> (shiftAmnt - 1) & 1)) {
									carry = true;
								}
								Op2 = rotateRight(Op2, shiftAmnt);
							}
							if (shiftAmnt == 32) {
								Dummy();
							}

							break;
						}
					}



				}
				if (S && Rd == 30) {
					//increment = false;
					updateC = false;
					S = false;
					switch (mode)
					{
					case Arm::Mode::User:
						//
						break;
					case Arm::Mode::Fiq:
						CPSR = SPSR_fiq;
						break;
					case Arm::Mode::Irq:
						CPSR = SPSR_irq;
						break;
					case Arm::Mode::Supervisor:
						CPSR = SPSR_svc;
						break;
					case Arm::Mode::Abort:
						CPSR = SPSR_abt;
						break;
					case Arm::Mode::Undefined:
						CPSR = SPSR_und;
						break;
					case Arm::Mode::System:
						break;
					}
				}

				if (valid) {
					if (Rd == 7 && Op2 > 0) {
						Dummy();
					}
					switch (bitrange(24, 21, insn))
					{
					case 0x0:
						// AND
						registers[Rd] = Op1 & Op2;
						if (S) {
							SetFlags(registers[Rd], carry, false, false);
						}
						//curInsnAsm = "AND";
						break;
					case 0x1:
						// EOR
						registers[Rd] = Op1 ^ Op2;
						if (S) {
							SetFlags(registers[Rd], carry, false, false);
						}
						//curInsnAsm = "EOR";
						break;
					case 0x2:
					{
						// SUB
						uint32_t og = registers[Rn];
						if (updateC) {
							SetBit(CPSR, 29);
						}
						registers[Rd] = Op1 - Op2;
						if (S) {
							SetFlagsSub(registers[Rd], (Op2 > Op1), (Overflow(Op1, Op2, registers[Rd], false)), true);
						}
						//curInsnAsm = "SUB";
						break;
					}
					case 0x3:
					{
						// RSB
						uint32_t og = registers[Rn];
						SetBit(CPSR, 29);
						registers[Rd] = Op2 - Op1;
						if (S) {
							SetFlagsSub(registers[Rd], (Op1 > Op2), (Overflow(Op2, Op1, registers[Rd], false)), true);
						}
						//curInsnAsm = "RSB";
						break;
					}
					case 0x4:
					{
						// ADD
						registers[Rd] = Op1 + Op2;
						if (S) {
							uint64_t tst = uint64_t(Op1) + uint64_t(Op2);
							bool poss = false;
							if (Op1 > 0 || Op2 > 0) {
								poss = true;
							}
							SetFlags(registers[Rd], (((Op1 + Op2) > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)), (Overflow(Op1, Op2, registers[Rd], true)), true);
						}

						//curInsnAsm = "ADD";
						break;
					}
					case 0x5:
					{
						// ADC
						uint64_t tst = uint64_t(Op1) + uint64_t(Op2);
						uint32_t og = registers[Rn];
						registers[Rd] = Op1 + Op2;
						if (C) {
							registers[Rd] += 1;
						}

						if (S) {
							bool poss = false;
							if (Op1 > 0 || Op2 > 0) {
								poss = true;
							}
							SetFlags(registers[Rd], ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0)), (Overflow(Op1, Op2, registers[Rd], true)), true);
						}

						//curInsnAsm = "ADC";
						break;
					}
					case 0x6:
					{
						// SBC
						uint32_t cv = 0;
						if (!C) {
							cv = 1;
						}
						SetBit(CPSR, 29);
						uint32_t og = Op2 - cv;
						registers[Rd] = Op1 - Op2 - cv;


						if (S) {
							SetFlagsSub(registers[Rd], ((Op2 > Op1) || ((Op2 == Op1) && cv)), (Overflow(Op1, Op2, registers[Rd], false)), true);
						}

						//curInsnAsm = "SBC";
						break;
					}
					case 0x7:
					{
						// RSC
						uint32_t cv = 0;
						if (!C) {
							cv = 1;
						}
						SetBit(CPSR, 29);
						uint32_t og = Op1 - cv;
						registers[Rd] = Op2 - Op1 - cv;

						if (S) {
							SetFlagsSub(registers[Rd], (og > Op2), (Overflow(Op2, og, registers[Rd], false)), true);
						}

						//curInsnAsm = "RSC";
						break;
					}
					case 0x8:
						// TST
						if (S) {
							SetFlags((Op1 & Op2), carry, false, false);
							//curInsnAsm = "TST";
						}
						break;
					case 0x9:
						// TEQ
						if (S) {
							SetFlags((Op1 ^ Op2), carry, false, false);
							//curInsnAsm = "TEQ";
						}
						break;
					case 0xA:
					{
						// CMP
						if (S) {
							uint32_t input = Op1;
							uint32_t result = Op1 - Op2;
							SetBit(CPSR, 29);
							SetFlagsSub(result, (Op2 > Op1), (Overflow(Op1, Op2, result, false)), true);
							//curInsnAsm = "CMP";
						}
						break;
					}
					case 0xB:
					{
						// CMN
						uint32_t input = Op1;
						uint32_t result = Op1 + Op2;
						if (S) {
							bool poss = false;
							if (Op1 > 0 || Op2 > 0) {
								poss = true;
							}
							uint64_t tst = uint64_t(Op1) + uint64_t(Op2);
							SetFlags(result, ((tst > 0xFFFFFFFF) || (poss && result == 0)), (Overflow(Op1, Op2, result, true)), true);
							//curInsnAsm = "CMN";
						}
						break;
					}
					case 0xC:
						// ORR
						registers[Rd] = Op1 | Op2;
						if (S) {
							SetFlags(registers[Rd], carry, false, false);
						}
						//curInsnAsm = "ORR";
						break;
					case 0xD:
						// MOV
						registers[Rd] = Op2;
						if (Rd == 30) {
							increment = false;
						}
						if (S) {
							SetFlags(registers[Rd], carry, false, false);
						}
						//curInsnAsm = "MOV";
						break;
					case 0xE:
						// BIC
						registers[Rd] = Op1 & ~(Op2);
						if (S) {
							SetFlags(registers[Rd], carry, false, false);
						}
						//curInsnAsm = "BIC";
						break;
					case 0xF:
						// MVN
						registers[Rd] = ~(Op2);
						if (S) {
							SetFlags(registers[Rd], carry, false, false);
						}
						//curInsnAsm = "MVN";
						break;

					default:
						break;
					}
				}
			}

		}

		bool multiply = false;
		// Multiply and Multiply-Accumulate
		if ((bitrange(27, 25, insn) == 0b000) && (bitrange(7, 4, insn) == 0b1001)) {
			uint32_t Rd = bitrange(19, 16, insn);
			uint32_t Rn = bitrange(15, 12, insn);
			uint32_t Rs = bitrange(11, 8, insn);
			uint32_t Rm = bitrange(3, 0, insn);
			multiply = true;
			switch (bitrange(24, 21, insn))
			{
			case 0b0000:
				// MUL
				registers[WhatRegister(Rd)] = registers[WhatRegister(Rm)] * registers[WhatRegister(Rs)];
				if (registers[Rd] == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if ((registers[Rd] >> 31) & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				//curInsnAsm = "MUL";
				break;
			case 0b0001:
				// MLA
				registers[WhatRegister(Rd)] = (registers[WhatRegister(Rm)] * registers[WhatRegister(Rs)]) + registers[WhatRegister(Rn)];
				if (registers[Rd] == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if ((registers[Rd] >> 31) & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				//curInsnAsm = "MLA";
				break;
			case 0b0100:
			{
				// UMULL
				if (registers[0] == 0x01066797 && registers[1] == 0xD5B9BEAC && registers[2] == 0xFC0000F8) {
					Dummy();
				}
				uint64_t Op1 = static_cast<uint64_t>(registers[WhatRegister(Rs)]);
				uint64_t Op2 = static_cast<uint64_t>(registers[WhatRegister(Rm)]);
				if (Op1 == -1) {
					Dummy();
				}
				uint64_t result = Op1 * Op2;
				registers[WhatRegister(Rn)] = (result << 32) >> 32;
				registers[WhatRegister(Rd)] = result >> 32;
				if (result == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if ((result >> 63) & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				//curInsnAsm = "UMULL";
				break;
			}
			case 0b0101:
			{
				// UMLAL
				uint64_t hi_lo = registers[WhatRegister(Rd)];
				hi_lo <<= 16;
				hi_lo <<= 16;
				hi_lo |= registers[WhatRegister(Rn)];

				uint64_t accumulate = registers[WhatRegister(Rd)] << 32 | registers[WhatRegister(Rn)];
				uint64_t Op1 = static_cast<uint64_t>(registers[WhatRegister(Rs)]);
				uint64_t Op2 = static_cast<uint64_t>(registers[WhatRegister(Rm)]);
				uint64_t result = (Op1 * Op2) + hi_lo;
				registers[WhatRegister(Rn)] = ((result << 32) >> 32);
				registers[WhatRegister(Rd)] = (result >> 32);
				if (result == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if ((result >> 63) & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				//curInsnAsm = "UMLAL";
				break;
			}
			case 0b0110:
			{
				// SMULL
				Dummy();
				if (registers[WhatRegister(Rs)] == -8) {
					Dummy();
				}
				if (registers[WhatRegister(Rm)] == -8) {
					Dummy();
				}
				int64_t Op1 = int32_t(registers[WhatRegister(Rs)]);
				int64_t Op2 = int32_t(registers[WhatRegister(Rm)]);

				int64_t result = Op1 * Op2;
				registers[WhatRegister(Rn)] = (result << 32) >> 32;
				registers[WhatRegister(Rd)] = result >> 32;
				if (result == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if ((result >> 63) & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				//curInsnAsm = "SMULL";
				break;
			}
			case 0b0111:
			{
				// SMLAL
				if (registers[0] == 0x06E74E70 && registers[1] == 0x88DCF44E) {
					Dummy();
				}
				Dummy();
				if (registers[WhatRegister(Rs)] == -8) {
					Dummy();
				}
				if (registers[WhatRegister(Rm)] == -8) {
					Dummy();
				}
				int64_t hi_lo = registers[WhatRegister(Rd)];
				hi_lo <<= 16;
				hi_lo <<= 16;
				hi_lo |= registers[WhatRegister(Rn)];

				int64_t in1 = static_cast<int32_t>(registers[WhatRegister(Rs)]);
				int64_t in2 = static_cast<int32_t>(registers[WhatRegister(Rm)]);

				int64_t result = (in1 * in2) + hi_lo;
				registers[WhatRegister(Rn)] = (result << 32) >> 32;
				registers[WhatRegister(Rd)] = (result >> 32);
				if (result == 0) {
					SetBit(CPSR, 30);
				}
				else
				{
					ClearBit(CPSR, 30);
				}
				if ((result >> 63) & 1) {
					SetBit(CPSR, 31);
				}
				else
				{
					ClearBit(CPSR, 31);
				}
				//curInsnAsm = "SMLAL";
				break;
			}
			}
		}

		// Single Data Transfer LDR STR PLD
		if (bitrange(27, 26, insn) == 0b01) {
			int Rd = bitrange(15, 12, insn);  // Source / destination register
			int Rn = bitrange(19, 16, insn);  // Base register
			int Rm = bitrange(3, 0, insn);	  // Offset register
			if (Rd == 15) {
				Dummy();
			}
			uint32_t address = registers[WhatRegister(Rn)];
			uint32_t offsetAmnt = 0;
			bool I = false;
			bool P = false;
			bool U = false;
			bool B = false;
			bool WriteBack = true;

			if ((insn >> 25) & 1) {
				I = true;
			}
			if ((insn >> 24) & 1) {
				P = true;
				if (!((insn >> 21) & 1)) {
					WriteBack = false;
				}
			}
			if ((insn >> 23) & 1) {
				U = true;
			}
			if ((insn >> 22) & 1) {
				B = true;
			}



			if (I) {
				// Shifted register
				if (Rd == 2) {
					Dummy();
				}
				uint8_t shiftAmnt = bitrange(11, 7, insn);
				offsetAmnt = registers[WhatRegister(Rm)];
				switch (bitrange(6, 5, insn))
				{
				case 0:
					offsetAmnt = offsetAmnt << shiftAmnt;
					break;
				case 1:
					offsetAmnt = offsetAmnt >> shiftAmnt;
					if (shiftAmnt == 0) {
						offsetAmnt = 0;
					}
					break;
				case 2:

					if (shiftAmnt == 0) {
						if (offsetAmnt >> 31 & 1) {
							offsetAmnt = 0;
							offsetAmnt = ~0;
						}
						else
						{
							offsetAmnt = 0;
						}
					}
					else
					{
						offsetAmnt = arithmeticRight(offsetAmnt, shiftAmnt);
					}
					break;
				case 3:
					Dummy();
					if (shiftAmnt == 0) {
						offsetAmnt = offsetAmnt >> 1;
						if ((CPSR >> 29) & 1) {
							SetBit(offsetAmnt, 31);
						}
						else
						{
							ClearBit(offsetAmnt, 31);
						}
					}
					offsetAmnt = rotateRight(offsetAmnt, shiftAmnt);
					break;
				}
			}
			else
			{
				offsetAmnt = bitrange(11, 0, insn);
			}

			if (P) {
				if (U) {
					address += offsetAmnt;
				}
				else
				{
					address -= offsetAmnt;
				}
			}

			if (address == 67109384) {
				Dummy();
			}

			if ((insn >> 20) & 1) {
				// LDR
				uint32_t rotAmnt = 0;
				if (address % 4 != 0) {
					rotAmnt = bitrange(1, 0, address);
					rotAmnt *= 8;
				}
				if (Rd == Rn) {
					WriteBack = false;
				}
				if (Rn == 15) {
					address += 8;
				}
				if (B) {
					registers[WhatRegister(Rd)] = mmu.MemoryRead(address);
					//if (Rd == 15) {
					//	registers[WhatRegister(Rd)] += 4;
					//}
					//curInsnAsm = "LDR{B}{T}";
				}
				else
				{
					if (registers[30] == 0x3000178) {
						Dummy();
					}

					uint32_t val1 = mmu.MemoryRead(address);
					uint32_t val2 = mmu.MemoryRead(address + 1);
					uint32_t val3 = mmu.MemoryRead(address + 2);
					uint32_t val4 = mmu.MemoryRead(address + 3);



					AlignAddr(address);
					uint32_t val = 0;
					val = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
					//uint32_t val = mmu.MemoryReadWord(address);
					registers[WhatRegister(Rd)] = rotateRight(val, rotAmnt);
					if (Rd == 15) {
						increment = false;
					}
					//curInsnAsm = "LDR{W}{T}";
				}

			}
			else
			{
				// STR
				uint32_t val = registers[WhatRegister(Rd)];
				if (Rd == 15) {
					val += 12;
				}
				if (B) {
					mmu.MemoryWrite(address, static_cast<uint8_t>(registers[WhatRegister(Rd)]));
					//curInsnAsm = "STR{B}{T}";
				}
				else
				{
					AlignAddr(address);
					//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
					//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
					//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
					//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
					mmu.MemoryWrite(address, val);
					//curInsnAsm = "STR{W}{T}";
				}


			}
			if (!P) {
				if (U) {
					address += offsetAmnt;
				}
				else
				{
					address -= offsetAmnt;
				}
			}
			if (WriteBack) {
				registers[WhatRegister(Rn)] = address;
			}
		}

		// Half/Double/Signed Data Transfer
		if ((bitrange(27, 25, insn) == 0b000) && ((insn >> 7) & 1) && ((insn >> 4) & 1)) {
			if (!multiply) {
				uint32_t Rd = bitrange(15, 12, insn);  // Source / destination register
				if (Rd == 7) {
					Dummy();
				}
				uint32_t Rn = bitrange(19, 16, insn);  // Base register
				uint32_t address = registers[WhatRegister(Rn)];
				uint32_t offset = 0;
				bool P = false;
				bool U = false;
				bool I = false;
				bool L = false;
				bool WriteBack = true;

				if ((insn >> 23) & 1) {
					U = true;
				}
				if ((insn >> 22) & 1) {
					I = true;
					uint8_t upperOffset = bitrange(11, 8, insn);
					uint8_t lowerOffset = bitrange(3, 0, insn);
					offset = (upperOffset << 4) | lowerOffset;
				}
				else
				{
					uint32_t Rm = bitrange(3, 0, insn);
					offset = registers[WhatRegister(Rm)];
				}
				if ((insn >> 24) & 1) {
					P = true;
					if (U) {
						address += offset;
					}
					else
					{
						address -= offset;
					}


					if (!((insn >> 21) & 1)) {
						WriteBack = false;
					}
				}

				if (address == 67109384) {
					Dummy();
				}

				if ((insn >> 20) & 1) {
					switch (bitrange(6, 5, insn))
					{
					case 0x0:
						// Reserved
						//curInsnAsm = "NOP";
						break;
					case 0x1:
					{
						// Load unsigned halfword
						if (Rd == Rn) {
							WriteBack = false;
						}
						uint32_t rotAmnt = 0;
						//rotAmnt = bitrange(1, 0, address);
						//rotAmnt *= 8;
						if (address % 2 != 0) {
							address -= 1;
						}
						uint32_t val = (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
						val = rotateRight(val, rotAmnt);
						registers[WhatRegister(Rd)] = val;
						//curInsnAsm = "LDR H";
						break;
					}
					case 0x2:
					{
						// Load signed byte
						if (Rd == Rn) {
							WriteBack = false;
						}
						uint8_t val = mmu.MemoryRead(address);
						if (val == 255) {
							Dummy();
						}
						if ((val >> 7) & 1) {
							registers[WhatRegister(Rd)] = (0xFFFFFF << 8) | val;
						}
						else
						{
							registers[WhatRegister(Rd)] = val;
						}
						//curInsnAsm = "LDR SB";
						break;
					}
					case 0x3:
					{
						// Load signed halfword
						if (Rd == Rn) {
							WriteBack = false;
						}
						uint32_t rotAmnt = 0;
						//rotAmnt = bitrange(1, 0, address);
						//rotAmnt *= 8;
						if (address % 4 != 0) {
							Dummy();
						}
						if (address % 2 != 0) {
							address -= 1;
						}

						uint32_t val = (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
						if ((val >> 15) & 1) {
							val = rotateRight(val, rotAmnt);
							if (rotAmnt == 8) {
								val = (0xFFFFFF << 8) | val;
							}
							else
							{
								val = (0xFFFF << 16) | val;
							}
							registers[WhatRegister(Rd)] = val;
						}
						else
						{
							registers[WhatRegister(Rd)] = val;
						}
						//curInsnAsm = "LDR SH";
						break;
					}
					}
				}
				else
				{
					switch (bitrange(6, 5, insn))
					{
					case 0x0:
					{
						Dummy();
					}
					break;
					case 0x1:
						// Store halfword
						if (address % 4 != 0) {
							Dummy();
						}
						if (address % 2 != 0) {
							address -= 1;
						}
						if (address == 0x60001e2) {
							Dummy();
						}
						mmu.MemoryWrite(address, static_cast<uint16_t>(bitrange(15, 0, registers[WhatRegister(Rd)])));
						//curInsnAsm = "STR H";
						break;
					case 0x2:
						// Load doubleword
						if (address % 4 != 0) {
							Dummy();
						}
						if (Rd == Rn) {
							WriteBack = false;
						}
						registers[WhatRegister(Rd)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
						registers[WhatRegister(Rd) + 1] = (mmu.MemoryRead(address + 7) << 24) | (mmu.MemoryRead(address + 6) << 16) | (mmu.MemoryRead(address + 5) << 8) | mmu.MemoryRead(address + 4);
						//curInsnAsm = "LDR D";
						break;
					case 0x3:
						// Store doubleword
						if (address % 4 != 0) {
							Dummy();
						}
						mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, registers[WhatRegister(Rd)])));
						mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, registers[WhatRegister(Rd)])));
						mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, registers[WhatRegister(Rd)])));
						mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, registers[WhatRegister(Rd)])));
						mmu.MemoryWrite(address + 4, static_cast<uint8_t>(bitrange(7, 0, registers[WhatRegister(Rd) + 1])));
						mmu.MemoryWrite(address + 5, static_cast<uint8_t>(bitrange(15, 8, registers[WhatRegister(Rd) + 1])));
						mmu.MemoryWrite(address + 6, static_cast<uint8_t>(bitrange(23, 16, registers[WhatRegister(Rd) + 1])));
						mmu.MemoryWrite(address + 7, static_cast<uint8_t>(bitrange(31, 24, registers[WhatRegister(Rd) + 1])));
						//curInsnAsm = "STR D";
						break;
					}
				}
				if (!P) {
					if (U) {
						address += offset;
					}
					else
					{
						address -= offset;
					}
				}
				if (WriteBack) {
					registers[WhatRegister(Rn)] = address;
				}
			}
		}

		// Block Data Transfer
		if (bitrange(27, 25, insn) == 0b100) {
			if (registers[30] == 0x8001900) {
				Dummy();
			}
			int Rn = bitrange(19, 16, insn);
			uint32_t address = registers[WhatRegister(Rn)];
			uint32_t oldAddr = address;
			int Rlist = 0;
			bool P = false;
			bool U = false;
			bool S = false;
			bool W = false;
			bool L = false;
			bool stm = false;
			if ((insn >> 24) & 1) {
				P = true;
			}
			if ((insn >> 23) & 1) {
				U = true;
			}
			if ((insn >> 21) & 1) {
				W = true;
			}
			if ((insn >> 20) & 1) {
				L = true;
			}
			bool found = false;
			bool match = false;
			int matchNum = -1;
			int first = -1;
			int last = -1;
			bool pc = false;
			for (int i = 0; i < 16; i++) {
				if ((insn >> i) & 1) {
					Rlist += 1;
					if (!found) {
						first = i;
						found = true;
					}
					last = i;
					if (i == Rn) {
						match = true;
						matchNum = i;
					}
					if (i == 15) {
						pc = true;
					}
					if (i == 14) {
						Dummy();
					}
				}
			}
			if (L && pc) {
				increment = false;
			}
			if ((insn >> 22) & 1) {
				S = true;
				if (L && pc) {
					Dummy();
					switch (mode)

					{
					case Arm::Mode::User:
						break;
					case Arm::Mode::Fiq:
						CPSR = SPSR_fiq;
						break;
					case Arm::Mode::Irq:
						CPSR = SPSR_irq;
						break;
					case Arm::Mode::Supervisor:
						CPSR = SPSR_svc;
						break;
					case Arm::Mode::Abort:
						CPSR = SPSR_abt;
						break;
					case Arm::Mode::Undefined:
						CPSR = SPSR_und;
						break;
					case Arm::Mode::System:
						break;
					}
				}
				else
				{
					mode = Mode::User;
				}

			}
			bool empty = false;

			if (address % 4 != 0) {
				Dummy();
			}
			if (Rlist == 0) {
				empty = true;
				W = false;
				if (U) {
					if (P) {
						if (L) {
							//LDMIB
							registers[30] = mmu.MemoryReadWord(address);
							increment = false;
							registers[WhatRegister(Rn)] = address + 0x40;
						}
						else
						{
							//STMIB
							mmu.MemoryWrite(address + 4, registers[30] + 12);
							registers[WhatRegister(Rn)] = address + 0x40;
						}
					}
					else
					{
						if (L) {
							//LDMIA
							registers[30] = mmu.MemoryReadWord(address);
							increment = false;
							registers[WhatRegister(Rn)] = address + 0x40;
						}
						else
						{
							//STMIA
							mmu.MemoryWrite(address, registers[30] + 12);
							registers[WhatRegister(Rn)] = address + 0x40;
						}
					}
				}
				else
				{
					if (P) {
						if (L) {
							//LDMDB
							registers[30] = mmu.MemoryReadWord(address);
							increment = false;
							registers[WhatRegister(Rn)] = address - 0x40;
						}
						else
						{
							//STMDB
							mmu.MemoryWrite(address - 0x40, registers[30] + 12);
							registers[WhatRegister(Rn)] = address - 0x40;
						}
					}
					else
					{
						if (L) {
							//LDMDA
							registers[30] = mmu.MemoryReadWord(address);
							increment = false;
							registers[WhatRegister(Rn)] = address - 0x40;
						}
						else
						{
							//STMDA
							mmu.MemoryWrite(address - 0x3C, registers[30] + 12);
							registers[WhatRegister(Rn)] = address - 0x40;
						}
					}
				}
			}
			int index = -1;
			bool ran = false;
			uint32_t finishedAddr = 0;

			if (address == 67109384) {
				Dummy();
			}

			for (int i = 0; i < 16; i++) {
				if (((insn >> i) & 1)) {
					if (empty) {
						i = 15;
						empty = false;
					}
					uint32_t val = registers[WhatRegister(i)];
					if (i == 15) {
						val += 12;
					}

					index += 1;
					if (P) {
						if (U) {
							if (insn == 0xE9A00000) {
								Dummy();
							}
							if (!ran) {
								finishedAddr = address + (Rlist * 4);
								address = address + 4;
								ran = true;
							}
							else
							{
								address += 4;
							}

							if (L) {
								registers[WhatRegister(i)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
								//if (i == 15) {
								//	registers[WhatRegister(i)] += 4;
								//}
								//curInsnAsm = "LDM IB";
							}
							else
							{
								if (i == matchNum && matchNum != first) {
									val = finishedAddr;
								}
								mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
								mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
								mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
								mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
								//curInsnAsm = "STM IB";
								stm = true;
							}
						}
						else
						{
							if (!ran) {
								finishedAddr = address - ((Rlist) * 4);
								address = finishedAddr;
								ran = true;
							}
							else
							{
								address += 4;
							}
							//if (Rlist == 0) {
							//	address -= 0x40;
							//
							//}
							if (L) {
								registers[WhatRegister(i)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
								//if (i == 15) {
								//	registers[WhatRegister(i)] += 4;
								//}
								//curInsnAsm = "LDM DB";
							}
							else
							{
								if (i == matchNum && matchNum != first) {
									val = finishedAddr;
								}
								mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
								mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
								mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
								mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
								//curInsnAsm = "STM DB";
								stm = true;
							}
						}
					}
					else
					{
						if (U)
						{
							Dummy();
							if (!ran) {
								finishedAddr = address + (Rlist * 4);
								address = address;
								ran = true;
							}
							else
							{
								address += 4;
							}

							Dummy();
							if (L) {

								registers[WhatRegister(i)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
								//if (i == 15) {
								//	registers[WhatRegister(i)] += 4;
								//}
								//curInsnAsm = "LDM IA";
							}
							else
							{
								if (i == matchNum && matchNum != first) {
									val = finishedAddr;
								}
								mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
								mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
								mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
								mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
								//curInsnAsm = "STM IA";
								stm = true;
							}
						}
						else
						{
							if (!ran) {
								finishedAddr = (address - ((Rlist) * 4));
								address = finishedAddr + 4;


								ran = true;
							}
							else
							{
								address += 4;
							}
							//if (Rlist == 0) {
							//	address -= 0x40;
							//
							//}
							if (L) {
								registers[WhatRegister(i)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
								//if (i == 15) {
								//	registers[WhatRegister(i)] += 4;
								//}
								//curInsnAsm = "LDM DA";
							}
							else
							{
								if (i == matchNum && matchNum != first) {
									val = finishedAddr;
								}
								mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
								mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
								mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
								mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));

								//curInsnAsm = "STM DA";
								stm = true;
							}
						}
					}
				}
			}
			if (match) {
				if (Rn == first) {
					if (!stm) {
						W = false;
					}
				}
				else if (Rn == last) {
					Dummy();
					if (!stm) {
						W = false;
					}
				}
				else
				{
					Dummy();
					if (!stm) {
						W = false;
					}
				}
			}

			if (W) {
				if (insn == 0xE9A00000) {
					Dummy();
				}
				//if (Rlist == 0) {
				//	if (U) {
				//		if (!P) {
				//			registers[WhatRegister(Rn)] = finishedAddr;
				//			registers[WhatRegister(Rn)] += 0x40;
				//		}
				//		else
				//		{
				//			registers[WhatRegister(Rn)] += 0x40;
				//			//registers[WhatRegister(Rn)] -= 4;
				//		}
				//	}
				//	else
				//	{
				//		registers[WhatRegister(Rn)] = address;
				//		if (!P) {
				//			registers[WhatRegister(Rn)] -= 4;
				//		}
				//	}
				//}
				//else
				//{
				registers[WhatRegister(Rn)] = finishedAddr;
				//}

			}
		}

		// Single Data Swap
		if ((bitrange(27, 23, insn) == 0b00010) && (bitrange(21, 20, insn) == 0) && (bitrange(11, 4, insn) == 0b00001001)) {
			int Rm = bitrange(3, 0, insn);
			int Rn = bitrange(19, 16, insn);
			int Rd = bitrange(15, 12, insn);

			uint32_t address = registers[WhatRegister(Rn)];
			if ((insn >> 22) & 1) {
				Dummy();
				if (Rd == Rm) {
					uint32_t temp = registers[WhatRegister(Rd)];
					registers[WhatRegister(Rd)] = mmu.MemoryRead(address);
				}
				else
				{
					uint8_t val = mmu.MemoryRead(address);
					mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, registers[WhatRegister(Rm)])));
					registers[WhatRegister(Rd)] = val;
				}
			}
			else
			{

				uint32_t rotAmnt = 0;
				rotAmnt = bitrange(1, 0, address);
				rotAmnt *= 8;
				AlignAddr(address);
				uint32_t val = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
				val = rotateRight(val, rotAmnt);
				if (Rd == Rm) {
					uint32_t temp = registers[WhatRegister(Rd)];
					registers[WhatRegister(Rd)] = val;
					mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, temp)));
					mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, temp)));
					mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, temp)));
					mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, temp)));
				}
				else
				{
					mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, registers[WhatRegister(Rm)])));
					mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, registers[WhatRegister(Rm)])));
					mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, registers[WhatRegister(Rm)])));
					mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, registers[WhatRegister(Rm)])));
					registers[WhatRegister(Rd)] = val;
				}
			}
			//curInsnAsm = "SWP";
		}
		
		//ArmFunc[nInsn];

	}
}

void Arm::B(uint32_t insn)
{
	int32_t offset = bitrange(23, 0, insn);

	offset <<= 2;
	if (offset & 0x2000000) {
		offset |= 0xFC000000;
	}

	registers[30] = registers[30] + offset + 8;
	increment = false;
}

void Arm::BL(uint32_t insn)
{
	int32_t offset = bitrange(23, 0, insn);
	offset <<= 2;
	if (offset & 0x2000000) {
		offset |= 0xFC000000;
	}

	registers[WhatRegister(14)] = registers[30] + 4;
	registers[30] = registers[30] + offset + 8;
	increment = false;
}

void Arm::MRSR(MMU & mmu)
{
	// MRS
	// SPSR
	//curInsnAsm = "MRSR";
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	switch (mode)
	{
	case Arm::Mode::User:
		break;
	case Arm::Mode::Fiq:
		registers[Rd] = SPSR_fiq;
		break;
	case Arm::Mode::Irq:
		registers[Rd] = SPSR_irq;
		break;
	case Arm::Mode::Supervisor:
		registers[Rd] = SPSR_svc;
		break;
	case Arm::Mode::Abort:
		registers[Rd] = SPSR_abt;
		break;
	case Arm::Mode::Undefined:
		registers[Rd] = SPSR_und;
		break;
	case Arm::Mode::System:
		registers[Rd] = CPSR;
		break;
	}
}

void Arm::MRS(MMU & mmu)
{
	// MRS
	// CPSR
	//curInsnAsm = "MRS";
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	registers[Rd] = CPSR;
}

void Arm::MSR(MMU & mmu)
{
	//curInsnAsm = "MSR";
	int Rm = WhatRegister(bitrange(3, 0, Ainsn));
	uint32_t val = registers[Rm];
	uint32_t mask = 0;
	if ((Ainsn >> 19) & 1) {
		mask |= 0xFF000000;
	}
	if ((Ainsn >> 18) & 1) {
		mask |= 0x00FF0000;
	}
	if ((Ainsn >> 17) & 1) {
		mask |= 0x0000FF00;
	}
	if ((Ainsn >> 16) & 1) {
		mask |= 0x000000FF;
	}
	//CPSR
	uint32_t old_CPSR = CPSR;
	CPSR &= ~mask;
	CPSR |= val;
	switch (bitrange(4, 0, CPSR))
	{
	case 0b10001:
		//FIQ
		SPSR_fiq = old_CPSR;
		break;
	case 0b10010:
		//IRQ
		SPSR_irq = old_CPSR;
		//registers[WhatRegister(14)] = registers[30] + 4;
		//registers[30] = 0x18;
		//increment = false;
		break;
	case 0b10011:
		//SWI
		SPSR_svc = old_CPSR;
		break;
	case 0b10111:
		//ABT
		SPSR_abt = old_CPSR;
		break;
	case 0b11011:
		//UND
		SPSR_und = old_CPSR;
		break;
	}
}

void Arm::MSRR(MMU & mmu)
{
	//curInsnAsm = "MSRR";
	int Rm = WhatRegister(bitrange(3, 0, Ainsn));
	uint32_t val = registers[Rm];
	uint32_t mask = 0;
	if ((Ainsn >> 19) & 1) {
		mask |= 0xFF000000;
	}
	if ((Ainsn >> 18) & 1) {
		mask |= 0x00FF0000;
	}
	if ((Ainsn >> 17) & 1) {
		mask |= 0x0000FF00;
	}
	if ((Ainsn >> 16) & 1) {
		mask |= 0x000000FF;
	}
	// SPSR_MODE
	switch (mode)
	{
	case Arm::Mode::User:
		break;
	case Arm::Mode::Fiq:
		SPSR_fiq = val;
		break;
	case Arm::Mode::Irq:
		SPSR_irq = val;
		break;
	case Arm::Mode::Supervisor:
		SPSR_svc = val;
		break;
	case Arm::Mode::Abort:
		SPSR_abt = val;
		break;
	case Arm::Mode::Undefined:
		SPSR_und = val;
		break;
	case Arm::Mode::System:
		break;
	}
}

void Arm::MSRI(MMU & mmu)
{
	//curInsnAsm = "MSRI";
	// MSR Immediate
	uint32_t mask = 0;
	if ((Ainsn >> 19) & 1) {
		mask |= 0xFF000000;
	}
	if ((Ainsn >> 18) & 1) {
		mask |= 0x00FF0000;
	}
	if ((Ainsn >> 17) & 1) {
		mask |= 0x0000FF00;
	}
	if ((Ainsn >> 16) & 1) {
		mask |= 0x000000FF;
	}
	uint32_t imm = bitrange(7, 0, Ainsn);
	uint16_t shiftAmnt = (bitrange(11, 8, Ainsn)) * 2;
	imm = rotateRight(imm, shiftAmnt);
	//CPSR
	uint32_t old_CPSR = CPSR;
	CPSR &= ~mask;
	CPSR |= imm;
	switch (bitrange(4, 0, CPSR))
	{
	case 0b10001:
		//FIQ
		SPSR_fiq = old_CPSR;
		break;
	case 0b10010:
		//IRQ
		SPSR_irq = old_CPSR;
		break;
	case 0b10011:
		//SWI
		SPSR_svc = old_CPSR;
		break;
	case 0b10111:
		//ABT
		SPSR_abt = old_CPSR;
		break;
	case 0b11011:
		//UND
		SPSR_und = old_CPSR;
		break;
	default:
		break;
	}
}

void Arm::MSRRI(MMU & mmu)
{
	//curInsnAsm = "MSRRI";
	// MSR Immediate
	uint32_t mask = 0;
	if ((Ainsn >> 19) & 1) {
		mask |= 0xFF000000;
	}
	if ((Ainsn >> 18) & 1) {
		mask |= 0x00FF0000;
	}
	if ((Ainsn >> 17) & 1) {
		mask |= 0x0000FF00;
	}
	if ((Ainsn >> 16) & 1) {
		mask |= 0x000000FF;
	}
	uint32_t imm = bitrange(7, 0, Ainsn);
	uint16_t shiftAmnt = (bitrange(11, 8, Ainsn)) * 2;
	imm = rotateRight(imm, shiftAmnt);
	// SPSR_MODE
	switch (mode)
	{
	case Arm::Mode::User:
		break;
	case Arm::Mode::Fiq:
		SPSR_fiq = imm;
		break;
	case Arm::Mode::Irq:
		SPSR_irq = imm;
		break;
	case Arm::Mode::Supervisor:
		SPSR_svc = imm;
		break;
	case Arm::Mode::Abort:
		SPSR_abt = imm;
		break;
	case Arm::Mode::Undefined:
		SPSR_und = imm;
		break;
	case Arm::Mode::System:
		break;
	}
	
}

void Arm::GenerateTable(MMU& mmu)
{
	for (int i = 0; i < 0xfff; i++) {
		
		int upper = (i & 0b111111110000) >> 4;
		int lower = i & 0b1111;
		
		using x = Arm;

		if (upper <= 0b00011111) {
			if (lower == 0b1001) {
				if (upper == 0 || upper == 1) {
					ArmFunc[i] = &x::MUL;
				}
				else if (upper == 2 || upper == 3) {
					ArmFunc[i] = &x::MLA;
				}
				else if (upper == 8 || upper == 9) {
					ArmFunc[i] = &x::UMULL;
				}
				else if (upper == 10 || upper == 11) {
					ArmFunc[i] = &x::UMLAL;
				}
				else if (upper == 12 || upper == 13) {
					ArmFunc[i] = &x::SMULL;
				}
				else if (upper == 14 || upper == 15) {
					ArmFunc[i] = &x::SMLAL;
				}
				else if (upper == 16) {
					ArmFunc[i] = &x::SWP;
				}
				else if (upper == 20) {
					ArmFunc[i] = &x::SWPB;
				}
				else if (upper >= 24 && upper <= 31) {
					ArmFunc[i] = &x::SMLAL;
				}
			}
			else if (lower == 0b1011) {
				ArmFunc[i] = &x::HDSDT;
			}
			else if ((lower == 0b1101 || lower == 0b1111) && (upper % 2 != 0)) {
				ArmFunc[i] = &x::HDSDT;
			}
			else if (upper == 0 || upper == 1) {
				ArmFunc[i] = &x::AND;
			}
			else if (upper == 2 || upper == 3) {
				ArmFunc[i] = &x::EOR;
			}
			else if (upper == 4 || upper == 5) {
				ArmFunc[i] = &x::SUB;
			}
			else if (upper == 6 || upper == 7) {
				ArmFunc[i] = &x::RSB;
			}
			else if (upper == 8 || upper == 9) {
				ArmFunc[i] = &x::ADD;
			}
			else if (upper == 10 || upper == 11) {
				ArmFunc[i] = &x::ADC;
			}
			else if (upper == 12 || upper == 13) {
				ArmFunc[i] = &x::SBC;
			}
			else if (upper == 14 || upper == 15) {
				ArmFunc[i] = &x::RSC;
			}
			else if (upper == 16 && lower == 0) {
				ArmFunc[i] = &x::MRS;
			}
			else if (upper == 17) {
				ArmFunc[i] = &x::TST;
			}
			else if (upper == 18) {
				if (lower == 0) {
					ArmFunc[i] = &x::MSR;
				}
				else if (lower == 1) {
					ArmFunc[i] = &x::BrX;
				}
			}
			else if (upper == 19) {
				ArmFunc[i] = &x::TEQ;
			}
			else if (upper == 20 && lower == 0) {
				ArmFunc[i] = &x::MRSR;
			}
			else if (upper == 21) {
				ArmFunc[i] = &x::CMP;
			}
			else if (upper == 22 && lower == 0) {
				ArmFunc[i] = &x::MSRR;
			}
			else if (upper == 23) {
				ArmFunc[i] = &x::CMN;
			}
			else if (upper == 24 || upper == 25) {
				ArmFunc[i] = &x::ORR;
			}
			else if (upper == 26 || upper == 27) {
				ArmFunc[i] = &x::MOV;
			}
			else if (upper == 28 || upper == 29) {
				ArmFunc[i] = &x::BIC;
			}
			else if (upper == 30 || upper == 31) {
				ArmFunc[i] = &x::MVN;
			}
		}
		else if (upper == 32 || upper == 33) {
			ArmFunc[i] = &x::AND;
		}
		else if (upper == 34 || upper == 35) {
			ArmFunc[i] = &x::EOR;
		}
		else if (upper == 36 || upper == 37) {
			ArmFunc[i] = &x::SUB;
		}
		else if (upper == 38 || upper == 39) {
			ArmFunc[i] = &x::RSB;
		}
		else if (upper == 40 || upper == 41) {
			ArmFunc[i] = &x::ADD;
		}
		else if (upper == 42 || upper == 43) {
			ArmFunc[i] = &x::ADC;
		}
		else if (upper == 44 || upper == 45) {
			ArmFunc[i] = &x::SBC;
		}
		else if (upper == 46 || upper == 47) {
			ArmFunc[i] = &x::RSC;
		}
		else if (upper == 48 || upper == 49) {
			ArmFunc[i] = &x::TST;
		}
		else if (upper == 50) {
			ArmFunc[i] = &x::MSRI;
		}
		else if (upper == 51) {
			ArmFunc[i] = &x::TEQ;
		}
		else if (upper == 52 || upper == 53) {
			ArmFunc[i] = &x::CMP;
		}
		else if (upper == 54) {
			ArmFunc[i] = &x::MSRRI;
		}
		else if (upper == 55) {
			ArmFunc[i] = &x::CMN;
		}
		else if (upper == 56 || upper == 57) {
			ArmFunc[i] = &x::ORR;
		}
		else if (upper == 58 || upper == 59) {
			ArmFunc[i] = &x::MOV;
		}
		else if (upper == 60 || upper == 61) {
			ArmFunc[i] = &x::BIC;
		}
		else if (upper == 62 || upper == 63) {
			ArmFunc[i] = &x::MVN;
		}
		//Single Data Transfer Immediate
		else if (upper == 64) {
			ArmFunc[i] = &x::STRI;
		}
		else if (upper == 65) {
			ArmFunc[i] = &x::LDRI;
		}
		else if (upper == 66) {
			ArmFunc[i] = &x::STRTI;
		}
		else if (upper == 67) {
			ArmFunc[i] = &x::LDRTI;
		}
		else if (upper == 68) {
			ArmFunc[i] = &x::STRBI;
		}
		else if (upper == 69) {
			ArmFunc[i] = &x::LDRBI;
		}
		else if (upper == 70) {
			ArmFunc[i] = &x::STRBTI;
		}
		else if (upper == 71) {
			ArmFunc[i] = &x::LDRBTI;
		}
		else if (upper == 72) {
			ArmFunc[i] = &x::STRI;
		}
		else if (upper == 73) {
			ArmFunc[i] = &x::LDRI;
		}
		else if (upper == 74) {
			ArmFunc[i] = &x::STRTI;
		}
		else if (upper == 75) {
			ArmFunc[i] = &x::LDRTI;
		}
		else if (upper == 76) {
			ArmFunc[i] = &x::STRBI;
		}
		else if (upper == 77) {
			ArmFunc[i] = &x::LDRBI;
		}
		else if (upper == 78) {
			ArmFunc[i] = &x::STRBTI;
		}
		else if (upper == 79) {
			ArmFunc[i] = &x::LDRBTI;
		}
		else if (upper == 80) {
			ArmFunc[i] = &x::STRPI;
		}
		else if (upper == 81) {
			ArmFunc[i] = &x::LDRPI;
		}
		else if (upper == 82) {
			ArmFunc[i] = &x::STRPI;
		}
		else if (upper == 83) {
			ArmFunc[i] = &x::LDRPI;
		}
		else if (upper == 84) {
			ArmFunc[i] = &x::STRBPI;
		}
		else if (upper == 85) {
			ArmFunc[i] = &x::LDRBPI;
		}
		else if (upper == 86) {
			ArmFunc[i] = &x::STRBPI;
		}
		else if (upper == 87) {
			ArmFunc[i] = &x::LDRBPI;
		}
		else if (upper == 88) {
			ArmFunc[i] = &x::STRPUI;
		}
		else if (upper == 89) {
			ArmFunc[i] = &x::LDRPUI;
		}
		else if (upper == 90) {
			ArmFunc[i] = &x::STRPUWI;
		}
		else if (upper == 91) {
			ArmFunc[i] = &x::LDRPUWI;
		}
		else if (upper == 92) {
			ArmFunc[i] = &x::STRBPUI;
		}
		else if (upper == 93) {
			ArmFunc[i] = &x::LDRBPUI;
		}
		else if (upper == 94) {
			ArmFunc[i] = &x::STRBPUWI;
		}
		else if (upper == 95) {
			ArmFunc[i] = &x::LDRBPUWI;
		}
		//Single Data Transfer Shifted
		else if ((upper == 96 || upper == 104) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STR;
		}
		else if ((upper == 97 || upper == 105) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDR;
		}
		else if ((upper == 98 || upper == 106) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRT;
		}
		else if ((upper == 99 || upper == 107) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRT;
		}
		else if ((upper == 100 || upper == 108) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRB;
		}
		else if ((upper == 101 || upper == 109) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRB;
		}
		else if ((upper == 102 || upper == 110) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRBT;
		}
		else if ((upper == 103 || upper == 111) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRBT;
		}
		else if ((upper == 112 || upper == 114) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRP;
		}
		else if ((upper == 113 || upper == 115) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRP;
		}
		else if ((upper == 116 || upper == 118) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRBP;
		}
		else if ((upper == 117 || upper == 119) && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRBP;
		}
		else if (upper == 120 && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRPU;
		}
		else if (upper == 121 && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRPU;
		}
		else if (upper == 122 && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRPUW;
		}
		else if (upper == 123 && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRPUW;
		}
		else if (upper == 124 && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRBPU;
		}
		else if (upper == 125 && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRBPU;
		}
		else if (upper == 126 && (lower % 2 == 0)) {
			ArmFunc[i] = &x::STRBPUW;
		}
		else if (upper == 127 && (lower % 2 == 0)) {
			ArmFunc[i] = &x::LDRBPUW;
		}
		else if (upper >= 128 && upper <= 159) {
			ArmFunc[i] = &x::BDT;
		}
		else if (upper >= 160 && upper <= 175) {
			ArmFunc[i] = &x::Br;
		}
		else if (upper >= 176 && upper <= 191) {
			ArmFunc[i] = &x::BrL;
		}
		else if (upper >= 240) {
			ArmFunc[i] = &x::SWI;
		}
	}
}


void Arm::UndefinedInstruction(MMU& mmu)
{
	Dummy();
}


void Arm::DataProc(uint32_t inst, uint32_t & Op1, uint32_t & Op2, bool & updateC, bool & carry,int Rn, int Rd, bool & S)
{
	////curInsnAsm = "DataProc";
	if ((Ainsn >> 25) & 1) {
		// Op2 Immediate
		Op2 = bitrange(7, 0, Ainsn);
		uint16_t shiftAmnt = bitrange(11, 8, Ainsn) * 2;
		if (Op2 > 0 && (Op2 >> (shiftAmnt - 1) & 1)) {
			carry = true;
		}
		Op2 = rotateRight(Op2, shiftAmnt);
		if (Rn == 30) {
			Op1 += 8;
		}
	}
	else {
		// Op2 Register
		int Rm = WhatRegister(bitrange(3, 0, Ainsn));
		Op2 = registers[Rm];
		bool inc = false;

		if ((Ainsn >> 4) & 1) {
			// Shift by register
			int Rs = WhatRegister(bitrange(11, 8, Ainsn));
			uint8_t shiftAmnt = bitrange(7, 0, registers[Rs]);
			if (Rm == 30) {
				Op2 += 12;
			}
			if (Rn == 30) {
				Op1 += 12;
			}
			switch (bitrange(6, 5, Ainsn))
			{
			case 0:
			{
				// LSL
				if (shiftAmnt == 0) {
					if (C) {
						carry = true;
					}
					else
					{
						carry = false;
					}
				}
				else if (shiftAmnt > 0) {
					uint64_t test = static_cast<uint64_t>(Op2);
					test = test << shiftAmnt;
					if ((test >> 32) & 1) {
						carry = true;
					}
					if (shiftAmnt < 32) {
						Op2 = Op2 << shiftAmnt;
					}
					else
					{
						Op2 = bitrange(31, 0, test);
					}
				}

				break;
			}
			case 1:
			{
				// LSR
				if (shiftAmnt == 0) {
					if (C) {
						carry = true;
					}
					else
					{
						carry = false;
					}
				}
				else if (shiftAmnt == 32) {
					if ((Op2 >> 31) & 1) {
						carry = true;
					}
					Op2 = 0;
				}
				else if (shiftAmnt > 32) {
					if ((Op2 >> (shiftAmnt - 1)) & 1) {
						carry = true;
					}
					carry = false;
					Op2 = 0;
				}
				else
				{
					uint32_t test = Op2;
					if ((Op2 >> (shiftAmnt - 1)) & 1) {
						carry = true;
					}
					Op2 = Op2 >> shiftAmnt;
				}
				break;
			}
			case 2:
			{
				// ASR
				if (shiftAmnt == 0) {
					if (C) {
						carry = true;
					}
					else
					{
						carry = false;
					}
				}
				else if (shiftAmnt >= 32) {
					if (Op2 >> 31 & 1) {
						Op2 = 0;
						Op2 = ~Op2;
						carry = true;
					}
					else
					{
						Op2 = 0;
					}
				}
				else
				{
					Dummy();
					uint64_t test = static_cast<uint64_t>(Op2);
					uint32_t ya = Op2 >> (shiftAmnt - 1);
					if ((Op2 >> (shiftAmnt - 1)) & 1) {
						carry = true;
					}
					Op2 = arithmeticRight(Op2, shiftAmnt);
				}
				break;
			}
			case 3:
				// ROR
				if (shiftAmnt == 0 && Op2 == 1) {
					Dummy();
				}
				if (shiftAmnt >= 32) {
					uint32_t ye = shiftAmnt - 32;
					uint32_t tst = Op2;
					if ((tst >> (shiftAmnt - 1) & 1)) {
						carry = true;
					}
					if (ye > 0) {
						Op2 = rotateRight(Op2, ye);
					}
				}
				else if (shiftAmnt == 0) {
					if (C) {
						carry = true;
					}
					else
					{
						carry = false;
					}
				}
				else
				{
					uint32_t tst = Op2;
					if ((tst >> (shiftAmnt - 1) & 1)) {
						carry = true;
					}
					Op2 = rotateRight(Op2, shiftAmnt);
				}
				break;
			}
		}
		else
		{
			// Shift by immediate
			uint8_t shiftAmnt = bitrange(11, 7, Ainsn);
			if (Rm == 30) {
				Op2 += 8;
			}
			if (Rn == 30) {
				Op1 += 8;
			}
			switch (bitrange(6, 5, Ainsn))
			{
			case 0:
			{
				// LSL
				if (shiftAmnt == 0) {
					carry = C;
				}


				uint64_t test = static_cast<uint64_t>(Op2);
				test = test << shiftAmnt;
				if ((test >> 32) & 1) {
					carry = true;
				}
				Op2 = Op2 << shiftAmnt;
				break;
			}
			case 1:
			{
				// LSR
				if (shiftAmnt == 0 || shiftAmnt == 32) {
					if ((Op2 >> 31) & 1)
					{
						carry = true;
					}
					Op2 = 0;
				}
				else if (shiftAmnt > 32) {
					if ((Op2 >> (shiftAmnt - 1)) & 1) {
						carry = true;
					}
					Op2 = 0;
				}
				else
				{
					uint32_t test = Op2;
					if ((Op2 >> (shiftAmnt - 1)) & 1) {
						carry = true;
					}
					Op2 = Op2 >> shiftAmnt;
				}

				break;
			}
			case 2:
			{
				// ASR
				if (shiftAmnt == 0 || shiftAmnt == 32) {
					Dummy();
					if ((Op2 >> 31) & 1)
					{
						Op2 = 0;
						carry = true;
						Op2 = ~Op2;
					}
					else
					{
						Op2 = 0;
					}
				}
				else if (shiftAmnt > 32) {
					if ((Op2 >> (shiftAmnt - 1)) & 1) {
						carry = true;
					}
					Op2 = 0;
				}
				else {
					uint32_t test = Op2;
					if ((Op2 >> (shiftAmnt - 1)) & 1) {
						carry = true;
					}
					Op2 = arithmeticRight(Op2, shiftAmnt);
				}
				break;
			}
			case 3:
				// ROR
				if (shiftAmnt == 0) {
					uint32_t tst = Op2;
					if ((CPSR >> 29) & 1) {
						shiftAmnt = 1;
						if ((tst >> 0) & 1) {
							carry = true;
						}
						Op2 = rotateRight(Op2, shiftAmnt);
						SetBit(Op2, 31);
					}
					else
					{
						shiftAmnt = 1;
						if ((tst >> 0) & 1) {
							carry = true;
						}
						Op2 = rotateRight(Op2, shiftAmnt);
						ClearBit(Op2, 31);
					}


				}
				else
				{
					uint32_t tst = Op2;
					if ((tst >> (shiftAmnt - 1) & 1)) {
						carry = true;
					}
					Op2 = rotateRight(Op2, shiftAmnt);
				}
				if (shiftAmnt == 32) {
					Dummy();
				}

				break;
			}
		}
	}
	if (Rd == 30) {
		//increment = false;
		//Dummy();
	}
	if (S && Rd == 30) {
		//increment = false;
		updateC = false;
		S = false;
		switch (mode)
		{
		case Arm::Mode::User:
			//
			break;
		case Arm::Mode::Fiq:
			CPSR = SPSR_fiq;
			break;
		case Arm::Mode::Irq:
			CPSR = SPSR_irq;
			break;
		case Arm::Mode::Supervisor:
			CPSR = SPSR_svc;
			break;
		case Arm::Mode::Abort:
			CPSR = SPSR_abt;
			break;
		case Arm::Mode::Undefined:
			CPSR = SPSR_und;
			break;
		case Arm::Mode::System:
			break;
		}
	}
}

void Arm::AND(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd,S);
	registers[Rd] = Op1 & Op2;
	if (S) {
		SetFlags(registers[Rd], carry, false, false);
	}
}

void Arm::EOR(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd,S);
	registers[Rd] = Op1 ^ Op2;
	if (S) {
		SetFlags(registers[Rd], carry, false, false);
	}
}

void Arm::SUB(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	//curInsnAsm = "SUB";
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd,S);
	uint32_t og = registers[Rn];
	if (updateC) {
		SetBit(CPSR, 29);
	}
	registers[Rd] = Op1 - Op2;
	//if (Rd == 30) {
	//	increment = false;
	//}
	if (S) {
		//SetBit(CPSR, 29);
		SetFlagsSub(registers[Rd], (Op2 > Op1), (Overflow(Op1, Op2, registers[Rd], false)), true);
	}
	//curInsnAsm = "reset";
}

void Arm::RSB(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd,S);
	uint32_t og = registers[Rn];
	//if (updateC) {
	//	SetBit(CPSR, 29);
	//}
	registers[Rd] = Op2 - Op1;
	if (S) {
		SetBit(CPSR, 29);
		SetFlagsSub(registers[Rd], (Op1 > Op2), (Overflow(Op2, Op1, registers[Rd], false)), true);
	}
}

void Arm::ADD(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	registers[Rd] = Op1 + Op2;
	if (S) {
		uint64_t tst = uint64_t(Op1) + uint64_t(Op2);
		bool poss = false;
		if (Op1 > 0 || Op2 > 0) {
			poss = true;
		}
		SetFlags(registers[Rd], (((Op1 + Op2) > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)), (Overflow(Op1, Op2, registers[Rd], true)), true);
	}
}

void Arm::ADC(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	uint64_t tst = uint64_t(Op1) + uint64_t(Op2);
	uint32_t og = registers[Rn];
	registers[Rd] = Op1 + Op2;
	if (C) {
		registers[Rd] += 1;
	}

	if (S) {
		bool poss = false;
		if (Op1 > 0 || Op2 > 0) {
			poss = true;
		}
		SetFlags(registers[Rd], ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0)), (Overflow(Op1, Op2, registers[Rd], true)), true);
	}

}

void Arm::SBC(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	uint32_t cv = 0;
	if (!C) {
		cv = 1;
	}
	SetBit(CPSR, 29);
	uint32_t og = Op2 - cv;
	registers[Rd] = Op1 - Op2 - cv;


	if (S) {
		SetFlagsSub(registers[Rd], ((Op2 > Op1) || ((Op2 == Op1) && cv)), (Overflow(Op1, Op2, registers[Rd], false)), true);
	}
}

void Arm::RSC(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	uint32_t cv = 0;
	if (!C) {
		cv = 1;
	}
	SetBit(CPSR, 29);
	uint32_t og = Op1 - cv;
	registers[Rd] = Op2 - Op1 - cv;

	if (S) {
		SetFlagsSub(registers[Rd], (og > Op2), (Overflow(Op2, og, registers[Rd], false)), true);
	}
}

void Arm::TST(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	if (S) {
		SetFlags((Op1 & Op2), carry, false, false);
	}
}

void Arm::TEQ(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	if (S) {
		SetFlags((Op1 ^ Op2), carry, false, false);
	}
}

void Arm::CMP(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));

	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	if (S) {
		uint32_t input = Op1;
		uint32_t result = Op1 - Op2;
		SetBit(CPSR, 29);
		SetFlagsSub(result, (Op2 > Op1), (Overflow(Op1, Op2, result, false)), true);
	}
}

void Arm::CMN(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	uint32_t input = Op1;
	uint32_t result = Op1 + Op2;
	if (S) {
		bool poss = false;
		if (Op1 > 0 || Op2 > 0) {
			poss = true;
		}
		uint64_t tst = uint64_t(Op1) + uint64_t(Op2);
		SetFlags(result, ((tst > 0xFFFFFFFF) || (poss && result == 0)), (Overflow(Op1, Op2, result, true)), true);
	}
}

void Arm::ORR(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	registers[Rd] = Op1 | Op2;
	if (S) {
		SetFlags(registers[Rd], carry, false, false);
	}
}

void Arm::MOV(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	//curInsnAsm = "MOV";
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	registers[Rd] = Op2;
	if (Rd == 30) {
		increment = false;
	}
	if (S) {
		SetFlags(registers[Rd], carry, false, false);
	}
	//curInsnAsm = "reset";
}

void Arm::BIC(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	registers[Rd] = Op1 & ~(Op2);
	if (S) {
		SetFlags(registers[Rd], carry, false, false);
	}
}
			  
void Arm::MVN(MMU& mmu)
{
	bool S = false;
	if ((Ainsn >> 20) & 1) {
		S = true;
	}
	int Rn = WhatRegister(bitrange(19, 16, Ainsn));
	int Rd = WhatRegister(bitrange(15, 12, Ainsn));
	uint32_t Op1 = registers[Rn];
	uint32_t Op2;
	bool updateC = true;
	bool carry = false;
	DataProc(Ainsn, Op1, Op2, updateC, carry, Rn, Rd, S);
	registers[Rd] = ~(Op2);
	if (S) {
		SetFlags(registers[Rd], carry, false, false);
	}
}

void Arm::MUL(MMU& mmu)
{
	//curInsnAsm = "MUL";
	uint32_t Rd = bitrange(19, 16, Ainsn);
	uint32_t Rn = bitrange(15, 12, Ainsn);
	uint32_t Rs = bitrange(11, 8,  Ainsn);
	uint32_t Rm = bitrange(3, 0,   Ainsn);
	registers[WhatRegister(Rd)] = registers[WhatRegister(Rm)] * registers[WhatRegister(Rs)];
	if (registers[Rd] == 0) {
		SetBit(CPSR, 30);
	}
	else
	{
		ClearBit(CPSR, 30);
	}
	if ((registers[Rd] >> 31) & 1) {
		SetBit(CPSR, 31);
	}
	else
	{
		ClearBit(CPSR, 31);
	}
}

void Arm::MLA(MMU& mmu)
{
	//curInsnAsm = "MLA";
	uint32_t Rd = bitrange(19, 16, Ainsn);
	uint32_t Rn = bitrange(15, 12, Ainsn);
	uint32_t Rs = bitrange(11, 8, Ainsn);
	uint32_t Rm = bitrange(3, 0, Ainsn);
	registers[WhatRegister(Rd)] = (registers[WhatRegister(Rm)] * registers[WhatRegister(Rs)]) + registers[WhatRegister(Rn)];
	if (registers[Rd] == 0) {
		SetBit(CPSR, 30);
	}
	else
	{
		ClearBit(CPSR, 30);
	}
	if ((registers[Rd] >> 31) & 1) {
		SetBit(CPSR, 31);
	}
	else
	{
		ClearBit(CPSR, 31);
	}
}

void Arm::UMULL(MMU& mmu)
{
	//curInsnAsm = "UMULL";
	uint32_t Rd = bitrange(19, 16, Ainsn);
	uint32_t Rn = bitrange(15, 12, Ainsn);
	uint32_t Rs = bitrange(11, 8, Ainsn);
	uint32_t Rm = bitrange(3, 0, Ainsn);
	uint64_t Op1 = static_cast<uint64_t>(registers[WhatRegister(Rs)]);
	uint64_t Op2 = static_cast<uint64_t>(registers[WhatRegister(Rm)]);
	uint64_t result = Op1 * Op2;
	registers[WhatRegister(Rn)] = (result << 32) >> 32;
	registers[WhatRegister(Rd)] = result >> 32;
	if (result == 0) {
		SetBit(CPSR, 30);
	}
	else
	{
		ClearBit(CPSR, 30);
	}
	if ((result >> 63) & 1) {
		SetBit(CPSR, 31);
	}
	else
	{
		ClearBit(CPSR, 31);
	}
}

void Arm::UMLAL(MMU& mmu)
{
	//curInsnAsm = "UMLAL";
	uint32_t Rd = bitrange(19, 16, Ainsn);
	uint32_t Rn = bitrange(15, 12, Ainsn);
	uint32_t Rs = bitrange(11, 8, Ainsn);
	uint32_t Rm = bitrange(3, 0, Ainsn);
	uint64_t hi_lo = registers[WhatRegister(Rd)];
	hi_lo <<= 16;
	hi_lo <<= 16;
	hi_lo |= registers[WhatRegister(Rn)];

	uint64_t accumulate = registers[WhatRegister(Rd)] << 32 | registers[WhatRegister(Rn)];
	uint64_t Op1 = static_cast<uint64_t>(registers[WhatRegister(Rs)]);
	uint64_t Op2 = static_cast<uint64_t>(registers[WhatRegister(Rm)]);
	uint64_t result = (Op1 * Op2) + hi_lo;
	registers[WhatRegister(Rn)] = ((result << 32) >> 32);
	registers[WhatRegister(Rd)] = (result >> 32);
	if (result == 0) {
		SetBit(CPSR, 30);
	}
	else
	{
		ClearBit(CPSR, 30);
	}
	if ((result >> 63) & 1) {
		SetBit(CPSR, 31);
	}
	else
	{
		ClearBit(CPSR, 31);
	}
}

void Arm::SMULL(MMU& mmu)
{
	//curInsnAsm = "SMULL";
	uint32_t Rd = bitrange(19, 16, Ainsn);
	uint32_t Rn = bitrange(15, 12, Ainsn);
	uint32_t Rs = bitrange(11, 8, Ainsn);
	uint32_t Rm = bitrange(3, 0, Ainsn);
	Dummy();
	if (registers[WhatRegister(Rs)] == -8) {
		Dummy();
	}
	if (registers[WhatRegister(Rm)] == -8) {
		Dummy();
	}
	int64_t Op1 = int32_t(registers[WhatRegister(Rs)]);
	int64_t Op2 = int32_t(registers[WhatRegister(Rm)]);

	int64_t result = Op1 * Op2;
	registers[WhatRegister(Rn)] = (result << 32) >> 32;
	registers[WhatRegister(Rd)] = result >> 32;
	if (result == 0) {
		SetBit(CPSR, 30);
	}
	else
	{
		ClearBit(CPSR, 30);
	}
	if ((result >> 63) & 1) {
		SetBit(CPSR, 31);
	}
	else
	{
		ClearBit(CPSR, 31);
	}
}

void Arm::SMLAL(MMU& mmu)
{
	//curInsnAsm = "SMLAL";
	uint32_t Rd = bitrange(19, 16, Ainsn);
	uint32_t Rn = bitrange(15, 12, Ainsn);
	uint32_t Rs = bitrange(11, 8, Ainsn);
	uint32_t Rm = bitrange(3, 0, Ainsn);
	if (registers[0] == 0x06E74E70 && registers[1] == 0x88DCF44E) {
		Dummy();
	}
	Dummy();
	if (registers[WhatRegister(Rs)] == -8) {
		Dummy();
	}
	if (registers[WhatRegister(Rm)] == -8) {
		Dummy();
	}
	int64_t hi_lo = registers[WhatRegister(Rd)];
	hi_lo <<= 16;
	hi_lo <<= 16;
	hi_lo |= registers[WhatRegister(Rn)];

	int64_t in1 = static_cast<int32_t>(registers[WhatRegister(Rs)]);
	int64_t in2 = static_cast<int32_t>(registers[WhatRegister(Rm)]);

	int64_t result = (in1 * in2) + hi_lo;
	registers[WhatRegister(Rn)] = (result << 32) >> 32;
	registers[WhatRegister(Rd)] = (result >> 32);
	if (result == 0) {
		SetBit(CPSR, 30);
	}
	else
	{
		ClearBit(CPSR, 30);
	}
	if ((result >> 63) & 1) {
		SetBit(CPSR, 31);
	}
	else
	{
		ClearBit(CPSR, 31);
	}
}

void Arm::HDSDT(MMU & mmu)
{
	//curInsnAsm = "HDSDT";
	uint32_t Rd = bitrange(15, 12, Ainsn);  // Source / destination register
	uint32_t Rn = bitrange(19, 16, Ainsn);  // Base register
	uint32_t address = registers[WhatRegister(Rn)];
	uint32_t offset = 0;
	bool P = false;
	bool U = false;
	bool I = false;
	bool L = false;
	bool WriteBack = true;

	if ((Ainsn >> 23) & 1) {
		U = true;
	}
	if ((Ainsn >> 22) & 1) {
		I = true;
		uint8_t upperOffset = bitrange(11, 8, Ainsn);
		uint8_t lowerOffset = bitrange(3, 0, Ainsn);
		offset = (upperOffset << 4) | lowerOffset;
	}
	else
	{
		uint32_t Rm = bitrange(3, 0, Ainsn);
		offset = registers[WhatRegister(Rm)];
	}
	if ((Ainsn >> 24) & 1) {
		P = true;
		if (U) {
			address += offset;
		}
		else
		{
			address -= offset;
		}


		if (!((Ainsn >> 21) & 1)) {
			WriteBack = false;
		}
	}

	if ((Ainsn >> 20) & 1) {
		switch (bitrange(6, 5, Ainsn))
		{
		case 0x0:
			// Reserved
			//curInsnAsm = "NOP";
			break;
		case 0x1:
		{
			// Load unsigned halfword
			if (Rd == Rn) {
				WriteBack = false;
			}
			uint32_t rotAmnt = 0;
			//rotAmnt = bitrange(1, 0, address);
			//rotAmnt *= 8;
			if (address % 2 != 0) {
				address -= 1;
			}

			if (address == 0x2000048) {
				Dummy();
			}

			uint32_t val = (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
			val = rotateRight(val, rotAmnt);
			if (address >= 0xD000000) {
				val = 1;
			}
			registers[WhatRegister(Rd)] = val;
			break;
		}
		case 0x2:
		{
			// Load signed byte
			if (Rd == Rn) {
				WriteBack = false;
			}
			uint8_t val = mmu.MemoryRead(address);
			if (val == 255) {
				Dummy();
			}
			if ((val >> 7) & 1) {
				registers[WhatRegister(Rd)] = (0xFFFFFF << 8) | val;
			}
			else
			{
				registers[WhatRegister(Rd)] = val;
			}
			break;
		}
		case 0x3:
		{
			// Load signed halfword
			if (Rd == Rn) {
				WriteBack = false;
			}
			uint32_t rotAmnt = 0;
			//rotAmnt = bitrange(1, 0, address);
			//rotAmnt *= 8;
			if (address % 4 != 0) {
				Dummy();
			}
			if (address % 2 != 0) {
				address -= 1;
			}

			uint32_t val = (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
			if ((val >> 15) & 1) {
				val = rotateRight(val, rotAmnt);
				if (rotAmnt == 8) {
					val = (0xFFFFFF << 8) | val;
				}
				else
				{
					val = (0xFFFF << 16) | val;
				}
				registers[WhatRegister(Rd)] = val;
			}
			else
			{
				registers[WhatRegister(Rd)] = val;
			}
			break;
		}
		}
	}
	else
	{
		switch (bitrange(6, 5, Ainsn))
		{
		case 0x0:
		{
			Dummy();
		}
		break;
		case 0x1:
			// Store halfword
			if (address % 4 != 0) {
				Dummy();
			}
			if (address % 2 != 0) {
				address -= 1;
			}
			if (address == 0x60001e2) {
				Dummy();
			}
			mmu.MemoryWrite(address, static_cast<uint16_t>(bitrange(15, 0, registers[WhatRegister(Rd)])));
			break;
		case 0x2:
			// Load doubleword
			if (address % 4 != 0) {
				Dummy();
			}
			if (Rd == Rn) {
				WriteBack = false;
			}
			registers[WhatRegister(Rd)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
			registers[WhatRegister(Rd) + 1] = (mmu.MemoryRead(address + 7) << 24) | (mmu.MemoryRead(address + 6) << 16) | (mmu.MemoryRead(address + 5) << 8) | mmu.MemoryRead(address + 4);
			break;
		case 0x3:
			// Store doubleword
			if (address % 4 != 0) {
				Dummy();
			}
			//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, registers[WhatRegister(Rd)])));
			//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, registers[WhatRegister(Rd)])));
			//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, registers[WhatRegister(Rd)])));
			//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, registers[WhatRegister(Rd)])));
			//mmu.MemoryWrite(address + 4, static_cast<uint8_t>(bitrange(7, 0, registers[WhatRegister(Rd) + 1])));
			//mmu.MemoryWrite(address + 5, static_cast<uint8_t>(bitrange(15, 8, registers[WhatRegister(Rd) + 1])));
			//mmu.MemoryWrite(address + 6, static_cast<uint8_t>(bitrange(23, 16, registers[WhatRegister(Rd) + 1])));
			//mmu.MemoryWrite(address + 7, static_cast<uint8_t>(bitrange(31, 24, registers[WhatRegister(Rd) + 1])));

			mmu.MemoryWrite(address, registers[WhatRegister(Rd)]);
			mmu.MemoryWrite(address + 4, registers[WhatRegister(Rd) + 1]);

			break;
		}
	}
	if (!P) {
		if (U) {
			address += offset;
		}
		else
		{
			address -= offset;
		}
	}
	if (WriteBack) {
		registers[WhatRegister(Rn)] = address;
	}
}

void Arm::SDT(MMU& mmu, bool shift, bool pre, bool up, bool byte, bool write, bool ldr)
{
	//curInsnAsm = "SDT";

	int Rd = bitrange(15, 12, Ainsn);  // Source / destination register
	int Rn = bitrange(19, 16, Ainsn);  // Base register
	int Rm = bitrange(3, 0, Ainsn);	  // Offset register
	if (Rd == 15) {
		Dummy();
	}
	uint32_t address = registers[WhatRegister(Rn)];
	uint32_t offsetAmnt = 0;
	bool I = false;
	bool P = false;
	bool U = false;
	bool B = false;
	bool WriteBack = true;

	if ((Ainsn >> 25) & 1) {
		I = true;
	}
	if ((Ainsn >> 24) & 1) {
		P = true;
		if (!((Ainsn >> 21) & 1)) {
			WriteBack = false;
		}
	}
	if ((Ainsn >> 23) & 1) {
		U = true;
	}
	if ((Ainsn >> 22) & 1) {
		B = true;
	}



	if (I) {
		// Shifted register
		if (Rd == 2) {
			Dummy();
		}
		uint8_t shiftAmnt = bitrange(11, 7, Ainsn);
		offsetAmnt = registers[WhatRegister(Rm)];
		switch (bitrange(6, 5, Ainsn))
		{
		case 0:
			offsetAmnt = offsetAmnt << shiftAmnt;
			break;
		case 1:
			offsetAmnt = offsetAmnt >> shiftAmnt;
			if (shiftAmnt == 0) {
				offsetAmnt = 0;
			}
			break;
		case 2:

			if (shiftAmnt == 0) {
				if (offsetAmnt >> 31 & 1) {
					offsetAmnt = 0;
					offsetAmnt = ~0;
				}
				else
				{
					offsetAmnt = 0;
				}
			}
			else
			{
				offsetAmnt = arithmeticRight(offsetAmnt, shiftAmnt);
			}
			break;
		case 3:
			Dummy();
			if (shiftAmnt == 0) {
				offsetAmnt = offsetAmnt >> 1;
				if ((CPSR >> 29) & 1) {
					SetBit(offsetAmnt, 31);
				}
				else
				{
					ClearBit(offsetAmnt, 31);
				}
			}
			offsetAmnt = rotateRight(offsetAmnt, shiftAmnt);
			break;
		}
	}
	else
	{
		offsetAmnt = bitrange(11, 0, Ainsn);
	}

	if (P) {
		if (U) {
			address += offsetAmnt;
		}
		else
		{
			address -= offsetAmnt;
		}
	}

	if (address == 67109384) {
		Dummy();
	}

	if ((Ainsn >> 20) & 1) {
		// LDR
		uint32_t rotAmnt = 0;
		if (address % 4 != 0) {
			rotAmnt = bitrange(1, 0, address);
			rotAmnt *= 8;
		}
		if (Rd == Rn) {
			WriteBack = false;
		}
		if (Rn == 15) {
			address += 8;
		}
		if (B) {
			registers[WhatRegister(Rd)] = mmu.MemoryRead(address);
			//if (Rd == 15) {
			//	registers[WhatRegister(Rd)] += 4;
			//}
			////curInsnAsm = "LDR{B}{T}";
		}
		else
		{
			if (registers[30] == 0x3000178) {
				Dummy();
			}

			uint32_t val1 = mmu.MemoryRead(address);
			uint32_t val2 = mmu.MemoryRead(address + 1);
			uint32_t val3 = mmu.MemoryRead(address + 2);
			uint32_t val4 = mmu.MemoryRead(address + 3);



			AlignAddr(address);
			uint32_t val = 0;
			val = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
			//uint32_t val = mmu.MemoryReadWord(address);
			registers[WhatRegister(Rd)] = rotateRight(val, rotAmnt);
			if (Rd == 15) {
				increment = false;
			}
			////curInsnAsm = "LDR{W}{T}";
		}

	}
	else
	{
		// STR
		uint32_t val = registers[WhatRegister(Rd)];
		if (Rd == 15) {
			val += 12;
		}
		if (B) {
			mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
			////curInsnAsm = "STR{B}{T}";
		}
		else
		{
			AlignAddr(address);
			//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
			//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
			//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
			//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
			mmu.MemoryWrite(address, val);
			////curInsnAsm = "STR{W}{T}";
		}


	}
	if (!P) {
		if (U) {
			address += offsetAmnt;
		}
		else
		{
			address -= offsetAmnt;
		}
	}
	if (WriteBack) {
		registers[WhatRegister(Rn)] = address;
	}



	//if (loaded) {
	//	int rd = bitrange(15, 12, Ainsn);  // source / destination register
	//	int rn = bitrange(19, 16, Ainsn);  // base register
	//	int rm = 0;	  // offset register
	//	uint32_t address = registers[WhatRegister(rn)];
	//	uint32_t offsetamnt = 0;

	//	if (!pre) {
	//		write = true;
	//	}

	//	if (shift) {
	//		// shifted register
	//		rm = bitrange(3, 0, Ainsn);
	//		uint8_t shiftamnt = bitrange(11, 7, Ainsn);
	//		offsetamnt = registers[WhatRegister(rm)];
	//		switch (bitrange(6, 5, Ainsn))
	//		{
	//		case 0:
	//			offsetamnt = offsetamnt << shiftamnt;
	//			break;
	//		case 1:
	//			offsetamnt = offsetamnt >> shiftamnt;
	//			if (shiftamnt == 0) {
	//				offsetamnt = 0;
	//			}
	//			break;
	//		case 2:

	//			if (shiftamnt == 0) {
	//				if (offsetamnt >> 31 & 1) {
	//					offsetamnt = 0;
	//					offsetamnt = ~0;
	//				}
	//				else
	//				{
	//					offsetamnt = 0;
	//				}
	//			}
	//			else
	//			{
	//				offsetamnt = arithmeticRight(offsetamnt, shiftamnt);
	//			}
	//			break;
	//		case 3:
	//			if (shiftamnt == 0) {
	//				offsetamnt = offsetamnt >> 1;
	//				if ((CPSR >> 29) & 1) {
	//					SetBit(offsetamnt, 31);
	//				}
	//				else
	//				{
	//					ClearBit(offsetamnt, 31);
	//				}
	//			}
	//			offsetamnt = rotateRight(offsetamnt, shiftamnt);
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		offsetamnt = bitrange(11, 0, Ainsn);
	//	}

	//	if (pre) {
	//		if (up) {
	//			address += offsetamnt;
	//		}
	//		else
	//		{
	//			address -= offsetamnt;
	//		}
	//	}

	//	if (ldr) {
	//		// ldr
	//		uint32_t rotamnt = 0;
	//		if (address % 4 != 0) {
	//			rotamnt = bitrange(1, 0, address);
	//			rotamnt *= 8;
	//		}
	//		if (rd == rn) {
	//			write = false;
	//		}
	//		if (rn == 15) {
	//			address += 8;
	//		}
	//		if (byte) {
	//			registers[WhatRegister(rd)] = mmu.MemoryRead(address);
	//		}
	//		else
	//		{
	//			uint32_t val1 = mmu.MemoryRead(address);
	//			uint32_t val2 = mmu.MemoryRead(address + 1);
	//			uint32_t val3 = mmu.MemoryRead(address + 2);
	//			uint32_t val4 = mmu.MemoryRead(address + 3);

	//			AlignAddr(address);
	//			uint32_t val = 0;
	//			val = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
	//			registers[WhatRegister(rd)] = rotateRight(val, rotamnt);
	//			if (rd == 15) {
	//				increment = false;
	//			}
	//		}

	//	}
	//	else
	//	{
	//		// str
	//		uint32_t val = registers[WhatRegister(rd)];
	//		if (rd == 15) {
	//			val += 12;
	//		}
	//		if (byte) {
	//			mmu.MemoryWrite(address, static_cast<uint8_t>(registers[WhatRegister(rd)]));
	//			//curInsnAsm = "str{b}{t}";
	//		}
	//		else
	//		{
	//			AlignAddr(address);
	//			mmu.MemoryWrite(address, val);
	//			//curInsnAsm = "str{w}{t}";
	//		}


	//	}
	//	if (!pre) {
	//		if (up) {
	//			address += offsetamnt;
	//		}
	//		else
	//		{
	//			address -= offsetamnt;
	//		}
	//	}
	//	if (write) {
	//		registers[WhatRegister(rn)] = address;
	//	}
	//}
}

void Arm::STRI(MMU& mmu)
{
	SDT(mmu, false, false, false, false, false, false);
}

void Arm::LDRI(MMU& mmu)
{
	SDT(mmu, false, false, false, false, false, true);
}

void Arm::STRTI(MMU & mmu)
{
	SDT(mmu, false, false, false, false, false, false);
}

void Arm::LDRTI(MMU & mmu)
{
	SDT(mmu, false, false, false, false, false, true);
}

void Arm::STRBI(MMU & mmu)
{
	SDT(mmu, false, false, false, true, false, false);
}

void Arm::LDRBI(MMU & mmu)
{
	SDT(mmu, false, false, false, true, false, true);
}

void Arm::STRBTI(MMU & mmu)
{
	SDT(mmu, false, false, false, true, false, false);
}

void Arm::LDRBTI(MMU & mmu)
{
	SDT(mmu, false, false, false, true, false, true);
}

void Arm::STRPI(MMU & mmu)
{
	SDT(mmu, false, true, false, false, false, false);
}

void Arm::LDRPI(MMU & mmu)
{
	SDT(mmu, false, true, false, false, false, true);
}

void Arm::STRBPI(MMU & mmu)
{
	SDT(mmu, false, true, false, true, false, false);
}

void Arm::LDRBPI(MMU & mmu)
{
	SDT(mmu, false, true, false, true, false, true);
}

void Arm::STRPUI(MMU & mmu)
{
	SDT(mmu, false, true, true, false, false, false);
}

void Arm::LDRPUI(MMU & mmu)
{
	SDT(mmu, false, true, true, false, false, true);
}

void Arm::STRPUWI(MMU & mmu)
{
	SDT(mmu, false, true, true, false, true, false);
}

void Arm::LDRPUWI(MMU & mmu)
{
	SDT(mmu, false, true, true, false, true, true);
}

void Arm::STRBPUI(MMU & mmu)
{
	SDT(mmu, false, true, true, true, false, false);
}

void Arm::LDRBPUI(MMU & mmu)
{
	SDT(mmu, false, true, true, true, false, true);
}

void Arm::STRBPUWI(MMU & mmu)
{
	SDT(mmu, false, true, true, true, true, false);
}

void Arm::LDRBPUWI(MMU & mmu)
{
	SDT(mmu, false, true, true, true, true, true);
}

void Arm::STR(MMU & mmu)
{
	SDT(mmu, true, false, false, false, false, false);
}

void Arm::LDR(MMU & mmu)
{
	SDT(mmu, true, false, false, false, false, true);
}

void Arm::STRT(MMU & mmu)
{
	SDT(mmu, true, false, false, false, false, false);
}

void Arm::LDRT(MMU & mmu)
{
	SDT(mmu, true, false, false, false, false, true);
}

void Arm::STRB(MMU & mmu)
{
	SDT(mmu, true, false, false, true, false, false);
}

void Arm::LDRB(MMU & mmu)
{
	SDT(mmu, true, false, false, true, false, true);
}

void Arm::STRBT(MMU & mmu)
{
	SDT(mmu, true, false, false, true, false, false);
}

void Arm::LDRBT(MMU & mmu)
{
	SDT(mmu, true, false, false, true, false, true);
}

void Arm::STRP(MMU & mmu)
{
	SDT(mmu, true, true, false, false, false, false);
}

void Arm::LDRP(MMU & mmu)
{
	SDT(mmu, true, true, false, false, false, true);
}

void Arm::STRBP(MMU & mmu)
{
	SDT(mmu, true, true, false, true, false, false);
}

void Arm::LDRBP(MMU & mmu)
{
	SDT(mmu, true, true, false, true, false, true);
}

void Arm::STRPU(MMU & mmu)
{
	SDT(mmu, true, true, true, false, false, false);
}

void Arm::LDRPU(MMU & mmu)
{
	SDT(mmu, true, true, true, false, false, true);
}

void Arm::STRPUW(MMU & mmu)
{
	SDT(mmu, true, true, true, false, true, false);
}

void Arm::LDRPUW(MMU & mmu)
{
	SDT(mmu, true, true, true, false, true, true);
}

void Arm::STRBPU(MMU & mmu)
{
	SDT(mmu, true, true, true, true, false, false);
}

void Arm::LDRBPU(MMU & mmu)
{
	SDT(mmu, true, true, true, true, false, true);
}

void Arm::STRBPUW(MMU & mmu)
{
	SDT(mmu, true, true, true, true, true, false);
}

void Arm::LDRBPUW(MMU & mmu)
{
	SDT(mmu, true, true, true, true, true, true);
}

void Arm::BDT(MMU & mmu)
{
	if (Ainsn == 0x812C5BD) {
		increment = false;
	}
	//curInsnAsm = "BDT";
	int Rn = bitrange(19, 16, Ainsn);
	uint32_t address = registers[WhatRegister(Rn)];
	uint32_t oldAddr = address;
	int Rlist = 0;
	bool P = false;
	bool U = false;
	bool S = false;
	bool W = false;
	bool L = false;
	bool stm = false;
	if ((Ainsn >> 24) & 1) {
		P = true;
	}
	if ((Ainsn >> 23) & 1) {
		U = true;
	}
	if ((Ainsn >> 21) & 1) {
		W = true;
	}
	if ((Ainsn >> 20) & 1) {
		L = true;
	}
	bool found = false;
	bool match = false;
	int matchNum = -1;
	int first = -1;
	int last = -1;
	bool pc = false;
	for (int i = 0; i < 16; i++) {
		if ((Ainsn >> i) & 1) {
			Rlist += 1;
			if (!found) {
				first = i;
				found = true;
			}
			last = i;
			if (i == Rn) {
				match = true;
				matchNum = i;
			}
			if (i == 15) {
				pc = true;
			}
			if (i == 14) {
				Dummy();
			}
		}
	}
	if (L && pc) {
		increment = false;
	}
	if ((Ainsn >> 22) & 1) {
		S = true;
		if (L && pc) {
			Dummy();
			switch (mode)

			{
			case Arm::Mode::User:
				break;
			case Arm::Mode::Fiq:
				CPSR = SPSR_fiq;
				break;
			case Arm::Mode::Irq:
				CPSR = SPSR_irq;
				break;
			case Arm::Mode::Supervisor:
				CPSR = SPSR_svc;
				break;
			case Arm::Mode::Abort:
				CPSR = SPSR_abt;
				break;
			case Arm::Mode::Undefined:
				CPSR = SPSR_und;
				break;
			case Arm::Mode::System:
				break;
			}
		}
		else
		{
			mode = Mode::User;
		}

	}
	bool empty = false;

	if (Rlist == 0) {
		empty = true;
		W = false;
		if (U) {
			if (P) {
				if (L) {
					//LDMIB
					registers[30] = mmu.MemoryReadWord(address);
					increment = false;
					registers[WhatRegister(Rn)] = address + 0x40;
				}
				else
				{
					//STMIB
					mmu.MemoryWrite(address + 4, registers[30] + 12);
					registers[WhatRegister(Rn)] = address + 0x40;
				}
			}
			else
			{
				if (L) {
					//LDMIA
					registers[30] = mmu.MemoryReadWord(address);
					increment = false;
					registers[WhatRegister(Rn)] = address + 0x40;
				}
				else
				{
					//STMIA
					mmu.MemoryWrite(address, registers[30] + 12);
					registers[WhatRegister(Rn)] = address + 0x40;
				}
			}
		}
		else
		{
			if (P) {
				if (L) {
					//LDMDB
					registers[30] = mmu.MemoryReadWord(address);
					increment = false;
					registers[WhatRegister(Rn)] = address - 0x40;
				}
				else
				{
					//STMDB
					mmu.MemoryWrite(address - 0x40, registers[30] + 12);
					registers[WhatRegister(Rn)] = address - 0x40;
				}
			}
			else
			{
				if (L) {
					//LDMDA
					registers[30] = mmu.MemoryReadWord(address);
					increment = false;
					registers[WhatRegister(Rn)] = address - 0x40;
				}
				else
				{
					//STMDA
					mmu.MemoryWrite(address - 0x3C, registers[30] + 12);
					registers[WhatRegister(Rn)] = address - 0x40;
				}
			}
		}
	}
	int index = -1;
	bool ran = false;
	uint32_t finishedAddr = 0;

	for (int i = 0; i < 16; i++) {
		if (((Ainsn >> i) & 1)) {
			if (empty) {
				i = 15;
				empty = false;
			}
			uint32_t val = registers[WhatRegister(i)];
			if (i == 15) {
				val += 12;
			}

			index += 1;
			if (P) {
				if (U) {
					if (!ran) {
						finishedAddr = address + (Rlist * 4);
						address = address + 4;
						ran = true;
					}
					else
					{
						address += 4;
					}

					if (L) {
						registers[WhatRegister(i)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
						//curInsnAsm = "LDM IB";
					}
					else
					{
						if (i == matchNum && matchNum != first) {
							val = finishedAddr;
						}
						//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
						//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
						//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
						//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
						mmu.MemoryWrite(address, val);
						//curInsnAsm = "STM IB";
						stm = true;
					}
				}
				else
				{
					if (!ran) {
						finishedAddr = address - ((Rlist) * 4);
						address = finishedAddr;
						ran = true;
					}
					else
					{
						address += 4;
					}
					if (L) {
						registers[WhatRegister(i)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
						//curInsnAsm = "LDM DB";
					}
					else
					{
						if (i == matchNum && matchNum != first) {
							val = finishedAddr;
						}
						//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
						//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
						//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
						//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
						mmu.MemoryWrite(address, val);
						//curInsnAsm = "STM DB";
						stm = true;
					}
				}
			}
			else
			{
				if (U)
				{
					if (!ran) {
						finishedAddr = address + (Rlist * 4);
						address = address;
						ran = true;
					}
					else
					{
						address += 4;
					}
					if (L) {
						registers[WhatRegister(i)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
						//curInsnAsm = "LDM IA";
					}
					else
					{
						if (i == matchNum && matchNum != first) {
							val = finishedAddr;
						}
						//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
						//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
						//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
						//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
						mmu.MemoryWrite(address, val);
						//curInsnAsm = "STM IA";
						stm = true;
					}
				}
				else
				{
					if (!ran) {
						finishedAddr = (address - ((Rlist) * 4));
						address = finishedAddr + 4;


						ran = true;
					}
					else
					{
						address += 4;
					}
					if (L) {
						//uint32_t val = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
						//if (i == 15) {
						//	val = val & 0xFFFFFFFC;
						//}
						registers[WhatRegister(i)] = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
						//curInsnAsm = "LDM DA";
						if (registers[30] % 2 != 0) {
							std::cerr << "\nInvalid PC\nRegister 30: " << registers[30] << std::endl;
						}
					}
					else
					{
						if (i == matchNum && matchNum != first) {
							val = finishedAddr;
						}
						//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, val)));
						//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, val)));
						//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, val)));
						//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, val)));
						mmu.MemoryWrite(address, val);
						//curInsnAsm = "STM DA";
						stm = true;
					}
				}
			}
		}
	}
	if (match) {
		if (Rn == first) {
			if (!stm) {
				W = false;
			}
		}
		else if (Rn == last) {
			if (!stm) {
				W = false;
			}
		}
		else
		{
			if (!stm) {
				W = false;
			}
		}
	}

	if (W) {
		registers[WhatRegister(Rn)] = finishedAddr;
	}
}

void Arm::SWP(MMU & mmu)
{
	//curInsnAsm = "SWP";
	int Rm = bitrange(3, 0, Ainsn);
	int Rn = bitrange(19, 16, Ainsn);
	int Rd = bitrange(15, 12, Ainsn);

	uint32_t address = registers[WhatRegister(Rn)];

	uint32_t rotAmnt = 0;
	rotAmnt = bitrange(1, 0, address);
	rotAmnt *= 8;
	AlignAddr(address);
	uint32_t val = (mmu.MemoryRead(address + 3) << 24) | (mmu.MemoryRead(address + 2) << 16) | (mmu.MemoryRead(address + 1) << 8) | mmu.MemoryRead(address);
	val = rotateRight(val, rotAmnt);
	if (Rd == Rm) {
		uint32_t temp = registers[WhatRegister(Rd)];
		registers[WhatRegister(Rd)] = val;
		//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, temp)));
		//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, temp)));
		//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, temp)));
		//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, temp)));
		mmu.MemoryWrite(address, temp);
	}
	else
	{
		//mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, registers[WhatRegister(Rm)])));
		//mmu.MemoryWrite(address + 1, static_cast<uint8_t>(bitrange(15, 8, registers[WhatRegister(Rm)])));
		//mmu.MemoryWrite(address + 2, static_cast<uint8_t>(bitrange(23, 16, registers[WhatRegister(Rm)])));
		//mmu.MemoryWrite(address + 3, static_cast<uint8_t>(bitrange(31, 24, registers[WhatRegister(Rm)])));
		mmu.MemoryWrite(address, registers[WhatRegister(Rm)]);
		registers[WhatRegister(Rd)] = val;
	}
	
}

void Arm::SWPB(MMU & mmu)
{
	//curInsnAsm = "SWPB";
	int Rm = bitrange(3, 0, Ainsn);		  //Source address
	int Rn = bitrange(19, 16, Ainsn);	  //Base address
	int Rd = bitrange(15, 12, Ainsn);	  //Register

	uint32_t address = registers[WhatRegister(Rn)];

	if (Rd == Rm) {
		uint8_t temp = bitrange(7,0, registers[WhatRegister(Rd)]);
		registers[WhatRegister(Rd)] = mmu.MemoryRead(address);
		mmu.MemoryWrite(address, temp);
	}
	else
	{
		uint8_t val = mmu.MemoryRead(address);
		mmu.MemoryWrite(address, static_cast<uint8_t>(bitrange(7, 0, registers[WhatRegister(Rm)])));
		registers[WhatRegister(Rd)] = val;
	}
}

void Arm::Br(MMU & mmu)
{
	//curInsnAsm = "B";
	int32_t offset = bitrange(23, 0, Ainsn);

	offset <<= 2;
	if (offset & 0x2000000) {
		offset |= 0xFC000000;
	}

	registers[30] = registers[30] + offset + 8;
	increment = false;
}

void Arm::BrL(MMU & mmu)
{
	//curInsnAsm = "BL";
	int32_t offset = bitrange(23, 0, Ainsn);
	offset <<= 2;
	if (offset & 0x2000000) {
		offset |= 0xFC000000;
	}

	registers[WhatRegister(14)] = registers[30] + 4;
	registers[30] = registers[30] + offset + 8;
	increment = false;
}

void Arm::BrX(MMU & mmu)
{
	//curInsnAsm = "BX";
	int x = bitrange(3, 0, Ainsn);
	uint32_t Rn = registers[WhatRegister(x)];
	if (x == 15) {
		Rn += 4;
	}
	if (Rn % 2 != 0) {
		Rn -= 1;
		SetBit(CPSR, 5);
	}
	else
	{
		ClearBit(CPSR, 5);
	}
	
	registers[30] = Rn;
	increment = false;
}

void Arm::SWI(MMU & mmu)
{
	//curInsnAsm = "SWI";
	//SWI
	registers[25] = registers[30] + 4;
	SPSR_svc = CPSR;
	uint32_t mask = 0x000000FF;
	CPSR &= ~mask;
	CPSR |= 0b10011;
	registers[30] = 0x8;
	increment = false;
}
