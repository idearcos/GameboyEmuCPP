#include "Z80.h"
#include <sstream>
#include <type_traits>

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

std::map<Z80::Interrupt, Z80::Instruction> Z80::FillInterruptInstructionMap()
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
		msg << "No instruction for op code 0x" << std::hex << static_cast<size_t>(opcode);
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
		msg << "No instruction for interrupt: " << interrupt;
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
	if (interrupt_master_enable_ || (State::Halted == state_) || (State::Stopped == state_)) // STOP under test
	{
		for (auto &pair : interrupts_signaled_)
		{
			if (pair.second && interrupts_enabled_[pair.first])
			{
				// The interrupt master enable is not needed to cancel Halt mode (ref: Game Boy Programming Manual p.112)
				if (State::Halted == state_)
				{
					state_ = State::Running;
				}

				// STOP under test
				if (State::Stopped == state_)
				{
					state_ = State::Running;
				}

				if (interrupt_master_enable_)
				{
					interrupt_master_enable_ = false;
					pair.second = false;

					Execute(pair.first);
				}
			}
		}
	}
}

void Z80::OnMemoryWrite(const Memory::Address &address, uint8_t value)
{
	if (interrupt_flags_register_ == address)
	{
		for (auto& pair : interrupts_signaled_)
		{
			pair.second = (value & static_cast<std::underlying_type_t<Interrupt>>(pair.first)) != 0;
		}
	}
	else if (interrupts_enable_register_ == address)
	{
		for (auto& pair : interrupts_enabled_)
		{
			pair.second = (value & static_cast<std::underlying_type_t<Interrupt>>(pair.first)) != 0;
		}
	}
}

void Z80::WriteToMmu(uint16_t address, uint8_t value) const
{
	mmu_.Write8bitToMemory(address, value);
}

void Z80::WriteToMmu(uint16_t address, uint16_t value) const
{
	mmu_.Write16bitToMemory(address, value);
}

Clock Z80::WrongOpCode(uint8_t opcode)
{
	std::stringstream msg;
	msg << "Wrong operation code 0x" << std::hex << static_cast<size_t>(opcode);
	throw std::runtime_error(msg.str());
}

std::ostream& operator << (std::ostream& os, const Z80::Interrupt& interrupt)
{
	switch (interrupt)
	{
	case Z80::Interrupt::VBlank:
		os << "VBlank";
		break;
	case Z80::Interrupt::LcdStatus:
		os << "LcdStatus";
		break;
	case Z80::Interrupt::Timer:
		os << "Timer";
		break;
	case Z80::Interrupt::SerialLink:
		os << "SerialLink";
		break;
	case Z80::Interrupt::Joypad:
		os << "Joypad";
		break;
	default:
		os << static_cast<size_t>(interrupt);
		break;
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const Z80::State& state)
{
	switch (state)
	{
	case Z80::State::Running:
		os << "Running";
		break;
	case Z80::State::Halted:
		os << "Halted";
		break;
	case Z80::State::Stopped:
		os << "Stopped";
		break;
	default:
		os << static_cast<size_t>(state);
		break;
	}

	return os;
}
