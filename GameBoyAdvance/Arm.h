#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "PPU.h"
#include "MMU.h"
#include "Graphics.h"


class Arm
{
public:
	bool q = false;
	int count = 0;
	bool hit = false;
	int frame = 0;

	Arm();
	void Load(MMU& mmu);
	void Cycle(PPU& ppu, MMU& mmu);
	void AlignAddr(uint32_t& addr);
	uint32_t bitrange(int msb, int lsb, uint16_t insn);
	uint32_t bitrange(int msb, int lsb, uint32_t insn);
	uint32_t bitrange(int msb, int lsb, uint64_t insn);
	void Decode(uint32_t insn, PPU& ppu, MMU& mmu);
	void SetMode();
	void SetBit(uint32_t& val, int bit);
	void SetBit(uint16_t& val, int bit);
	void ClearBit(uint32_t& val, int bit);
	void ClearBit(uint16_t& val, int bit);
	struct rStrings
	{
		std::string registerSet[30];
	};
private:
	void DMATransfer(MMU& mmu, PPU& ppu);
	void Timer(MMU& mmu);
	bool Overflow(uint32_t val1, uint32_t val2, uint32_t result, bool add);
	void DecodeThumb(uint16_t insn, MMU& mmu);
	void Dummy();
	int WhatRegister(uint32_t input);
	void SetFlags(uint32_t Rd, bool carry, bool overflow, bool setV);
	void SetFlagsSub(uint32_t Rd, bool carry, bool overflow, bool setV);
	uint32_t insn = 0;
	uint32_t ins1 = 0;
	uint32_t ins2 = 0;
	uint32_t ins3 = 0;
	void SetState();
	uint32_t FetchArm(MMU& mmu);
	uint16_t FetchThumb(MMU& mmu);
	uint16_t Tinsn = 0;
	uint32_t Ainsn = 0;
	bool increment = true;
	void ReadCpsr();
	void Interrupt(MMU& mmu, PPU& ppu);
	void CallInterrupt();
	bool Condition(uint32_t insn);


	//Timers
	uint16_t Timer0 = 0;
	uint16_t Timer0ReloadVal = 0;
	uint16_t Timer0Cnt = 0;
	bool Timer0Irq = false;

	uint16_t Timer1 = 0;
	uint16_t Timer1ReloadVal = 0;
	uint16_t Timer1Cnt = 0;
	bool Timer1Irq = false;

	uint16_t Timer2 = 0;
	uint16_t Timer2ReloadVal = 0;
	uint16_t Timer2Cnt = 0;
	bool Timer2Irq = false;

	uint16_t Timer3 = 0;
	uint16_t Timer3ReloadVal = 0;
	uint16_t Timer3Cnt = 0;
	bool Timer3Irq = false;

	bool Timer0OverFlow = false;
	bool Timer1OverFlow = false;
	bool Timer2OverFlow = false;
	bool Timer3OverFlow = false;


	//Audio
	bool fifoEmpty = false;



	//DMA

	bool DMA0irq = false;
	uint32_t dma_0_addr_src = 0;
	uint32_t dma_0_addr_dest = 0;
	bool needs_reload0 = false;
	int dma0cycle = 0;

	bool DMA1irq = false;
	uint32_t dma_1_addr_src = 0;
	uint32_t dma_1_addr_dest = 0;
	bool needs_reload1 = false;
	int dma1cycle = 0;

	bool DMA2irq = false;
	uint32_t dma_2_addr_src = 0;
	uint32_t dma_2_addr_dest = 0;
	bool needs_reload2 = false;
	int dma2cycle = 0;

	bool DMA3irq = false;
	uint32_t dma_3_addr_src = 0;
	uint32_t dma_3_addr_dest = 0;
	bool needs_reload3 = false;
	int dma3cycle = 0;

	bool irqd = false;

	uint32_t rotateRight(uint32_t num, uint16_t rotation)
	{
		rotation %= 32;

		while (rotation--)
			num = ((num >> 1) & (~(1 << 31)) | ((num & 1) << 31));

		return num;
	}
	uint32_t arithmeticRight(uint32_t num, uint32_t val) {
		uint32_t finalVal = 0;
		bool sign = ((num >> 31) & 1);
		if (sign) {
			finalVal = num >> val;
			int index = 31;

			for (uint32_t i = 0; i < val; i++) {
				SetBit(finalVal, index);
				index--;
			}

		}
		else
		{
			finalVal = num >> val;
		}
		return finalVal;
	}


	//IRQ
	bool bExecute = true;




	// Logical Functions
	void MOV(uint32_t insn);
	void MVN(uint32_t insn);
	void AND(uint32_t insn);
	void TST(uint32_t insn);
	void EOR(uint32_t insn);
	void TEQ(uint32_t insn);
	void ORR(uint32_t insn);
	void BIC(uint32_t insn);

	// Arithmetic Functions
	void ADD(uint32_t insn);
	void ADC(uint32_t insn);
	void SUB(uint32_t insn);
	void SBC(uint32_t insn);
	void RSB(uint32_t insn);
	void RSC(uint32_t insn);
	void CMP(uint32_t insn);
	void CMN(uint32_t insn);

	// Multiply Functions
	void MUL(uint32_t insn);
	void MLA(uint32_t insn);
	void UMULL(uint32_t insn);
	void UMLAL(uint32_t insn);
	void SMULL(uint32_t insn);
	void SMLAL(uint32_t insn);
	void SMLAxy(uint32_t insn);
	void SMLAWy(uint32_t insn);
	void SMULWy(uint32_t insn);
	void SMLALxy(uint32_t insn);
	void SMULxy(uint32_t insn);

	// Memory Functions
	void LDR(uint32_t insn);
	//void LDR(uint32_t insn);
	//void LDR(uint32_t insn);
	//void LDR(uint32_t insn);
	//void LDR(uint32_t insn);
	void LDM(uint32_t insn);
	void STM(uint32_t insn);
	void BlockTransfer(uint32_t insn, MMU& mmu);
	void SWP(uint32_t insn);
	void PLD(uint32_t insn);

	// Jumps,Calls,Other
	void B(uint32_t insn);
	void BL(uint32_t insn);
	void BX(uint32_t insn);
	void BLX(uint32_t insn);
	//void BLX(uint32_t insn);
	void MRS(uint32_t insn);
	void MSR(uint32_t insn);
	void SWI(uint32_t insn);
	void BKPT(uint32_t insn);


	std::string curInsnAsm = "NULL";


	enum class Mode {
		User,
		Fiq,
		Irq,
		Supervisor,
		Abort,
		Undefined,
		System
	};

	enum class State
	{
		Arm,
		Thumb
	};

	Mode mode = Mode::User;
	State state = State::Arm;

	bool N, Z, C, V, Q;









	// Registers

	// General purpose registers:
	uint32_t registers[31];

	uint32_t registers_user[16];
	uint32_t registers_fiq[7];
	uint32_t registers_svc[2];
	uint32_t registers_abt[2];
	uint32_t registers_irq[2];
	uint32_t registers_und[2];

	// 0 - 7 General
	// 8 - R8
	// 9 - R8_fiq
	// 10 - R9
	// 11 - R9_fiq
	// 12 - R10
	// 13 - R10_fiq
	// 14 - R11
	// 15 - R11_fiq
	// 16 - R12
	// 17 - R12_fiq
	// 18 - R13		- Stack Pointer
	// 19 - R13_svc	- Stack Pointer
	// 20 - R13_abt	- Stack Pointer
	// 21 - R13_und	- Stack Pointer
	// 22 - R13_irq	- Stack Pointer
	// 23 - R13_fiq	- Stack Pointer
	// 24 - R14		- Link Register
	// 25 - R14_svc	- Link Register
	// 26 - R14_abt	- Link Register
	// 27 - R14_und	- Link Register
	// 28 - R14_irq	- Link Register
	// 29 - R14_fiq	- Link Register
	// 30 - R15 - Program Counter


	// Program status registers
	uint32_t CPSR = 0b00000000000000000000000000010000;
	uint32_t SPSR_fiq = 0;
	uint32_t SPSR_svc = 0;
	uint32_t SPSR_abt = 0;
	uint32_t SPSR_irq = 0;
	uint32_t SPSR_und = 0;





};

