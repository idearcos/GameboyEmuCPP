#include "Z80.h"

// BIT n, r
// (Z80 p.241)
Clock Z80::TestBit(uint8_t bit_index, Register8bit reg)
{
	if (bit_index >= 8)
	{
		throw std::logic_error("TestBit called with invalid bit index");
	}

	registers.SetFlag(Flags::HalfCarry, true);
	registers.SetFlag(Flags::Subtract, false);
	// C is not affected

	registers.SetFlag(Flags::Zero, (registers.Read(reg) & (1 << bit_index)) == 0);

	return Clock(2, 8);
}

// BIT n, *rr
// (Z80 p.243)
Clock Z80::TestBit(uint8_t bit_index, Register16bit reg_addr)
{
	if (bit_index >= 8)
	{
		throw std::logic_error("TestBit called with invalid bit index");
	}

	registers.SetFlag(Flags::HalfCarry, true);
	registers.SetFlag(Flags::Subtract, false);
	// C is not affected

	registers.SetFlag(Flags::Zero, (mmu.Read8bitFromMemory(registers.Read(reg_addr)) & (1 << bit_index)) == 0);

	return Clock(3, 12);
}

// SET n, r
// (Z80 p.249)
Clock Z80::SetBit(uint8_t bit_index, Register8bit reg)
{
	if (bit_index >= 8)
	{
		throw std::logic_error("SetBit called with invalid bit index");
	}

	// Flags not affected

	registers.Write(reg, registers.Read(reg) | (1 << bit_index));

	return Clock(2, 8);
}

// SET n, *rr
// (Z80 p.251)
Clock Z80::SetBit(uint8_t bit_index, Register16bit reg_addr)
{
	if (bit_index >= 8)
	{
		throw std::logic_error("SetBit called with invalid bit index");
	}

	// Flags not affected

	mmu.Write8bitToMemory(registers.Read(reg_addr), mmu.Read8bitFromMemory(registers.Read(reg_addr)) | (1 << bit_index));

	return Clock(4, 15);
}

// RES n, r
// (Z80 p.256)
Clock Z80::ResetBit(uint8_t bit_index, Register8bit reg)
{
	if (bit_index >= 8)
	{
		throw std::logic_error("ResetBit called with invalid bit index");
	}

	// Flags not affected

	registers.Write(reg, registers.Read(reg) & ~(1 << bit_index));

	return Clock(2, 8);
}

// RES n, *rr
// (Z80 p.241)
Clock Z80::ResetBit(uint8_t bit_index, Register16bit reg_addr)
{
	if (bit_index >= 8)
	{
		throw std::logic_error("ResetBit called with invalid bit index");
	}

	// Flags not affected

	mmu.Write8bitToMemory(registers.Read(reg_addr), mmu.Read8bitFromMemory(registers.Read(reg_addr)) & ~(1 << bit_index));

	return Clock(4, 15);
}
