#include "Game.h"

void Game::Draw(Graphics & gfx)
{
	display.Draw(gfx);
}

Game::Game()
{
}

void Game::Update(Graphics & gfx)
{
	if (!start) {
		arm.Load(mmu);
		start = true;
	}
	count++;

	if (count >= 10 && start) {
	
		for (int i = 0; i < 157696; i++)
		{
			arm.Cycle(ppu, mmu);
			if (i % 1232 == 0) {
				ppu.Update(mmu, display);
			}
		}
	}
	//arm.Cycle(ppu, mmu);
	//if (count % 1232 == 0) {
	//	ppu.Update(mmu, display);
	//}
	kbd.ProcessInput();
	ProcessInput();
}

void Game::ProcessInput()
{
	if (kbd.key_UP) {
		ClearBit(mmu.KEYINPUT, 6);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 6);
	}
	if (kbd.key_DOWN) {
		ClearBit(mmu.KEYINPUT, 7);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 7);
	}
	if (kbd.key_LEFT) {
		ClearBit(mmu.KEYINPUT, 5);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 5);
	}
	if (kbd.key_RIGHT) {
		ClearBit(mmu.KEYINPUT, 4);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 4);
	}

	if (kbd.key_W) {
		ClearBit(mmu.KEYINPUT, 0);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 0);
	}
	if (kbd.key_A) {
		ClearBit(mmu.KEYINPUT, 9);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 9);
	}
	if (kbd.key_S) {
		ClearBit(mmu.KEYINPUT, 1);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 1);
	}
	if (kbd.key_D) {
		ClearBit(mmu.KEYINPUT, 8);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 8);
	}
	if (kbd.key_R) {
		ClearBit(mmu.KEYINPUT, 2);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 2);
		//ClearBit(mmu.KEYINPUT, 2);
	}
	if (kbd.key_T) {
		ClearBit(mmu.KEYINPUT, 3);

	}
	else
	{
		SetBit(mmu.KEYINPUT, 3);
	}
}
