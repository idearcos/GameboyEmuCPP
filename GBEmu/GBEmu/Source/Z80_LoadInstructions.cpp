#include "Z80.h"

#pragma region 8-bit load group
// LD r, r'
// (Z80 p.69)
Clock Z80::LoadRegister(Register8bit dest, Register8bit source)
{
	// Flags not affected
	registers_.Write(dest, registers_.Read(source));
	return Clock(1, 4);
}

// LD r, n
// (Z80 p.70)
Clock Z80::LoadRegister(Register8bit dest, uint8_t value)
{
	// Flags not affected
	registers_.Write(dest, value);
	return Clock(2, 8);
}

// LD r, (rr')
// (Z80 p.72)
Clock Z80::LoadRegisterFromAddress(Register8bit dest, Register16bit source_addr)
{
	// Flags not affected
	registers_.Write(dest, mmu_.Read8bitFromMemory(registers_.Read(source_addr)));
	return Clock(2, 8);
}

// LD (rr), r'
// (Z80 p.77)
Clock Z80::LoadAddressFromRegister(Register16bit dest_addr, Register8bit source)
{
	// Flags not affected
	mmu_.Write8bitToMemory(registers_.Read(dest_addr), registers_.Read(source));
	return Clock(2, 8);
}

// LD (rr), n
// (Z80 p.83)
Clock Z80::LoadAddress(Register16bit dest_addr, uint8_t value)
{
	// Flags not affected
	mmu_.Write8bitToMemory(registers_.Read(dest_addr), value);
	return Clock(3, 12);
}

// LD r, (nn)
// (Z80 p.88)
Clock Z80::LoadRegisterFromAddress(Register8bit dest, uint16_t addr)
{
	// Flags not affected
	registers_.Write(dest, mmu_.Read8bitFromMemory(addr));
	return Clock(4, 16);
}

// LD (nn), r
// (Z80 p.91)
Clock Z80::LoadAddressFromRegister(uint16_t addr, Register8bit source)
{
	mmu_.Write8bitToMemory(addr, registers_.Read(source));
	return Clock(4, 16);
}
#pragma endregion

#pragma region 16-bit load group
// LD rr, nn
// (Z80 p.97)
Clock Z80::LoadRegister(Register16bit dest, uint16_t value)
{
	// Flags not affected
	registers_.Write(dest, value);
	return Clock(3, 12);
}

// LD (nn), rr
Clock Z80::LoadAddressFromRegister(uint16_t addr, Register16bit source)
{
	// Flags not affected
	mmu_.Write16bitToMemory(addr, registers_.Read(source));
	return Clock(5, 20);
}

// LD rr, rr'
// (Z80 p.110)
Clock Z80::LoadRegister(Register16bit dest, Register16bit source)
{
	// Flags not affected
	registers_.Write(dest, registers_.Read(source));
	return Clock(2, 8);
}

// PUSH qq
// (Z80 p.113)
Clock Z80::PushToStack(Register16bit source)
{
	// Flags not affected
	registers_.Decrement(Register16bit::SP);
	registers_.Decrement(Register16bit::SP);
	mmu_.Write16bitToMemory(registers_.Read(Register16bit::SP), registers_.Read(source));

	return Clock(4, 16);
}

// POP qq
// (Z80 p.117)
Clock Z80::PopFromStack(Register16bit dest)
{
	// Flags not affected
	uint16_t value{ mmu_.Read16bitFromMemory(registers_.Read(Register16bit::SP)) };
	registers_.Increment(Register16bit::SP);
	registers_.Increment(Register16bit::SP);
	registers_.Write(dest, value);

	return Clock(3, 12);
}
#pragma endregion
