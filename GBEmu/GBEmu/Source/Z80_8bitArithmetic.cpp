#include "Z80.h"

// ADD A, n
// (Z80 p.145)
Clock Z80::Add(uint8_t value)
{
	registers_.SetFlag(Flags::Subtract, false);
	{const uint8_t low_nibble_result = (registers_.Read(Register8bit::A) & 0x0F) + (value & 0x0F);
	registers_.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }
	
	const uint16_t result = registers_.Read(Register8bit::A) + value;

	registers_.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers_.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	registers_.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 8);
}

// ADD A, r
// (Z80 p.143)
Clock Z80::Add(Register8bit reg)
{
	Add(registers_.Read(reg));
	return Clock(1, 4);
}

// ADD A, (rr)
// (Z80 p.146)
Clock Z80::Add(Register16bit source_addr)
{
	Add(mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// ADC A, n
// (Z80 p.149)
Clock Z80::AddPlusCarry(uint8_t value)
{
	registers_.SetFlag(Flags::Subtract, false);
	{const uint8_t low_nibble_result = (registers_.Read(Register8bit::A) & 0x0F) + (value & 0x0F)
		+ (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);
	registers_.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }
	
	const uint16_t result = registers_.Read(Register8bit::A) + value + (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);

	registers_.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers_.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	registers_.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 8);
}

// ADC A, r
// (Z80 p.149)
Clock Z80::AddPlusCarry(Register8bit reg)
{
	AddPlusCarry(registers_.Read(reg));
	return Clock(1, 4);
}

// ADC A, (rr)
// (Z80 p.149)
Clock Z80::AddPlusCarry(Register16bit source_addr)
{
	AddPlusCarry(mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// SUB A, n
// (Z80 p.151)
Clock Z80::Sub(uint8_t value)
{
	registers_.SetFlag(Flags::Subtract, true);
	{const uint8_t low_nibble_result = (registers_.Read(Register8bit::A) & 0x0F) - (value & 0x0F);
	registers_.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }
	
	const uint16_t result = registers_.Read(Register8bit::A) - value;

	registers_.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers_.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	registers_.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 8);
}

// SUB A, r
// (Z80 p.151)
Clock Z80::Sub(Register8bit reg)
{
	Sub(registers_.Read(reg));
	return Clock(1, 4);
}

// SUB A, (rr)
// (Z80 p.151)
Clock Z80::Sub(Register16bit source_addr)
{
	Sub(mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// SBC A, n
// (Z80 p.153)
Clock Z80::SubMinusCarry(uint8_t value)
{
	registers_.SetFlag(Flags::Subtract, true);
	{const uint8_t low_nibble_result = (registers_.Read(Register8bit::A) & 0x0F) - (value & 0x0F)
		- (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);
	registers_.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }
	
	const uint16_t result = registers_.Read(Register8bit::A) - value - (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);

	registers_.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers_.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	registers_.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 8);
}

// SBC A, r
// (Z80 p.153)
Clock Z80::SubMinusCarry(Register8bit reg)
{
	SubMinusCarry(registers_.Read(reg));
	return Clock(1, 4);
}

// SBC A, (rr)
// (Z80 p.153)
Clock Z80::SubMinusCarry(Register16bit source_addr)
{
	SubMinusCarry(mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// AND n
// (Z80 p.155)
Clock Z80::And(uint8_t value)
{
	registers_.SetFlag(Flags::Carry, false);
	registers_.SetFlag(Flags::Subtract, false);
	registers_.SetFlag(Flags::HalfCarry, true);

	const uint8_t result = registers_.Read(Register8bit::A) & value;
	registers_.SetFlag(Flags::Zero, result == 0);

	registers_.Write(Register8bit::A, result);

	return Clock(2, 8);
}

// AND r
// (Z80 p.155)
Clock Z80::And(Register8bit reg)
{
	And(registers_.Read(reg));
	return Clock(1, 4);
}

// AND (rr)
// (Z80 p.155)
Clock Z80::And(Register16bit source_addr)
{
	And(mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// OR n
// (Z80 p.157)
Clock Z80::Or(uint8_t value)
{
	registers_.SetFlag(Flags::Carry, false);
	registers_.SetFlag(Flags::Subtract, false);
	registers_.SetFlag(Flags::HalfCarry, false);

	const uint8_t result = registers_.Read(Register8bit::A) | value;
	registers_.SetFlag(Flags::Zero, result == 0);

	registers_.Write(Register8bit::A, result);

	return Clock(2, 8);
}

// OR r
// (Z80 p.157)
Clock Z80::Or(Register8bit reg)
{
	Or(registers_.Read(reg));
	return Clock(1, 4);
}

// OR (rr)
// (Z80 p.157)
Clock Z80::Or(Register16bit source_addr)
{
	Or(mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// XOR n
// (Z80 p.159)
Clock Z80::Xor(uint8_t value)
{
	registers_.SetFlag(Flags::Carry, false);
	registers_.SetFlag(Flags::Subtract, false);
	registers_.SetFlag(Flags::HalfCarry, false);

	const uint8_t result = registers_.Read(Register8bit::A) ^ value;
	registers_.SetFlag(Flags::Zero, result == 0);

	registers_.Write(Register8bit::A, result);

	return Clock(2, 8);
}

// XOR r
// (Z80 p.159)
Clock Z80::Xor(Register8bit reg)
{
	Xor(registers_.Read(reg));
	return Clock(1, 4);
}

// XOR (rr)
// (Z80 p.159)
Clock Z80::Xor(Register16bit source_addr)
{
	Xor(mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// CP n
// (Z80 p.161)
Clock Z80::Compare(uint8_t value)
{
	registers_.SetFlag(Flags::Subtract, true);
	{const uint8_t low_nibble_result = (registers_.Read(Register8bit::A) & 0x0F) - (value & 0x0F);
	registers_.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }

	const uint16_t result = registers_.Read(Register8bit::A) - value;

	registers_.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers_.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	return Clock(2, 8);
}

// CP r
// (Z80 p.161)
Clock Z80::Compare(Register8bit reg)
{
	Compare(registers_.Read(reg));
	return Clock(1, 4);
}

// CP (rr)
// (Z80 p.161)
Clock Z80::Compare(Register16bit source_addr)
{
	Compare(mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// INC r
// (Z80 p.163)
Clock Z80::Increment(Register8bit reg)
{
	registers_.SetFlag(Flags::Subtract, false);
	// Carry is unaffected
	{const uint8_t low_nibble = registers_.Read(reg) & 0x0F;
	registers_.SetFlag(Flags::HalfCarry, low_nibble == 0x0F); }
	
	const uint8_t result = registers_.Read(reg) + 1;
	registers_.SetFlag(Flags::Zero, result == 0);

	registers_.Write(reg, result);

	return Clock(1, 4);
}

// INC (rr)
// (Z80 p.165)
Clock Z80::IncrementInAddress(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::Subtract, false);
	// Carry is unaffected
	{const uint8_t low_nibble = mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 0x0F;
	registers_.SetFlag(Flags::HalfCarry, low_nibble == 0x0F); }

	const uint8_t result = mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) + 1;

	registers_.SetFlag(Flags::Zero, result == 0);

	WriteToMmu(registers_.Read(reg_addr), result);

	return Clock(3, 12);
}

// DEC r
// (Z80 p.168)
Clock Z80::Decrement(Register8bit reg)
{
	registers_.SetFlag(Flags::Subtract, true);
	// Carry is unaffected
	{const uint8_t low_nibble = registers_.Read(reg) & 0x0F;
	registers_.SetFlag(Flags::HalfCarry, low_nibble == 0); }

	const uint8_t result = registers_.Read(reg) - 1;

	registers_.SetFlag(Flags::Zero, result == 0);

	registers_.Write(reg, result);

	return Clock(1, 4);
}

// DEC (rr)
// (Z80 p.168)
Clock Z80::DecrementInAddress(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::Subtract, true);
	// Carry is unaffected
	{const uint8_t low_nibble = mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 0x0F;
	registers_.SetFlag(Flags::HalfCarry, low_nibble == 0); }

	const uint8_t result = mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) - 1;

	registers_.SetFlag(Flags::Zero, result == 0);

	WriteToMmu(registers_.Read(reg_addr), result);

	return Clock(3, 12);
}
