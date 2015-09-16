#include "Z80.h"

// JP nn
// (Z80 p.259)
Clock Z80::Jump(uint16_t address)
{
	// Flags not affected
	
	registers.Write(Register16bit::PC, address);

	return Clock(3, 10);
}

// JP cc, nn
// (Z80 p.260)
Clock Z80::JumpIf(uint16_t address, Flags flag, bool flag_value)
{
	// Flags not affected

	if (registers.IsFlagSet(flag) == flag_value)
	{
		registers.Write(Register16bit::PC, address);
	}

	return Clock(3, 10);
}

// JR n
// (Z80 p.262)
Clock Z80::Jump(int8_t displacement)
{
	// Flags not affected

	registers.Write(Register16bit::PC, registers.Read(Register16bit::PC) + displacement);

	return Clock(3, 12);
}

// JR cc, n
// (Z80 p.264) (Z80 p.266) (Z80 p.268) (Z80 p.270)
Clock Z80::JumpIf(int8_t displacement, Flags flag, bool flag_value)
{
	// Flags not affected

	if (registers.IsFlagSet(flag) == flag_value)
	{
		registers.Write(Register16bit::PC, registers.Read(Register16bit::PC) + displacement);
		return Clock(3, 12);
	}

	return Clock(2, 7);
}

// JP *rr
// (Z80 p.272)
Clock Z80::Jump(Register16bit reg_addr)
{
	// Flags not affected

	registers.Write(Register16bit::PC, mmu.Read8bitFromMemory(registers.Read(reg_addr)));

	return Clock(1, 4);
}
