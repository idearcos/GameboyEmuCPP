#include "Z80.h"

// CALL nn
// (Z80 p.278)
Clock Z80::Call(uint16_t address)
{
	// Flags not affected

	registers_.Decrement(Register16bit::SP);
	registers_.Decrement(Register16bit::SP);
	WriteToMmu(registers_.Read(Register16bit::SP), registers_.Read(Register16bit::PC));

	registers_.Write(Register16bit::PC, address);

	return Clock(6, 24);
}

// CALL cc, nn
// (Z80 p.280)
Clock Z80::CallIf(uint16_t address, Flags flag, bool flag_value)
{
	// Flags not affected

	if (registers_.IsFlagSet(flag) == flag_value)
	{
		registers_.Decrement(Register16bit::SP);
		registers_.Decrement(Register16bit::SP);
		WriteToMmu(registers_.Read(Register16bit::SP), registers_.Read(Register16bit::PC));

		registers_.Write(Register16bit::PC, address);

		return Clock(6, 24);
	}

	return Clock(3, 12);
}

// RET
// (Z80 p.282)
Clock Z80::Return()
{
	// Flags not affected

	uint16_t value{ mmu_.Read16bitFromMemory(registers_.Read(Register16bit::SP)) };
	registers_.Increment(Register16bit::SP);
	registers_.Increment(Register16bit::SP);

	registers_.Write(Register16bit::PC, value);

	return Clock(4, 16);
}

// RET cc
// (Z80 p.283)
Clock Z80::ReturnIf(Flags flag, bool flag_value)
{
	// Flags not affected

	if (registers_.IsFlagSet(flag) == flag_value)
	{
		uint16_t value{ mmu_.Read16bitFromMemory(registers_.Read(Register16bit::SP)) };
		registers_.Increment(Register16bit::SP);
		registers_.Increment(Register16bit::SP);

		registers_.Write(Register16bit::PC, value);

		return Clock(5, 20);
	}

	return Clock(2, 8);
}

// RETI
// (Z80 p.285)
Clock Z80::ReturnFromInterrupt()
{
	// Flags not affected

	registers_.Write(Register16bit::PC, mmu_.Read16bitFromMemory(registers_.Read(Register16bit::SP)));
	registers_.Increment(Register16bit::SP);
	registers_.Increment(Register16bit::SP);

	interrupt_master_enable_ = true;

	return Clock(4, 16);
}

// RST p
// (Z80 p.289)
Clock Z80::Restart(uint16_t address)
{
	// Flags not affected

	registers_.Decrement(Register16bit::SP);
	registers_.Decrement(Register16bit::SP);

	WriteToMmu(registers_.Read(Register16bit::SP), registers_.Read(Register16bit::PC));

	registers_.Write(Register16bit::PC, address);

	return Clock(4, 16);
}
