#include "Arm.h"
#include <fstream>
#include <bitset>



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

void Arm::Load(MMU& mmu)
{
	std::ifstream rom;
	std::streampos size;
	char* buffer;

	rom.open("C:\\Users\\brend\\Desktop\\roms\\fuzz_arm.gba", std::ios::ate | std::ios::binary);
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
	mmu.EEPROM.resize(eep, 0);

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

	if (registers[30] == 0x80989A0) {
		Dummy();
	}
	if (registers[4] == 100724736) {
		Dummy();
	}
	if (registers[4] == 0x600F0C2) {
		Dummy();
	}
	if (registers[30] == 0x8001260) {
		Dummy();
	}
	if (registers[30] == 0x800128A) {
		Dummy();
	}

	//if (frame % 4 == 0) {
		if (state == State::Arm) {

			//Ainsn = FetchArm(mmu);

			Decode(FetchArm(mmu), ppu, mmu);

			if (increment) {
				registers[30] += 4;
			}
		}
		else
		{

			//Tinsn = FetchThumb(mmu);

			DecodeThumb(FetchThumb(mmu), mmu);

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
	//}



	DMATransfer(mmu, ppu);
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
	return mmu.MemoryReadWord(registers[30]);
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

		if (dma_3_addr_dest == 0xD000000) {
			Dummy();
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


		destCnt = bitrange(6, 5, mmu.DMA3CNT_H);
		srcCnt = bitrange(8, 7, mmu.DMA3CNT_H);
		if (mmu.DMA3CNT_H >> 11 & 1) {
			drq = true;
		}
		if (bStart) {
			for (uint32_t i = 0; i < wordCount; i++) {
				if (mmu.DMA3DAD == 0x600F0C0 || mmu.DMA3DAD == 0x600F0C2) {
					Dummy();
				}
				if (dma_3_addr_dest == 0x600F0C0 || dma_3_addr_dest == 0x600F0C2) {
					Dummy();
				}
				// Transfer
				if (word) {

					uint32_t val = mmu.MemoryReadWord(dma_3_addr_src);
					mmu.MemoryWrite(dma_3_addr_dest, val);
				}
				else
				{
					uint16_t val = mmu.MemoryRead(dma_3_addr_src + 1) << 8 | mmu.MemoryRead(dma_3_addr_src);
					mmu.MemoryWrite(dma_3_addr_dest, val);
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
			registers[Rd] = arithmeticRight(registers[Rs], offset);
			if (offset == 0 || offset == 32) {
				registers[Rd] = 0;
				registers[Rd] = ~0;
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
			registers[Rd] = imm;
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
			uint64_t tst = uint64_t(registers[WhatRegister(Rd)]) + uint64_t(registers[WhatRegister(Rs)]);
			bool poss = false;
			if (registers[WhatRegister(Rd)] > 0 || registers[WhatRegister(Rs)] > 0) {
				poss = true;
			}
			registers[WhatRegister(Rd)] = Op1 + Op2;
			//curInsnAsm = "ADD";
			//if (registers[WhatRegister(Rd)] >> 31 & 1) {
			//	SetBit(CPSR, 31);
			//}
			//else
			//{
			//	ClearBit(CPSR, 31);
			//}
			//if (registers[WhatRegister(Rd)] == 0) {
			//	SetBit(CPSR, 30);
			//}
			//else
			//{
			//	ClearBit(CPSR, 30);
			//}
			//if ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)) {
			//	SetBit(CPSR, 29);
			//}
			//else
			//{
			//	ClearBit(CPSR, 29);
			//}
			//if (Overflow(registers[WhatRegister(Rd)], registers[WhatRegister(Rs)], registers[WhatRegister(Rd)], true)) {
			//	SetBit(CPSR, 28);
			//}
			//else
			//{
			//	ClearBit(CPSR, 28);
			//}
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
			registers[WhatRegister(Rd)] = Op2;
			if (Rd == 15) {
				increment = false;
			}
			//curInsnAsm = "MOV";
			//if (registers[WhatRegister(Rd)] >> 31 & 1) {
			//	SetBit(CPSR, 31);
			//}
			//else
			//{
			//	ClearBit(CPSR, 31);
			//}
			//if (registers[WhatRegister(Rd)] == 0) {
			//	SetBit(CPSR, 30);
			//}
			//else
			//{
			//	ClearBit(CPSR, 30);
			//}
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
			if (registers[WhatRegister(Rs)] < 0x8000000) {
				Dummy();
			}
			if (registers[WhatRegister(Rs)] < 0x3003CBA) {
				Dummy();
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
				if (registers[30] == 0) {
					Dummy();
				}
				increment = false;
				//curInsnAsm = "BX";
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
				//curInsnAsm = "LDRH";
				registers[Rd] = mmu.MemoryRead(address + 1) << 8 | mmu.MemoryRead(address);
				Dummy();
				break;
			case 0b11:
				// LDSH 
				//curInsnAsm = "LDSH";
				registers[Rd] = mmu.MemoryRead(address + 1) << 8 | mmu.MemoryRead(address);
				if (registers[Rd] >> 15 & 1) {
					registers[Rd] |= 0xFFFF0000;
				}
				Dummy();
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

		if (insn >> 11 & 1) {
			// LDR
			//curInsnAsm = "LDRH";
			registers[Rd] = mmu.MemoryRead(address + 1) << 8 | mmu.MemoryRead(address);
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
			registers[Rd] = mmu.MemoryReadWord(address);
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
	if (bitrange(15, 8, insn) == 0b10110000) {
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
		for (int i = 0; i < 8; i++) {
			if (insn >> i & 1) {
				if (insn >> 11 & 1) {
					//LDMIA
					registers[i] = mmu.MemoryReadWord(address);
					address += 4;
					//curInsnAsm = "LDMIA";
				}
				else
				{
					//STMIA
					mmu.MemoryWrite(address, registers[i]);
					address += 4;
					//curInsnAsm = "STMIA";
				}
			}
		}
		registers[Rb] = address;
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

void Arm::Decode(uint32_t insn, PPU& ppu, MMU& mmu)
{
	Dummy();

	if (registers[0] == 0x1B8C959B && registers[1] == 0xD9B35E27) {
		Dummy();
	}
	if (registers[30] == 134419556) {
		Dummy();
	}
	if (frame == 3310716) {
		Dummy();
	}

	if (Condition(insn)) {
		bool S;
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
			Dummy();
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
				if (Rd == 3) {
					Dummy();
				}

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
							else if (shiftAmnt > 32) {
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
							if (carry) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
							if ((Op1 & Op2) == 0) {
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
						//curInsnAsm = "AND";
						break;
					case 0x1:
						// EOR
						registers[Rd] = Op1 ^ Op2;
						if (S) {
							Dummy();
							if (carry) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
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
						//if (Rd == 30) {
						//	if (CPSR >> 5 & 1) {
						//		//registers[Rd] += 4;
						//	}
						//	increment = false;
						//}

						if (S) {
							if (Overflow(Op1, Op2, registers[Rd], false)) {
								SetBit(CPSR, 28);
							}
							else
							{
								ClearBit(CPSR, 28);
							}
							if (Op2 > Op1) {
								ClearBit(CPSR, 29);
							}
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
						//curInsnAsm = "SUB";
						break;
					}
					case 0x3:
					{
						// RSB
						uint32_t og = registers[Rn];
						if (registers[1] == 0x05274924 && registers[2] == 0x21) {
							Dummy();
						}
						SetBit(CPSR, 29);
						registers[Rd] = Op2 - Op1;
						if (S) {
							if (Overflow(Op2, Op1, registers[Rd], false)) {
								SetBit(CPSR, 28);
							}
							else
							{
								ClearBit(CPSR, 28);
							}
							if (Op1 > Op2) {
								ClearBit(CPSR, 29);
							}
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
						//curInsnAsm = "RSB";
						break;
					}
					case 0x4:
					{
						// ADD
						if (Op2 == 4) {
							Dummy();
						}
						if (Op1 == 0x7CC340 && Op2 == 0xFFFFFCD3) {
							Dummy();
						}
						registers[Rd] = Op1 + Op2;

						if (S) {
							if (Overflow(Op1, Op2, registers[Rd], true)) {
								SetBit(CPSR, 28);
							}
							else
							{
								ClearBit(CPSR, 28);
							}
							bool poss = false;
							if (Op1 > 0 || Op2 > 0) {
								poss = true;
							}
							uint64_t tst = uint64_t(Op1) + uint64_t(Op2);
							if (((Op1 + Op2) > 0xFFFFFFFF) || (poss && registers[Rd] == 0) || (tst >> 32 & 1)) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
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
							if (Overflow(Op1, Op2, registers[Rd], true)) {
								SetBit(CPSR, 28);
							}
							else
							{
								ClearBit(CPSR, 28);
							}
							bool poss = false;
							if (Op1 > 0 || Op2 > 0) {
								poss = true;
							}
							if ((tst > 0xFFFFFFFF) || (poss && registers[Rd] == 0)) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
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
							if (Overflow(Op1, og, registers[Rd], false)) {
								SetBit(CPSR, 28);
							}
							else
							{
								ClearBit(CPSR, 28);
							}
							if (Op2 > Op1) {
								ClearBit(CPSR, 29);
							}
							if (Op2 == Op1) {
								if (cv) {
									ClearBit(CPSR, 29);
								}
							}
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
							if (Overflow(Op2, og, registers[Rd], false)) {
								SetBit(CPSR, 28);
							}
							else
							{
								ClearBit(CPSR, 28);
							}
							if (og > Op2) {
								ClearBit(CPSR, 29);
							}
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

						//curInsnAsm = "RSC";
						break;
					}
					case 0x8:
						// TST
						Dummy();
						if (S) {
							if (carry) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
							if ((Op1 & Op2) == 0) {
								SetBit(CPSR, 30);
							}
							else
							{
								ClearBit(CPSR, 30);
							}
							if (((Op1 & Op2) >> 31) & 1) {
								SetBit(CPSR, 31);
							}
							else
							{
								ClearBit(CPSR, 31);
							}
							//curInsnAsm = "TST";
						}
						break;
					case 0x9:
						// TEQ
						if (registers[0] == 0x10679948 && registers[1] == 0xACD0ABD1) {
							Dummy();
						}
						if (registers[3] == 0xACD0ABD1 && registers[4] == 0x10679948) {
							Dummy();
						}
						Dummy();
						if (S) {
							if (carry) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
							if ((Op1 ^ Op2) == 0) {
								SetBit(CPSR, 30);
							}
							else
							{
								ClearBit(CPSR, 30);
							}
							if (((Op1 ^ Op2) >> 31) & 1) {
								SetBit(CPSR, 31);
							}
							else
							{
								ClearBit(CPSR, 31);
							}
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
							if (Overflow(Op1, Op2, result, false)) {
								SetBit(CPSR, 28);
							}
							else
							{
								ClearBit(CPSR, 28);
							}
							if (Op2 > input) {
								ClearBit(CPSR, 29);
							}
							if ((registers[Rn] - Op2) == 0) {
								SetBit(CPSR, 30);
							}
							else
							{
								ClearBit(CPSR, 30);
							}
							if (((registers[Rn] - Op2) >> 31) & 1) {
								SetBit(CPSR, 31);
							}
							else
							{
								ClearBit(CPSR, 31);
							}
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
							if (Overflow(input, Op2, result, true)) {
								SetBit(CPSR, 28);
							}
							else
							{
								ClearBit(CPSR, 28);
							}
							bool poss = false;
							if (Op1 > 0 || Op2 > 0) {
								poss = true;
							}
							uint64_t tst = uint64_t(Op1) + uint64_t(Op2);
							if ((tst > 0xFFFFFFFF) || (poss && result == 0)) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
							if ((Op1 + Op2) == 0) {
								SetBit(CPSR, 30);
							}
							else
							{
								ClearBit(CPSR, 30);
							}
							if (((Op1 + Op2) >> 31) & 1) {
								SetBit(CPSR, 31);
							}
							else
							{
								ClearBit(CPSR, 31);
							}
							//curInsnAsm = "CMN";
						}
						break;
					}
					case 0xC:
						// ORR
						registers[Rd] = Op1 | Op2;
						if (Op2 == 0x10) {
							Dummy();
						}
						if (S) {
							if (carry) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
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
						//curInsnAsm = "ORR";
						break;
					case 0xD:
						// MOV
						if (registers[0] == 0x00319537) {
							Dummy();
						}
						if (Rd == 1 && Op2 == 0) {
							Dummy();
						}
						registers[Rd] = Op2;
						if (Rd == 30) {
							increment = false;
						}
						if (S) {
							if (carry) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
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
						//curInsnAsm = "MOV";
						break;
					case 0xE:
						// BIC
						registers[Rd] = Op1 & ~(Op2);
						if (S) {
							if (carry) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
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
						//curInsnAsm = "BIC";
						break;
					case 0xF:
						// MVN
						if (registers[0] == 0x1B8C959B) {
							Dummy();
						}
						registers[Rd] = ~(Op2);
						if (S) {
							if (carry) {
								SetBit(CPSR, 29);
							}
							else
							{
								ClearBit(CPSR, 29);
							}
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
	}

	else
	{
		Dummy();
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


