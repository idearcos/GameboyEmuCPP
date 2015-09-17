#include "Z80.h"

// LDI (rr), r'
Clock Z80::LoadAddressAndIncrement(Register16bit dest_addr, Register8bit source)
{
	mmu_.Write8bitToMemory(registers_.Read(dest_addr), registers_.Read(source));
	registers_.Write(dest_addr, registers_.Read(dest_addr) + 1);
	return Clock(2, 8);
}

// LDD (rr), r'
Clock Z80::LoadAddressAndDecrement(Register16bit dest_addr, Register8bit source)
{
	mmu_.Write8bitToMemory(registers_.Read(dest_addr), registers_.Read(source));
	registers_.Write(dest_addr, registers_.Read(dest_addr) - 1);
	return Clock(2, 8);
}

// LDI r, (rr')
Clock Z80::LoadRegisterAndIncrement(Register8bit dest, Register16bit source_addr)
{
	registers_.Write(dest, mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	registers_.Write(source_addr, registers_.Read(source_addr) + 1);
	return Clock(2, 8);
}

// LDD r, (rr')
Clock Z80::LoadRegisterAndDecrement(Register8bit dest, Register16bit source_addr)
{
	registers_.Write(dest, mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	registers_.Write(source_addr, registers_.Read(source_addr) - 1);
	return Clock(2, 8);
}

// SWAP r
Clock Z80::Swap(Register8bit reg)
{
	registers_.SetFlag(Flags::Zero, registers_.Read(reg) == 0);
	registers_.SetFlag(Flags::Subtract, false);
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Carry, false);

	registers_.Write(reg, (registers_.Read(reg) << 4) | (registers_.Read(reg) >> 4));

	return Clock(2, 8);
}

// SWAP (rr)
Clock Z80::Swap(Register16bit reg_address)
{
	const auto original_value = mmu_.Read8bitFromMemory(registers_.Read(reg_address));
	registers_.SetFlag(Flags::Zero, original_value == 0);
	registers_.SetFlag(Flags::Subtract, false);
	registers_.SetFlag(Flags::HalfCarry, false);
	registers_.SetFlag(Flags::Carry, false);

	mmu_.Write8bitToMemory(registers_.Read(reg_address), (original_value << 4) | (original_value >> 4));

	return Clock(4, 16);
}

// LD IO r, n
Clock Z80::LoadRegisterFromIO(Register8bit dest, uint8_t displacement)
{
	registers_.Write(dest, mmu_.Read8bitFromMemory(0xFF00 + displacement));

	return Clock(3, 12);
}

// LD IO r, r'
Clock Z80::LoadRegisterFromIO(Register8bit dest, Register8bit reg_displacement)
{
	registers_.Write(dest, mmu_.Read8bitFromMemory(0xFF00 + registers_.Read(reg_displacement)));

	return Clock(2, 8);
}
