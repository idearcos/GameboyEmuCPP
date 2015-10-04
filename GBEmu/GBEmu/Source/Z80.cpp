#include "Z80.h"
#include <sstream>
#include <type_traits>
#include "GPU.h"

Z80::Z80(MMU &mmu, GPU &gpu) :
	instructions_(FillInstructionMap()),
	bit_instructions_(FillBitInstructionMap()),
	mmu_(mmu),
	gpu_(gpu)
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

	interrupt_handler_addresses_[Interrupt::VBlank] = 0x0040;
	interrupt_handler_addresses_[Interrupt::LcdStatus] = 0x0048;
	interrupt_handler_addresses_[Interrupt::Timer] = 0x0050;
	interrupt_handler_addresses_[Interrupt::SerialLink] = 0x0058;
	interrupt_handler_addresses_[Interrupt::Joypad] = 0x0060;
}

uint8_t Z80::FetchByte()
{
	const auto byte = mmu_.Read8bitFromMemory(registers_.Read(Register16bit::PC));
	registers_.Increment(Register16bit::PC);
	return byte;
}

void Z80::Execute(uint8_t opcode)
{
	const auto op_duration = instructions_.at(opcode)();
	clock_ += op_duration;
	gpu_.Lapse(op_duration);
}

void Z80::CheckAndHandleInterrupts()
{
	for (auto &pair : interrupts_signaled_)
	{
		if (pair.second && interrupts_enabled_[pair.first])
		{
			interrupt_master_enable_ = false;
			pair.second = false;

			Clock handling_duration(Restart(interrupt_handler_addresses_[pair.first]));
			clock_ += handling_duration;
			gpu_.Lapse(handling_duration);
		}
	}
}

void Z80::OnMemoryWrite(MMU::Region region, uint16_t address, uint8_t value)
{
	if (writing_to_mmu_)
	{
		return;
	}

	if (MMU::Region::IO == region)
	{
		if (interrupt_flags_register == address)
		{
			for (auto& pair : interrupts_signaled_)
			{
				pair.second = (value & static_cast<std::underlying_type_t<Interrupt>>(pair.first)) != 0;
			}
		}
	}
	else if (MMU::Region::ZeroPage == region)
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
	std::stringstream error_msg;
	error_msg << "Wrong operation code 0x" << std::hex << static_cast<size_t>(opcode) << std::endl;
	throw std::runtime_error(error_msg.str());
}
