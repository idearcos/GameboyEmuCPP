#include "Timer.h"

Timer::Timer(IMMU &mmu) :
	mmu_(mmu)
{

}

void Timer::OnClockLapse(const Clock &op_duration)
{
	// The "Timer Enable" bit of Timer Control only affects the timer, the divider is ALWAYS counting.
	divider_machine_cycles_ += op_duration.GetMachineCycles();
	if (divider_machine_cycles_ >= machine_cycles_per_divider_update_)
	{
		divider_machine_cycles_ -= machine_cycles_per_divider_update_;
		divider_ += 1;
		WriteToMmu(divider_register_, divider_);
	}

	if (running_)
	{
		counter_machine_cycles_ += op_duration.GetMachineCycles();
		if (counter_machine_cycles_ >= machine_cycles_per_counter_update_)
		{
			counter_machine_cycles_ -= machine_cycles_per_counter_update_;
			if (++counter_ == 0)
			{
				counter_ = modulo_;
				auto interrupt_flags = mmu_.Read8bitFromMemory(interrupt_flags_register_);
				interrupt_flags |= 0x04;
				// Bits 5-7 are unused, leave them set to 1
				interrupt_flags |= 0xE0;
				WriteToMmu(interrupt_flags_register_, interrupt_flags);
			}
			WriteToMmu(counter_register_, counter_);
		}
	}
}

void Timer::OnMemoryWrite(const Memory::Address &address, uint8_t value)
{
	if (writing_to_mmu_)
	{
		return;
	}

	Memory::Region region{ Memory::Region::IO };
	uint16_t relative_address{ 0 };
	std::tie(region, relative_address) = address.GetRelativeAddress();

	if (Memory::Region::IO == region)
	{
		if (address == divider_register_)
		{
			// Writing any value to this register resets it to 0x00
			divider_ = 0;
			WriteToMmu(divider_register_, divider_);
		}
		else if (address == counter_register_)
		{
			counter_ = value;
		}
		else if (address == modulo_register_)
		{
			modulo_ = value;
		}
		else if (address == timer_control_register_)
		{
			switch (value & 0x03)
			{
			case 0:
				machine_cycles_per_counter_update_ = 4 * 64;
				break;
			case 1:
				machine_cycles_per_counter_update_ = 4 * 1;
				break;
			case 2:
				machine_cycles_per_counter_update_ = 4 * 4;
				break;
			case 3:
				machine_cycles_per_counter_update_ = 4 * 16;
				break;
			}

			running_ = (value & 0x04) != 0;
		}
	}
}

void Timer::WriteToMmu(const Memory::Address &address, uint8_t value) const
{
	writing_to_mmu_ = true;
	mmu_.Write8bitToMemory(address, value);
	writing_to_mmu_ = false;
}