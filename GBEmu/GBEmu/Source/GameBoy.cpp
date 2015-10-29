#include "GameBoy.h"
#include <iostream>

GameBoy::GameBoy() :
	z80_(mmu_),
	gpu_(window_, mmu_),
	keypad_(mmu_)
{
	z80_.AddObserver(&gpu_);
	mmu_.AddObserver(&z80_);
	mmu_.AddObserver(&gpu_);
	mmu_.AddObserver(&keypad_);
	mmu_.LoadRom("ttt.gb");

	glfwSetWindowUserPointer(window_, this);
	glfwSetKeyCallback(window_, GameBoy::OnKeyEvent);
}

void GameBoy::Run()
{
	while (!glfwWindowShouldClose(window_))
	{
		try
		{
			switch (z80_.GetState())
			{
			case Z80::State::Running:
				// STOP under test
			case Z80::State::Stopped:
				z80_.Execute(z80_.FetchByte());
				break;
			case Z80::State::Halted:
				z80_.Execute(0x00);
				break;
			}

			z80_.CheckAndHandleInterrupts();
		}
		catch (std::exception &exc)
		{
			std::cout << "Exception caught: " << exc.what() << std::endl;
			break;
		}
	}
}

void GameBoy::OnKeyEvent(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	reinterpret_cast<GameBoy*>(glfwGetWindowUserPointer(window))->keypad_.HandleKeys(key, action);
}
