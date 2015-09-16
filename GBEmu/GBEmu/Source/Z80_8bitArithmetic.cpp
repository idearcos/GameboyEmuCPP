#include "Z80.h"

// ADD A, n
// (Z80 p.145)
Clock Z80::Add(uint8_t value)
{
	registers.SetFlag(Flags::Subtract, false);
	{const uint8_t low_nibble_result = (registers.Read(Register8bit::A) & 0x0F) + (value & 0x0F);
	registers.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }
	
	const uint16_t result = registers.Read(Register8bit::A) + value;

	registers.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	registers.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 7);
}

// ADD A, r
// (Z80 p.143)
Clock Z80::Add(Register8bit reg)
{
	Add(registers.Read(reg));
	return Clock(1, 4);
}

// ADD A, (rr)
// (Z80 p.146)
Clock Z80::Add(Register16bit source_addr)
{
	Add(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// ADC A, n
// (Z80 p.149)
Clock Z80::AddPlusCarry(uint8_t value)
{
	registers.SetFlag(Flags::Subtract, false);
	{const uint8_t low_nibble_result = (registers.Read(Register8bit::A) & 0x0F) + (value & 0x0F)
		+ (registers.IsFlagSet(Flags::Carry) ? 1 : 0);
	registers.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }
	
	const uint16_t result = registers.Read(Register8bit::A) + value + (registers.IsFlagSet(Flags::Carry) ? 1 : 0);

	registers.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	registers.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 7);
}

// ADC A, r
// (Z80 p.149)
Clock Z80::AddPlusCarry(Register8bit reg)
{
	AddPlusCarry(registers.Read(reg));
	return Clock(1, 4);
}

// ADC A, (rr)
// (Z80 p.149)
Clock Z80::AddPlusCarry(Register16bit source_addr)
{
	AddPlusCarry(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// SUB A, n
// (Z80 p.151)
Clock Z80::Sub(uint8_t value)
{
	registers.SetFlag(Flags::Subtract, true);
	{const uint8_t low_nibble_result = (registers.Read(Register8bit::A) & 0x0F) - (value & 0x0F);
	registers.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }
	
	const uint16_t result = registers.Read(Register8bit::A) - value;

	registers.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	registers.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 7);
}

// SUB A, r
// (Z80 p.151)
Clock Z80::Sub(Register8bit reg)
{
	Sub(registers.Read(reg));
	return Clock(1, 4);
}

// SUB A, (rr)
// (Z80 p.151)
Clock Z80::Sub(Register16bit source_addr)
{
	Sub(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// SBC A, n
// (Z80 p.153)
Clock Z80::SubMinusCarry(uint8_t value)
{
	registers.SetFlag(Flags::Subtract, true);
	{const uint8_t low_nibble_result = (registers.Read(Register8bit::A) & 0x0F) - (value & 0x0F)
		- (registers.IsFlagSet(Flags::Carry) ? 1 : 0);
	registers.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }
	
	const uint16_t result = registers.Read(Register8bit::A) - value - (registers.Read(Register8bit::A) ? 1 : 0);

	registers.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	registers.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 7);
}

// SBC A, r
// (Z80 p.153)
Clock Z80::SubMinusCarry(Register8bit reg)
{
	SubMinusCarry(registers.Read(reg));
	return Clock(1, 4);
}

// SBC A, (rr)
// (Z80 p.153)
Clock Z80::SubMinusCarry(Register16bit source_addr)
{
	SubMinusCarry(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// AND n
// (Z80 p.155)
Clock Z80::And(uint8_t value)
{
	registers.SetFlag(Flags::Carry, false);
	registers.SetFlag(Flags::Subtract, false);
	registers.SetFlag(Flags::HalfCarry, true);

	const uint8_t result = registers.Read(Register8bit::A) & value;
	registers.SetFlag(Flags::Zero, result == 0);

	registers.Write(Register8bit::A, result);

	return Clock(2, 7);
}

// AND r
// (Z80 p.155)
Clock Z80::And(Register8bit reg)
{
	And(registers.Read(reg));
	return Clock(1, 4);
}

// AND (rr)
// (Z80 p.155)
Clock Z80::And(Register16bit source_addr)
{
	And(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// OR n
// (Z80 p.157)
Clock Z80::Or(uint8_t value)
{
	registers.SetFlag(Flags::Carry, false);
	registers.SetFlag(Flags::Subtract, false);
	registers.SetFlag(Flags::HalfCarry, false);

	const uint8_t result = registers.Read(Register8bit::A) | value;
	registers.SetFlag(Flags::Zero, result == 0);

	registers.Write(Register8bit::A, result);

	return Clock(2, 7);
}

// OR r
// (Z80 p.157)
Clock Z80::Or(Register8bit reg)
{
	Or(registers.Read(reg));
	return Clock(1, 4);
}

// OR (rr)
// (Z80 p.157)
Clock Z80::Or(Register16bit source_addr)
{
	Or(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// XOR n
// (Z80 p.159)
Clock Z80::Xor(uint8_t value)
{
	registers.SetFlag(Flags::Carry, false);
	registers.SetFlag(Flags::Subtract, false);
	registers.SetFlag(Flags::HalfCarry, false);

	const uint8_t result = ~(registers.Read(Register8bit::A) & value);
	registers.SetFlag(Flags::Zero, result == 0);

	registers.Write(Register8bit::A, result);

	return Clock(2, 7);
}

// XOR r
// (Z80 p.159)
Clock Z80::Xor(Register8bit reg)
{
	Xor(registers.Read(reg));
	return Clock(1, 4);
}

// XOR (rr)
// (Z80 p.159)
Clock Z80::Xor(Register16bit source_addr)
{
	Xor(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// CP n
// (Z80 p.161)
Clock Z80::Compare(uint8_t value)
{
	registers.SetFlag(Flags::Subtract, true);
	{const uint8_t low_nibble_result = (registers.Read(Register8bit::A) & 0x0F) - (value & 0x0F);
	registers.SetFlag(Flags::HalfCarry, (low_nibble_result & 0x10) != 0); }

	const uint16_t result = registers.Read(Register8bit::A) - value;

	registers.SetFlag(Flags::Carry, (result & 0x0100) != 0);
	registers.SetFlag(Flags::Zero, (result & 0xFF) == 0);

	return Clock(2, 7);
}

// CP r
// (Z80 p.161)
Clock Z80::Compare(Register8bit reg)
{
	Compare(registers.Read(reg));
	return Clock(1, 4);
}

// CP (rr)
// (Z80 p.161)
Clock Z80::Compare(Register16bit source_addr)
{
	Compare(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// INC r
// (Z80 p.163)
Clock Z80::Increment(Register8bit reg)
{
	registers.SetFlag(Flags::Subtract, false);
	// Carry is unaffected
	{const uint8_t low_nibble = registers.Read(reg) & 0x0F;
	registers.SetFlag(Flags::HalfCarry, low_nibble == 0x0F); }
	
	const uint8_t result = registers.Read(reg) + 1;
	registers.SetFlag(Flags::Zero, result == 0);

	registers.Write(reg, result);

	return Clock(1, 4);
}

// INC (rr)
// (Z80 p.165)
Clock Z80::IncrementInAddress(Register16bit reg_addr)
{
	registers.SetFlag(Flags::Subtract, false);
	// Carry is unaffected
	{const uint8_t low_nibble = mmu.Read8bitFromMemory(registers.Read(reg_addr)) & 0x0F;
	registers.SetFlag(Flags::HalfCarry, low_nibble == 0x0F); }

	const uint8_t result = mmu.Read8bitFromMemory(registers.Read(reg_addr)) + 1;

	registers.SetFlag(Flags::Zero, result == 0);

	mmu.Write8bitToMemory(registers.Read(reg_addr), result);

	return Clock(3, 11);
}

// DEC r
// (Z80 p.168)
Clock Z80::Decrement(Register8bit reg)
{
	registers.SetFlag(Flags::Subtract, true);
	// Carry is unaffected
	{const uint8_t low_nibble = registers.Read(reg) & 0x0F;
	registers.SetFlag(Flags::HalfCarry, low_nibble == 0); }

	const uint8_t result = registers.Read(reg) - 1;

	registers.SetFlag(Flags::Zero, result == 0);

	registers.Write(reg, result);

	return Clock(1, 4);
}

// DEC (rr)
// (Z80 p.168)
Clock Z80::DecrementInAddress(Register16bit reg_addr)
{
	registers.SetFlag(Flags::Subtract, true);
	// Carry is unaffected
	{const uint8_t low_nibble = mmu.Read8bitFromMemory(registers.Read(reg_addr)) & 0x0F;
	registers.SetFlag(Flags::HalfCarry, low_nibble == 0); }

	const uint8_t result = mmu.Read8bitFromMemory(registers.Read(reg_addr)) - 1;

	registers.SetFlag(Flags::Zero, result == 0);

	mmu.Write8bitToMemory(registers.Read(reg_addr), result);

	return Clock(3, 11);
}
