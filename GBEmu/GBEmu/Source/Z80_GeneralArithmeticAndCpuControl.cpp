#include "Z80.h"


// DAA
// (The undocumented Z80 p.17)
// ( http://www.worldofspectrum.org/faq/reference/z80reference.htm )
Clock Z80::DAA()
{
	uint8_t correction_factor{ 0 };
	const uint8_t original_value{ registers.Read(Register8bit::A) };

	// Correct the high nibble first
	if ((original_value > 0x99) || registers.IsFlagSet(Flags::Carry))
	{
		correction_factor |= 0x60;
	}
	registers.SetFlag(Flags::Carry, correction_factor != 0);

	// Correct the low nibble
	if (((original_value & 0x0F) > 9) || registers.IsFlagSet(Flags::HalfCarry))
	{
		correction_factor |= 0x06;
	}

	{const uint8_t result = registers.IsFlagSet(Flags::Subtract) ? original_value - correction_factor :
		original_value + correction_factor;

	// If borrow/carry from bit 3 to 4 has been caused because of the operation, set half carry flag
	registers.SetFlag(Flags::HalfCarry, (~(original_value & result) & 0x10) != 0);
	registers.SetFlag(Flags::Zero, result == 0);

	registers.Write(Register8bit::A, result); }

	return Clock(1, 4);
}

// CPL
// (Z80 p.173)
Clock Z80::InvertAccumulator()
{
	registers.SetFlag(Flags::HalfCarry, true);
	registers.SetFlag(Flags::Subtract, true);

	registers.Write(Register8bit::A, ~registers.Read(Register8bit::A));

	return Clock(1, 4);
}

// CCF
// (Z80 p.176)
Clock Z80::InvertCarryFlag()
{
	registers.SetFlag(Flags::Subtract, false);
	registers.SetFlag(Flags::HalfCarry, registers.IsFlagSet(Flags::Carry));
	registers.SetFlag(Flags::Carry, !registers.IsFlagSet(Flags::Carry));

	return Clock(1, 4);
}

// SCF
// (Z80 p.177)
Clock Z80::SetCarryFlag()
{
	registers.SetFlag(Flags::Subtract, false);
	registers.SetFlag(Flags::HalfCarry, false);
	registers.SetFlag(Flags::Carry, true);

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
	interrupt_enabled_ = false;

	return Clock(1, 4);
}

// EI
// (Z80 p.181)
Clock Z80::EnableInterrupts()
{
	interrupt_enabled_ = true;

	return Clock(1, 4);
}
