#include "Z80.h"

// RLCA
// (Z80 p.203)
Clock Z80::Rlca()
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);
	// Z not affected

	const bool bit_7_set = (registers_.Read(Register8bit::A) & 0x80) != 0;
	const uint8_t rotated_acc = (registers_.Read(Register8bit::A) << 1) | (bit_7_set ? 1 : 0);
	
	registers_.SetFlag(Flags::Carry, bit_7_set);
	registers_.Write(Register8bit::A, rotated_acc);

	return Clock(1, 4);
}

// RLA
// (Z80 p.205)
Clock Z80::Rla()
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);
	// Z not affected

	const bool bit_7_set = (registers_.Read(Register8bit::A) & 0x80) != 0;
	const uint8_t rotated_acc = (registers_.Read(Register8bit::A) << 1) | (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);

	registers_.SetFlag(Flags::Carry, bit_7_set);
	registers_.Write(Register8bit::A, rotated_acc);

	return Clock(1, 4);
}

// RRCA
// (Z80 p.207)
Clock Z80::Rrca()
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);
	// Z not affected

	const bool bit_0_set = (registers_.Read(Register8bit::A) & 1) != 0;
	const uint8_t rotated_acc = (registers_.Read(Register8bit::A) >> 1) | (bit_0_set ? 0x80 : 0);

	registers_.SetFlag(Flags::Carry, bit_0_set);
	registers_.Write(Register8bit::A, rotated_acc);

	return Clock(1, 4);
}

// RRA
// (Z80 p.209)
Clock Z80::Rra()
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);
	// Z not affected

	const bool bit_0_set = (registers_.Read(Register8bit::A) & 1) != 0;
	const uint8_t rotated_acc = (registers_.Read(Register8bit::A) >> 1) | (registers_.IsFlagSet(Flags::Carry) ? 0x80 : 0);

	registers_.SetFlag(Flags::Carry, bit_0_set);
	registers_.Write(Register8bit::A, rotated_acc);

	return Clock(1, 4);
}

// RLC r
// (Z80 p.211)
Clock Z80::Rlc(Register8bit reg)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_7_set = (registers_.Read(reg) & 0x80) != 0;
	const uint8_t rotated_acc = (registers_.Read(reg) << 1) | (bit_7_set ? 1 : 0);

	registers_.SetFlag(Flags::Carry, bit_7_set);
	registers_.Write(reg, rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(2, 8);
}

// RLC (rr)
// (Z80 p.213)
Clock Z80::Rlc(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_7_set = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 0x80) != 0;
	const uint8_t rotated_acc = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) << 1) | (bit_7_set ? 1 : 0);

	registers_.SetFlag(Flags::Carry, bit_7_set);
	mmu_.Write8bitToMemory(registers_.Read(reg_addr), rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(4, 15);
}

// RL r
// (Z80 p.219)
Clock Z80::Rl(Register8bit reg)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_7_set = (registers_.Read(reg) & 0x80) != 0;
	const uint8_t rotated_acc = (registers_.Read(reg) << 1) | (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);

	registers_.SetFlag(Flags::Carry, bit_7_set);
	registers_.Write(reg, rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(2, 8);
}

// RL (rr)
// (Z80 p.219)
Clock Z80::Rl(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_7_set = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 0x80) != 0;
	const uint8_t rotated_acc = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) << 1) | (registers_.IsFlagSet(Flags::Carry) ? 1 : 0);

	registers_.SetFlag(Flags::Carry, bit_7_set);
	mmu_.Write8bitToMemory(registers_.Read(reg_addr), rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(4, 15);
}

// RRC r
// (Z80 p.222)
Clock Z80::Rrc(Register8bit reg)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_0_set = (registers_.Read(reg) & 1) != 0;
	const uint8_t rotated_acc = (registers_.Read(reg) >> 1) | (bit_0_set ? 0x80 : 0);

	registers_.SetFlag(Flags::Carry, bit_0_set);
	registers_.Write(reg, rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(2, 8);
}

// RRC (rr)
// (Z80 p.222)
Clock Z80::Rrc(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_0_set = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 1) != 0;
	const uint8_t rotated_acc = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) >> 1) | (bit_0_set ? 0x80 : 0);

	registers_.SetFlag(Flags::Carry, bit_0_set);
	mmu_.Write8bitToMemory(registers_.Read(reg_addr), rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(4, 15);
}

// RR r
// (Z80 p.225)
Clock Z80::Rr(Register8bit reg)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_0_set = (registers_.Read(reg) & 1) != 0;
	const uint8_t rotated_acc = (registers_.Read(reg) >> 1) | (registers_.IsFlagSet(Flags::Carry) ? 0x80 : 0);

	registers_.SetFlag(Flags::Carry, bit_0_set);
	registers_.Write(reg, rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(2, 8);
}

// RR (rr)
// (Z80 p.225)
Clock Z80::Rr(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_0_set = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 1) != 0;
	const uint8_t rotated_acc = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) >> 1) | (registers_.IsFlagSet(Flags::Carry) ? 0x80 : 0);

	registers_.SetFlag(Flags::Carry, bit_0_set);
	mmu_.Write8bitToMemory(registers_.Read(reg_addr), rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(4, 15);
}

// SLA r
// (Z80 p.228)
Clock Z80::Sla(Register8bit reg)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_7_set = (registers_.Read(reg) & 0x80) != 0;
	const uint8_t rotated_acc = registers_.Read(reg) << 1;

	registers_.SetFlag(Flags::Carry, bit_7_set);
	registers_.Write(reg, rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(2, 8);
}

// SLA (rr)
// (Z80 p.228)
Clock Z80::Sla(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_7_set = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 0x80) != 0;
	const uint8_t rotated_acc = mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) << 1;

	registers_.SetFlag(Flags::Carry, bit_7_set);
	mmu_.Write8bitToMemory(registers_.Read(reg_addr), rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(4, 15);
}

// SRA r
// (Z80 p.231)
Clock Z80::Sra(Register8bit reg)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_0_set = (registers_.Read(reg) & 1) != 0;
	const bool bit_7_set = (registers_.Read(reg) & 0x80) != 0;
	const uint8_t rotated_acc = (registers_.Read(reg) >> 1) | (bit_7_set ? 0x80 : 0);

	registers_.SetFlag(Flags::Carry, bit_0_set);
	registers_.Write(reg, rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(2, 8);
}

// SRA (rr)
// (Z80 p.231)
Clock Z80::Sra(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_0_set = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 1) != 0;
	const bool bit_7_set = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 0x80) != 0;
	const uint8_t rotated_acc = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) >> 1) | (bit_7_set ? 0x80 : 0);

	registers_.SetFlag(Flags::Carry, bit_0_set);
	mmu_.Write8bitToMemory(registers_.Read(reg_addr), rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(4, 15);
}

// SRL r
// (Z80 p.234)
Clock Z80::Srl(Register8bit reg)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_0_set = (registers_.Read(reg) & 1) != 0;
	const uint8_t rotated_acc = registers_.Read(reg) >> 1;

	registers_.SetFlag(Flags::Carry, bit_0_set);
	registers_.Write(reg, rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(2, 8);
}

// SRL (rr)
// (Z80 p.234)
Clock Z80::Srl(Register16bit reg_addr)
{
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Subtract, false);

	const bool bit_0_set = (mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) & 1) != 0;
	const uint8_t rotated_acc = mmu_.Read8bitFromMemory(registers_.Read(reg_addr)) >> 1;

	registers_.SetFlag(Flags::Carry, bit_0_set);
	mmu_.Write8bitToMemory(registers_.Read(reg_addr), rotated_acc);
	registers_.SetFlag(Flags::Zero, rotated_acc == 0);

	return Clock(4, 15);
}
