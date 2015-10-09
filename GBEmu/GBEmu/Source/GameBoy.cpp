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
		const auto opcode = z80_.FetchByte();
		try
		{
			z80_.Execute(opcode);
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
