#include "Z80.h"

// ADD rr, rr'
// (Z80 p.186)
Clock Z80::Add(Register16bit dest, Register16bit source)
{
	registers_.SetFlag(Flags::Subtract, false);
	{const uint16_t low_12_bits_result = (registers_.Read(source) & 0x0FFF) + (registers_.Read(dest) & 0x0FFF);
	registers_.SetFlag(Flags::HalfCarry, (low_12_bits_result & 0x1000) != 0); }

	const uint32_t result = registers_.Read(source) + registers_.Read(dest);

	registers_.SetFlag(Flags::Carry, (result & 0x10000) != 0);
	// Z is unaffected

	registers_.Write(dest, static_cast<uint16_t>(result & 0xFFFF));

	return Clock(3, 11);
}

// ADC rr, rr'
// (Z80 p.188)
Clock Z80::AddPlusCarry(Register16bit dest, Register16bit source)
{
	registers_.SetFlag(Flags::Subtract, false);
	{const uint16_t low_12_bits_result = (registers_.Read(source) & 0x0FFF) + (registers_.Read(dest) & 0x0FFF)
		+ (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);
	registers_.SetFlag(Flags::HalfCarry, (low_12_bits_result & 0x1000) != 0); }

	const uint32_t result = registers_.Read(source) + registers_.Read(dest) + (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);

	registers_.SetFlag(Flags::Carry, (result & 0x10000) != 0);
	registers_.SetFlag(Flags::Zero, (result & 0xFFFF) == 0);

	registers_.Write(dest, static_cast<uint16_t>(result & 0xFFFF));

	return Clock(4, 15);
}

// SBC rr, rr'
// (Z80 p.190)
Clock Z80::SubtractMinusCarry(Register16bit dest, Register16bit source)
{
	registers_.SetFlag(Flags::Subtract, true);
	{const uint16_t low_12_bits_result = (registers_.Read(source) & 0x0FFF) - (registers_.Read(dest) & 0x0FFF)
		- (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);
	registers_.SetFlag(Flags::HalfCarry, (low_12_bits_result & 0x1000) != 0); }

	const uint32_t result = registers_.Read(source) - registers_.Read(dest) - (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);

	registers_.SetFlag(Flags::Carry, (result & 0x10000) != 0);
	registers_.SetFlag(Flags::Zero, (result & 0xFFFF) == 0);

	registers_.Write(dest, static_cast<uint16_t>(result & 0xFFFF));

	return Clock(4, 15);
}

// INC rr
// (Z80 p.196)
Clock Z80::Increment(Register16bit reg)
{
	registers_.Increment(reg);

	return Clock(1, 6);
}

// DEC rr
// (Z80 p.199)
Clock Z80::Decrement(Register16bit reg)
{
	registers_.Decrement(reg);

	return Clock(1, 6);
}
