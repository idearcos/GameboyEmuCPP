#pragma once

#include <thread>
#include "Z80.h"
#include "MMU.h"
#include "GPU.h"
#include "Keys.h"
#include "Timer.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"

class GameBoy
{
public:
	GameBoy();
	~GameBoy() = default;

	void Run();

	static void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	GLFWwindow* window_{ nullptr };
	Z80 z80_;
	MMU mmu_;
	GPU gpu_;
	KeyPad keypad_;
	Timer timer_;
};
