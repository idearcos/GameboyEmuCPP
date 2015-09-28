#pragma once

#include <thread>
#include "Z80.h"
#include "MMU.h"
#include "GPU.h"

class GameBoy
{
public:
	GameBoy();
	~GameBoy();

private:
	void DispatchLoopFunction();

private:
	Z80 z80_;
	MMU mmu_;
	GPU gpu_;

	std::atomic<bool> exit_loop_{ false };
	std::thread dispatch_loop_thread_;
};
