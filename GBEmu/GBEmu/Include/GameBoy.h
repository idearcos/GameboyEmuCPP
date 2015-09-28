#pragma once

#include <thread>
#include "Z80.h"
#include "MMU.h"
#include "GPU.h"

class GameBoy
{
public:
	GameBoy();
	~GameBoy() = default;

	void Run();

private:
	Z80 z80_;
	MMU mmu_;
	GPU gpu_;
};
