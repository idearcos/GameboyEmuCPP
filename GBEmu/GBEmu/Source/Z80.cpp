#include "Z80.h"

#include <iostream>

Z80::Z80() :
	instructions_(FillInstructionMap()),
	bit_instructions_(FillBitInstructionMap()),
	gpu_(mmu_)
{
	mmu_.AddObserver(&gpu_);
}

void Z80::DispatchLoopFunction()
{
	while (exit_loop_.load() == false)
	{
		const auto opcode = FetchByte();
		try
		{
			const auto op_duration = instructions_.at(opcode)();
			clock_ += op_duration;
			//TODO Lapse GPU
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

uint8_t Z80::FetchByte()
{
	const auto opcode = mmu_.Read8bitFromMemory(registers_.Read(Register16bit::PC));
	registers_.Increment(Register16bit::PC);
	return opcode;
}

Clock Z80::WrongOpCode()
{
	throw std::runtime_error("Wrong operation code");
}
