#include "Z80.h"
#include <sstream>
#include <type_traits>
#include "GPU.h"

Z80::Z80(IMMU &mmu) :
	instructions_(FillInstructionMap()),
	bit_instructions_(FillBitInstructionMap()),
	interrupt_instructions_(FillInterruptInstructionMap()),
	mmu_(mmu)
{
	interrupts_enabled_[Interrupt::VBlank] = false;
	interrupts_enabled_[Interrupt::LcdStatus] = false;
	interrupts_enabled_[Interrupt::Timer] = false;
	interrupts_enabled_[Interrupt::SerialLink] = false;
	interrupts_enabled_[Interrupt::Joypad] = false;

	interrupts_signaled_[Interrupt::VBlank] = false;
	interrupts_signaled_[Interrupt::LcdStatus] = false;
	interrupts_signaled_[Interrupt::Timer] = false;
	interrupts_signaled_[Interrupt::SerialLink] = false;
	interrupts_signaled_[Interrupt::Joypad] = false;
}

std::map<Z80::Interrupt, Instruction> Z80::FillInterruptInstructionMap()
{
	std::map<Interrupt, Instruction> interrupt_instructions;

	interrupt_instructions[Interrupt::VBlank] = [this](){ return this->Restart(0x0040); };
	interrupt_instructions[Interrupt::LcdStatus] = [this](){ return this->Restart(0x0048); };
	interrupt_instructions[Interrupt::Timer] = [this](){ return this->Restart(0x0050); };
	interrupt_instructions[Interrupt::SerialLink] = [this](){ return this->Restart(0x0058); };
	interrupt_instructions[Interrupt::Joypad] = [this](){ return this->Restart(0x0060); };

	return interrupt_instructions;
}

uint8_t Z80::FetchByte()
{
	const auto byte = mmu_.Read8bitFromMemory(registers_.Read(Register16bit::PC));
	registers_.Increment(Register16bit::PC);
	return byte;
}

void Z80::Execute(uint8_t opcode)
{
	try
	{
		Execute(instructions_.at(opcode));
	}
	catch (std::out_of_range &)
	{
		std::stringstream msg;
		msg << "Exception caught: No instruction for op code 0x" << std::hex << static_cast<size_t>(opcode);
		throw std::runtime_error(msg.str());
	}
}

void Z80::Execute(Interrupt interrupt)
{
	try
	{
		Execute(interrupt_instructions_.at(interrupt));
	}
	catch (std::out_of_range &)
	{
		std::stringstream msg;
		msg << "Exception caught: No instruction for interrupt " << static_cast<size_t>(interrupt);
		throw std::runtime_error(msg.str());
	}
}

void Z80::Execute(Instruction instruction)
{
	const auto op_duration = instruction();
	clock_ += op_duration;
	Notify(&Z80Observer::OnClockLapse, op_duration);
}

void Z80::CheckAndHandleInterrupts()
{
	for (auto &pair : interrupts_signaled_)
	{
		if (pair.second && interrupts_enabled_[pair.first])
		{
			interrupt_master_enable_ = false;
			pair.second = false;

			Execute(pair.first);
		}
	}
}

void Z80::OnMemoryWrite(Region region, uint16_t address, uint8_t value)
{
	if (writing_to_mmu_)
	{
		return;
	}

	if (Region::IO == region)
	{
		if (interrupt_flags_register == address)
		{
			for (auto& pair : interrupts_signaled_)
			{
				pair.second = (value & static_cast<std::underlying_type_t<Interrupt>>(pair.first)) != 0;
			}
		}
	}
	else if (Region::ZeroPage == region)
	{
		if (interrupts_enable_register == address)
		{
			for (auto& pair : interrupts_enabled_)
			{
				pair.second = (value & static_cast<std::underlying_type_t<Interrupt>>(pair.first)) != 0;
			}
		}
	}
}

void Z80::WriteToMmu(uint16_t address, uint8_t value) const
{
	writing_to_mmu_ = true;
	mmu_.Write8bitToMemory(address, value);
	writing_to_mmu_ = false;
}

void Z80::WriteToMmu(uint16_t address, uint16_t value) const
{
	writing_to_mmu_ = true;
	mmu_.Write16bitToMemory(address, value);
	writing_to_mmu_ = false;
}

Clock Z80::WrongOpCode(uint8_t opcode)
{
	std::stringstream msg;
	msg << "Wrong operation code 0x" << std::hex << static_cast<size_t>(opcode);
	throw std::runtime_error(msg.str());
}
