#include "Z80.h"

// BIT n, r
// (Z80 p.241)
Clock Z80::TestBit(uint8_t bit_index, Register8bit reg)
{
	if (bit_index >= 8)
	{
		throw std::logic_error("TestBit called with invalid bit index");
	}

	registers_.SetFlag(Flags::HalfCarry, true);
	registers_.SetFlag(Flags::Subtract, false);
	// C is not affected

	registers_.SetFlag(Flags::Zero, (registers_.Read(reg) & (1 << bit_index)) == 0);

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

	registers_.SetFlag(Flags::HalfCarry, true);
	registers_.SetFlag(Flags::Subtract, false);
	// C is not affected

	registers_.SetFlag(Flags::Zero, (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & (1 << bit_index)) == 0);

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

	registers_.Write(reg, registers_.Read(reg) | (1 << bit_index));

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

	mmu_.Write8bitToMemory(registers_.Read(reg_addr), mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) | (1 << bit_index));

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

	registers_.Write(reg, registers_.Read(reg) & ~(1 << bit_index));

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

	mmu_.Write8bitToMemory(registers_.Read(reg_addr), mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & ~(1 << bit_index));

	return Clock(4, 15);
}
