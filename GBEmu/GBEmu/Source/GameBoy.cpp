#include "GameBoy.h"
#include <iostream>

GameBoy::GameBoy() :
	z80_(mmu_),
	gpu_(mmu_)
{
	mmu_.AddObserver(&gpu_);
	mmu_.LoadRom("opus5.gb");

	dispatch_loop_thread_ = std::thread(&GameBoy::DispatchLoopFunction, this);
}

GameBoy::~GameBoy()
{
	exit_loop_.store(true);
	dispatch_loop_thread_.join();
}

void GameBoy::DispatchLoopFunction()
{
	Clock clock;
	//while (exit_loop_.load() == false)
	while (clock.GetTicks() < 70224)
	{
		const auto opcode = z80_.FetchByte();
		try
		{
			const auto op_duration = z80_.Execute(opcode);
			clock += op_duration;
			gpu_.Lapse(op_duration);
		}
		catch (std::out_of_range &)
		{
			std::cout << "Exception caught: No instruction for op code " << std::hex << static_cast<uint32_t>(opcode) << std::endl;
			break;
		}
		catch (std::exception &exc)
		{
			std::cout << "Exception caught: " << exc.what() << std::endl;
			break;
		}
	}
}
