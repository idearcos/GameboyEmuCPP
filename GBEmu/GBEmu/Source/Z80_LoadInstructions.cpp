#include "Z80.h"

#pragma region 8-bit load group
// LD r, r'
// (Z80 p.69)
Clock Z80::LoadRegister(Register8bit dest, Register8bit source)
{
	// Flags not affected
	registers.Write(dest, registers.Read(source));
	return Clock(1, 4);
}

// LD r, n
// (Z80 p.70)
Clock Z80::LoadRegister(Register8bit dest, uint8_t value)
{
	// Flags not affected
	registers.Write(dest, value);
	return Clock(2, 7);
}

// LD r, (rr')
// (Z80 p.72)
Clock Z80::LoadRegisterFromAddress(Register8bit dest, Register16bit source_addr)
{
	// Flags not affected
	registers.Write(dest, mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// LD (rr), r'
// (Z80 p.77)
Clock Z80::LoadAddressFromRegister(Register16bit dest_addr, Register8bit source)
{
	// Flags not affected
	mmu.Write8bitToMemory(registers.Read(dest_addr), registers.Read(source));
	return Clock(2, 7);
}

// LD (rr), n
// (Z80 p.83)
Clock Z80::LoadAddress(Register16bit dest_addr, uint8_t value)
{
	// Flags not affected
	mmu.Write8bitToMemory(registers.Read(dest_addr), value);
	return Clock(3, 10);
}

// LD r, (nn)
// (Z80 p.88)
Clock Z80::LoadRegisterFromAddress(Register8bit dest, uint16_t addr)
{
	// Flags not affected
	registers.Write(dest, mmu.Read8bitFromMemory(addr));
	return Clock(4, 13);
}

// LD (nn), r
// (Z80 p.91)
Clock Z80::LoadAddressFromRegister(uint16_t addr, Register8bit source)
{
	mmu.Write8bitToMemory(addr, registers.Read(source));
	return Clock(4, 13);
}
#pragma endregion

#pragma region 16-bit load group
// LD dd, nn
// (Z80 p.97)
Clock Z80::LoadRegister(Register16bit dest, uint16_t value)
{
	// Flags not affected
	registers.Write(dest, value);
	return Clock(2, 10);
}

// LD dd, dd'
// (Z80 p.110)
Clock Z80::LoadRegister(Register16bit dest, Register16bit source)
{
	// Flags not affected
	registers.Write(dest, registers.Read(source));
	return Clock(1, 6);
}

// PUSH qq
// (Z80 p.113)
Clock Z80::PushToStack(Register16bit source)
{
	// Flags not affected
	registers.Decrement(Register16bit::SP);
	mmu.Write8bitToMemory(registers.Read(Register16bit::SP), (registers.Read(source) >> 8) & 0xFF);
	registers.Decrement(Register16bit::SP);
	mmu.Write8bitToMemory(registers.Read(Register16bit::SP), (registers.Read(source) & 0xFF));

	return Clock(3, 11);
}

// POP qq
// (Z80 p.117)
Clock Z80::PopFromStack(Register16bit dest)
{
	// Flags not affected
	uint16_t value{ 0 };
	value += static_cast<uint16_t>(mmu.Read8bitFromMemory(registers.Read(Register16bit::SP))) << 8;
	registers.Increment(Register16bit::SP);
	value += mmu.Read8bitFromMemory(registers.Read(Register16bit::SP));
	registers.Increment(Register16bit::SP);
	registers.Write(dest, value);

	return Clock(3, 10);
}
#pragma endregion

#pragma region GB-specific load instruction group
// LDI (rr), r'
Clock Z80::LoadAddressAndIncrement(Register16bit dest_addr, Register8bit source)
{
	mmu.Write8bitToMemory(registers.Read(dest_addr), registers.Read(source));
	registers.Write(dest_addr, registers.Read(dest_addr) + 1);
	return Clock(2, 8);
}

// LDD (rr), r'
Clock Z80::LoadAddressAndDecrement(Register16bit dest_addr, Register8bit source)
{
	mmu.Write8bitToMemory(registers.Read(dest_addr), registers.Read(source));
	registers.Write(dest_addr, registers.Read(dest_addr) - 1);
	return Clock(2, 8);
}

// LDI r, (rr')
Clock Z80::LoadRegisterAndIncrement(Register8bit dest, Register16bit source_addr)
{
	registers.Write(dest, mmu.Read8bitFromMemory(registers.Read(source_addr)));
	registers.Write(source_addr, registers.Read(source_addr) + 1);
	return Clock(2, 8);
}

// LDD r, (rr')
Clock Z80::LoadRegisterAndDecrement(Register8bit dest, Register16bit source_addr)
{
	registers.Write(dest, mmu.Read8bitFromMemory(registers.Read(source_addr)));
	registers.Write(source_addr, registers.Read(source_addr) - 1);
	return Clock(2, 8);
}
#pragma endregion
