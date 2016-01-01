#include "Z80.h"
#include <sstream>

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

	return Clock(2, 8);
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
Clock Z80::LoadRegisterFromRegisterPlusDisplacement(Register16bit dest, Register16bit source, int8_t displacement)
{
	registers_.SetFlag(Flags::Zero, false);
	registers_.SetFlag(Flags::Subtract, false);
	
	// Flags are calculated with unsigned value on lower byte of SP
	{const uint16_t low_4_bits_result = (registers_.Read(source) & 0x000F) + (static_cast<uint8_t>(displacement) & 0x000F);
	registers_.SetFlag(Flags::HalfCarry, (low_4_bits_result & 0x0010) != 0); }

	{const uint16_t low_8_bits_result = (registers_.Read(source) & 0x00FF) + static_cast<uint8_t>(displacement);
	registers_.SetFlag(Flags::Carry, (low_8_bits_result & 0x0100) != 0); }

	const uint16_t result = registers_.Read(source) + displacement;

	registers_.Write(dest, result);

	return Clock(3, 12);
}

// ADD rr, n
Clock Z80::Add(Register16bit dest, int8_t value)
{
	registers_.SetFlag(Flags::Zero, false);
	registers_.SetFlag(Flags::Subtract, false);

	// Flags are calculated with unsigned value on lower byte of SP
	{const uint16_t low_4_bits_result = (registers_.Read(dest) & 0x000F) + (static_cast<uint8_t>(value) & 0x000F);
	registers_.SetFlag(Flags::HalfCarry, (low_4_bits_result & 0x0010) != 0); }

	{const uint16_t low_8_bits_result = (registers_.Read(dest) & 0x00FF) + static_cast<uint8_t>(value);
	registers_.SetFlag(Flags::Carry, (low_8_bits_result & 0x0100) != 0); }

	const uint16_t result = registers_.Read(dest) + value;

	registers_.Write(dest, result);

	return Clock(4, 16);
}

// STOP
Clock Z80::Stop(uint8_t next_fetched_byte)
{
	if (0x00 != next_fetched_byte)
	{
		std::stringstream msg;
		msg << "Byte fetched right after STOP instruction is != 0x00 (" << static_cast<size_t>(next_fetched_byte) << ")";
		throw std::runtime_error(msg.str());
	}

	std::cout << "Z80 entering STOP mode" << std::endl;

	state_ = State::Stopped;

	return Clock(1, 4);
}
