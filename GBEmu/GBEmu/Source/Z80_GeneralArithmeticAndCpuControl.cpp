#include "Z80.h"

// DAA
// (The undocumented Z80 p.17)
// ( http://www.worldofspectrum.org/faq/reference/z80reference.htm )
Clock Z80::DAA()
{
	uint8_t correction_factor{ 0 };
	const uint8_t original_value{ registers_.Read(Register8bit::A) };

	// Correct the high nibble first
	if ((original_value > 0x99) || registers_.IsFlagSet(Flags::Carry))
	{
		correction_factor |= 0x60;
	}
	registers_.SetFlag(Flags::Carry, correction_factor != 0);

	// Correct the low nibble
	if (((original_value & 0x0F) > 9) || registers_.IsFlagSet(Flags::HalfCarry))
	{
		correction_factor |= 0x06;
	}
	registers_.SetFlag(Flags::HalfCarry, (correction_factor & 0x0F) != 0);

	{const uint8_t result = registers_.IsFlagSet(Flags::Subtract) ? original_value - correction_factor :
		original_value + correction_factor;

	registers_.SetFlag(Flags::Zero, result == 0);

	registers_.Write(Register8bit::A, result); }

	return Clock(1, 4);
}

// CPL
// (Z80 p.173)
Clock Z80::InvertAccumulator()
{
	registers_.SetFlag(Flags::HalfCarry, true);
	registers_.SetFlag(Flags::Subtract, true);

	registers_.Write(Register8bit::A, ~registers_.Read(Register8bit::A));

	return Clock(1, 4);
}

// CCF
// (Z80 p.176)
Clock Z80::InvertCarryFlag()
{
	registers_.SetFlag(Flags::Subtract, false);
	registers_.SetFlag(Flags::HalfCarry, registers_.IsFlagSet(Flags::Carry));
	registers_.SetFlag(Flags::Carry, !registers_.IsFlagSet(Flags::Carry));

	return Clock(1, 4);
}

// SCF
// (Z80 p.177)
Clock Z80::SetCarryFlag()
{
	registers_.SetFlag(Flags::Subtract, false);
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Carry, true);

	return Clock(1, 4);
}

// NOP
// (Z80 p.178)
Clock Z80::NoOperation()
{
	return Clock(1, 4);
}

// HALT
// (Z80 p.179)
Clock Z80::Halt()
{
	state_ = State::Halted;

	return Clock(1, 4);
}

// DI
// (Z80 p.180)
Clock Z80::DisableInterrupts()
{
	interrupt_master_enable_ = false;

	return Clock(1, 4);
}

// EI
// (Z80 p.181)
Clock Z80::EnableInterrupts()
{
	interrupt_master_enable_ = true;

	return Clock(1, 4);
}
