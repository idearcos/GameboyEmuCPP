#include "GameBoy.h"
#include <iostream>
#include <conio.h>

GameBoy::GameBoy() :
	z80_(mmu_),
	gpu_(mmu_)
{
	mmu_.AddObserver(&gpu_);
	mmu_.LoadRom("tetris.gb");
}

void GameBoy::Run()
{
	Clock clock;
	while (!_kbhit())
	{
		const auto opcode = z80_.FetchByte();
		try
		{
			//const auto op_duration = z80_.Execute(opcode);
			//clock += op_duration;
			gpu_.Lapse(Clock(1, 4));
		}
		catch (std::out_of_range &)
		{
			std::cout << "Exception caught: No instruction for op code 0x" << std::hex << static_cast<uint32_t>(opcode) << std::endl;
			break;
		}
		catch (std::exception &exc)
		{
			std::cout << "Exception caught: " << exc.what() << std::endl;
			break;
		}
	}
}
