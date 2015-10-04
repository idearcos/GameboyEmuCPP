#include "Z80.h"

// LDI (rr), r'
Clock Z80::LoadAddressAndIncrement(Register16bit dest_addr, Register8bit source)
{
	WriteToMmu(registers_.Read(dest_addr), registers_.Read(source));
	registers_.Write(dest_addr, registers_.Read(dest_addr) + 1);
	return Clock(2, 8);
}

// LDD (rr), r'
Clock Z80::LoadAddressAndDecrement(Register16bit dest_addr, Register8bit source)
{
	WriteToMmu(registers_.Read(dest_addr), registers_.Read(source));
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

	WriteToMmu(registers_.Read(reg_address), static_cast<uint8_t>((original_value << 4) | (original_value >> 4)));

	return Clock(4, 16);
}

// LDIO (n), r
Clock Z80::LoadIOFromRegister(uint8_t displacement, Register8bit source)
{
	WriteToMmu(0xFF00 + displacement, registers_.Read(source));

	return Clock(3, 12);
}

// LDIO (r), r'
Clock Z80::LoadIOFromRegister(Register8bit reg_displacement, Register8bit source)
{
	WriteToMmu(0xFF00 + registers_.Read(reg_displacement), registers_.Read(source));

	return Clock(3, 12);
}

// LDIO r, (n)
Clock Z80::LoadRegisterFromIO(Register8bit dest, uint8_t displacement)
{
	registers_.Write(dest, mmu_.Read8bitFromMemory(0xFF00 + displacement));

	return Clock(3, 12);
}

// LDIO r, (r')
Clock Z80::LoadRegisterFromIO(Register8bit dest, Register8bit reg_displacement)
{
	registers_.Write(dest, mmu_.Read8bitFromMemory(0xFF00 + registers_.Read(reg_displacement)));

	return Clock(2, 8);
}

// LD rr, rr'+n
Clock Z80::LoadRegisterFromAddress(Register16bit dest, Register16bit source_addr, int8_t displacement)
{
	registers_.SetFlag(Flags::Zero, false);
	registers_.SetFlag(Flags::Subtract, false);
	//TODO how are carry and half carry affected?
	//registers_.SetFlag(Flags::HalfCarry, false);
	//registers_.SetFlag(Flags::Carry, false);

	registers_.Write(dest, mmu_.Read16bitFromMemory(registers_.Read(source_addr) + displacement));

	return Clock(3, 12);
}

// ADD rr, n
Clock Z80::Add(Register16bit dest, int8_t value)
{
	registers_.SetFlag(Flags::Zero, false);
	registers_.SetFlag(Flags::Subtract, false);

	{const uint16_t low_12_bits_result = (registers_.Read(dest) & 0x0FFF) + value;
	registers_.SetFlag(Flags::HalfCarry, (low_12_bits_result & 0x1000) != 0); }

	const uint32_t result = registers_.Read(dest) + value;
	registers_.SetFlag(Flags::Carry, (result & 0x10000) != 0);

	registers_.Write(dest, static_cast<uint16_t>(result & 0xFFFF));

	return Clock(4, 16);
}
