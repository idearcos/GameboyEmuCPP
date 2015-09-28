#include "Z80.h"
#include <sstream>

Z80::Z80(MMU &mmu) :
	instructions_(FillInstructionMap()),
	bit_instructions_(FillBitInstructionMap()),
	mmu_(mmu)
{
	
}

uint8_t Z80::FetchByte()
{
	const auto opcode = mmu_.Read8bitFromMemory(registers_.Read(Register16bit::PC));
	registers_.Increment(Register16bit::PC);
	return opcode;
}

Clock Z80::Execute(uint8_t opcode)
{
	const auto op_duration = instructions_.at(opcode)();
	clock_ += op_duration;
	return op_duration;
}

Clock Z80::WrongOpCode(uint8_t opcode)
{
	std::stringstream error_msg;
	error_msg << "Wrong operation code 0x" << std::hex << static_cast<size_t>(opcode) << std::endl;
	throw std::runtime_error(error_msg.str());
}
