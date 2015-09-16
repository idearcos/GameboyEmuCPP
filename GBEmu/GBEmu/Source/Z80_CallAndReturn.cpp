#include "Z80.h"

// CALL nn
// (Z80 p.278)
Clock Z80::Call(uint16_t address)
{
	// Flags not affected

	registers.Decrement(Register16bit::SP);
	mmu.Write8bitToMemory(registers.Read(Register16bit::SP), (registers.Read(Register16bit::PC) >> 8) & 0xFF);
	registers.Decrement(Register16bit::SP);
	mmu.Write8bitToMemory(registers.Read(Register16bit::SP), (registers.Read(Register16bit::PC) & 0xFF));

	registers.Write(Register16bit::PC, address);

	return Clock(5, 17);
}

// CALL cc, nn
// (Z80 p.280)
Clock Z80::CallIf(uint16_t address, Flags flag, bool flag_value)
{
	// Flags not affected

	if (registers.IsFlagSet(flag) == flag_value)
	{
		registers.Decrement(Register16bit::SP);
		mmu.Write8bitToMemory(registers.Read(Register16bit::SP), (registers.Read(Register16bit::PC) >> 8) & 0xFF);
		registers.Decrement(Register16bit::SP);
		mmu.Write8bitToMemory(registers.Read(Register16bit::SP), (registers.Read(Register16bit::PC) & 0xFF));

		registers.Write(Register16bit::PC, address);

		return Clock(5, 17);
	}

	return Clock(3, 10);
}

// RET
// (Z80 p.282)
Clock Z80::Return()
{
	// Flags not affected

	uint16_t value{ 0 };
	value += static_cast<uint16_t>(mmu.Read8bitFromMemory(registers.Read(Register16bit::SP))) << 8;
	registers.Increment(Register16bit::SP);
	value += mmu.Read8bitFromMemory(registers.Read(Register16bit::SP));
	registers.Increment(Register16bit::SP);

	registers.Write(Register16bit::PC, value);

	return Clock(3, 10);
}

// RET cc
// (Z80 p.283)
Clock Z80::ReturnIf(Flags flag, bool flag_value)
{
	// Flags not affected

	if (registers.IsFlagSet(flag) == flag_value)
	{
		uint16_t value{ 0 };
		value += static_cast<uint16_t>(mmu.Read8bitFromMemory(registers.Read(Register16bit::SP))) << 8;
		registers.Increment(Register16bit::SP);
		value += mmu.Read8bitFromMemory(registers.Read(Register16bit::SP));
		registers.Increment(Register16bit::SP);

		registers.Write(Register16bit::PC, value);

		return Clock(3, 11);
	}

	return Clock(1, 5);
}

// RETI
// (Z80 p.285)
Clock Z80::ReturnFromInterrupt()
{
	// Flags not affected

	uint16_t value{ 0 };
	value += static_cast<uint16_t>(mmu.Read8bitFromMemory(registers.Read(Register16bit::SP))) << 8;
	registers.Increment(Register16bit::SP);
	value += mmu.Read8bitFromMemory(registers.Read(Register16bit::SP));
	registers.Increment(Register16bit::SP);

	registers.Write(Register16bit::PC, value);

	interrupt_enabled_ = true;

	return Clock(4, 14);
}

// RST p
// (Z80 p.289)
Clock Z80::Restart(uint16_t address)
{
	// Flags not affected

	registers.Decrement(Register16bit::SP);
	mmu.Write8bitToMemory(registers.Read(Register16bit::SP), (registers.Read(Register16bit::PC) >> 8) & 0xFF);
	registers.Decrement(Register16bit::SP);
	mmu.Write8bitToMemory(registers.Read(Register16bit::SP), (registers.Read(Register16bit::PC) & 0xFF));

	registers.Write(Register16bit::PC, address);

	return Clock(3, 11);
}
