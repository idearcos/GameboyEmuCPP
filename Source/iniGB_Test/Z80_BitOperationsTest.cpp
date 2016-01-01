#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void TestBitOperation(uint8_t opcode, uint8_t cb_opcode, Register8bit reg, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry = false)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	expected_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), expected_flags.begin(), expected_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	if (set_carry)
	{
		z80.GetRegisters().SetFlag(Flags::Carry, true);
	}
	z80.GetRegisters().Write(reg, value);
	mmu.Write8bitToMemory(0, cb_opcode);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(reg)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(reg)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(1, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	for (const auto& flag : expected_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	}
}

void TestBitOperation(uint8_t opcode, uint8_t cb_opcode, Register16bit address_reg, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry = false)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	expected_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), expected_flags.begin(), expected_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	if (set_carry)
	{
		z80.GetRegisters().SetFlag(Flags::Carry, true);
	}
	z80.GetRegisters().Write(address_reg, 0x1234);
	mmu.Write8bitToMemory(0x1234, value);
	mmu.Write8bitToMemory(0, cb_opcode);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, mmu.Read8bitFromMemory(0x1234)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(0x1234)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(1, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(Clock(4, 16), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	for (const auto& flag : expected_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	}
}

#pragma region BIT
TEST(Z80_BitOperationsTest, BitRegA)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x47 + (i * 8), Register8bit::A, 0, 0, { Flags::HalfCarry, Flags::Zero, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x47 + (i * 8), Register8bit::A, 0xFF, 0xFF, { Flags::HalfCarry, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x47 + (i * 8), Register8bit::A, 1 << i, 1 << i, { Flags::HalfCarry }, false);
		TestBitOperation(0xCB, 0x47 + (i * 8), Register8bit::A, ~(1 << i), ~(1 << i), { Flags::HalfCarry, Flags::Zero }, false);
	}
}

TEST(Z80_BitOperationsTest, BitRegB)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x40 + (i * 8), Register8bit::B, 0, 0, { Flags::HalfCarry, Flags::Zero, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x40 + (i * 8), Register8bit::B, 0xFF, 0xFF, { Flags::HalfCarry, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x40 + (i * 8), Register8bit::B, 1 << i, 1 << i, { Flags::HalfCarry }, false);
		TestBitOperation(0xCB, 0x40 + (i * 8), Register8bit::B, ~(1 << i), ~(1 << i), { Flags::HalfCarry, Flags::Zero }, false);
	}
}

TEST(Z80_BitOperationsTest, BitRegC)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x41 + (i * 8), Register8bit::C, 0, 0, { Flags::HalfCarry, Flags::Zero, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x41 + (i * 8), Register8bit::C, 0xFF, 0xFF, { Flags::HalfCarry, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x41 + (i * 8), Register8bit::C, 1 << i, 1 << i, { Flags::HalfCarry }, false);
		TestBitOperation(0xCB, 0x41 + (i * 8), Register8bit::C, ~(1 << i), ~(1 << i), { Flags::HalfCarry, Flags::Zero }, false);
	}
}

TEST(Z80_BitOperationsTest, BitRegD)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x42 + (i * 8), Register8bit::D, 0, 0, { Flags::HalfCarry, Flags::Zero, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x42 + (i * 8), Register8bit::D, 0xFF, 0xFF, { Flags::HalfCarry, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x42 + (i * 8), Register8bit::D, 1 << i, 1 << i, { Flags::HalfCarry }, false);
		TestBitOperation(0xCB, 0x42 + (i * 8), Register8bit::D, ~(1 << i), ~(1 << i), { Flags::HalfCarry, Flags::Zero }, false);
	}
}

TEST(Z80_BitOperationsTest, BitRegE)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x43 + (i * 8), Register8bit::E, 0, 0, { Flags::HalfCarry, Flags::Zero, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x43 + (i * 8), Register8bit::E, 0xFF, 0xFF, { Flags::HalfCarry, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x43 + (i * 8), Register8bit::E, 1 << i, 1 << i, { Flags::HalfCarry }, false);
		TestBitOperation(0xCB, 0x43 + (i * 8), Register8bit::E, ~(1 << i), ~(1 << i), { Flags::HalfCarry, Flags::Zero }, false);
	}
}

TEST(Z80_BitOperationsTest, BitRegH)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x44 + (i * 8), Register8bit::H, 0, 0, { Flags::HalfCarry, Flags::Zero, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x44 + (i * 8), Register8bit::H, 0xFF, 0xFF, { Flags::HalfCarry, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x44 + (i * 8), Register8bit::H, 1 << i, 1 << i, { Flags::HalfCarry }, false);
		TestBitOperation(0xCB, 0x44 + (i * 8), Register8bit::H, ~(1 << i), ~(1 << i), { Flags::HalfCarry, Flags::Zero }, false);
	}
}

TEST(Z80_BitOperationsTest, BitRegL)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x45 + (i * 8), Register8bit::L, 0, 0, { Flags::HalfCarry, Flags::Zero, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x45 + (i * 8), Register8bit::L, 0xFF, 0xFF, { Flags::HalfCarry, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x45 + (i * 8), Register8bit::L, 1 << i, 1 << i, { Flags::HalfCarry }, false);
		TestBitOperation(0xCB, 0x45 + (i * 8), Register8bit::L, ~(1 << i), ~(1 << i), { Flags::HalfCarry, Flags::Zero }, false);
	}
}

TEST(Z80_BitOperationsTest, BitAddressInRegHL)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x46 + (i * 8), Register16bit::HL, 0, 0, { Flags::HalfCarry, Flags::Zero, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x46 + (i * 8), Register16bit::HL, 0xFF, 0xFF, { Flags::HalfCarry, Flags::Carry }, true);
		TestBitOperation(0xCB, 0x46 + (i * 8), Register16bit::HL, 1 << i, 1 << i, { Flags::HalfCarry }, false);
		TestBitOperation(0xCB, 0x46 + (i * 8), Register16bit::HL, ~(1 << i), ~(1 << i), { Flags::HalfCarry, Flags::Zero }, false);
	}
}
#pragma endregion

#pragma region SET
TEST(Z80_BitOperationsTest, SetRegA)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0xC7 + (i * 8), Register8bit::A, 0, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC7 + (i * 8), Register8bit::A, 1 << i, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC7 + (i * 8), Register8bit::A, 0xFF, 0xFF, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC7 + (i * 8), Register8bit::A, ~(1 << i), 0xFF, {}, false);
	}
}

TEST(Z80_BitOperationsTest, SetRegB)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0xC0 + (i * 8), Register8bit::B, 0, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC0 + (i * 8), Register8bit::B, 1 << i, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC0 + (i * 8), Register8bit::B, 0xFF, 0xFF, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC0 + (i * 8), Register8bit::B, ~(1 << i), 0xFF, {}, false);
	}
}

TEST(Z80_BitOperationsTest, SetRegC)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0xC1 + (i * 8), Register8bit::C, 0, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC1 + (i * 8), Register8bit::C, 1 << i, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC1 + (i * 8), Register8bit::C, 0xFF, 0xFF, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC1 + (i * 8), Register8bit::C, ~(1 << i), 0xFF, {}, false);
	}
}

TEST(Z80_BitOperationsTest, SetRegD)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0xC2 + (i * 8), Register8bit::D, 0, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC2 + (i * 8), Register8bit::D, 1 << i, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC2 + (i * 8), Register8bit::D, 0xFF, 0xFF, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC2 + (i * 8), Register8bit::D, ~(1 << i), 0xFF, {}, false);
	}
}

TEST(Z80_BitOperationsTest, SetRegE)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0xC3 + (i * 8), Register8bit::E, 0, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC3 + (i * 8), Register8bit::E, 1 << i, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC3 + (i * 8), Register8bit::E, 0xFF, 0xFF, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC3 + (i * 8), Register8bit::E, ~(1 << i), 0xFF, {}, false);
	}
}

TEST(Z80_BitOperationsTest, SetRegH)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0xC4 + (i * 8), Register8bit::H, 0, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC4 + (i * 8), Register8bit::H, 1 << i, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC4 + (i * 8), Register8bit::H, 0xFF, 0xFF, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC4 + (i * 8), Register8bit::H, ~(1 << i), 0xFF, {}, false);
	}
}

TEST(Z80_BitOperationsTest, SetRegL)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0xC5 + (i * 8), Register8bit::L, 0, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC5 + (i * 8), Register8bit::L, 1 << i, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC5 + (i * 8), Register8bit::L, 0xFF, 0xFF, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC5 + (i * 8), Register8bit::L, ~(1 << i), 0xFF, {}, false);
	}
}

TEST(Z80_BitOperationsTest, SetAddressInRegHL)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0xC6 + (i * 8), Register16bit::HL, 0, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC6 + (i * 8), Register16bit::HL, 1 << i, 1 << i, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC6 + (i * 8), Register16bit::HL, 0xFF, 0xFF, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0xC6 + (i * 8), Register16bit::HL, ~(1 << i), 0xFF, {}, false);
	}
}
#pragma endregion

#pragma region RES
TEST(Z80_BitOperationsTest, ResRegA)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x87 + (i * 8), Register8bit::A, 0, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x87 + (i * 8), Register8bit::A, 1 << i, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x87 + (i * 8), Register8bit::A, ~(1 << i), ~(1 << i), { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x87 + (i * 8), Register8bit::A, 0xFF, ~(1 << i), {}, false);
	}
}

TEST(Z80_BitOperationsTest, ResRegB)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x80 + (i * 8), Register8bit::B, 0, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x80 + (i * 8), Register8bit::B, 1 << i, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x80 + (i * 8), Register8bit::B, ~(1 << i), ~(1 << i), { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x80 + (i * 8), Register8bit::B, 0xFF, ~(1 << i), {}, false);
	}
}

TEST(Z80_BitOperationsTest, ResRegC)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x81 + (i * 8), Register8bit::C, 0, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x81 + (i * 8), Register8bit::C, 1 << i, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x81 + (i * 8), Register8bit::C, ~(1 << i), ~(1 << i), { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x81 + (i * 8), Register8bit::C, 0xFF, ~(1 << i), {}, false);
	}
}

TEST(Z80_BitOperationsTest, ResRegD)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x82 + (i * 8), Register8bit::D, 0, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x82 + (i * 8), Register8bit::D, 1 << i, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x82 + (i * 8), Register8bit::D, ~(1 << i), ~(1 << i), { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x82 + (i * 8), Register8bit::D, 0xFF, ~(1 << i), {}, false);
	}
}

TEST(Z80_BitOperationsTest, ResRegE)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x83 + (i * 8), Register8bit::E, 0, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x83 + (i * 8), Register8bit::E, 1 << i, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x83 + (i * 8), Register8bit::E, ~(1 << i), ~(1 << i), { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x83 + (i * 8), Register8bit::E, 0xFF, ~(1 << i), {}, false);
	}
}

TEST(Z80_BitOperationsTest, ResRegH)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x84 + (i * 8), Register8bit::H, 0, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x84 + (i * 8), Register8bit::H, 1 << i, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x84 + (i * 8), Register8bit::H, ~(1 << i), ~(1 << i), { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x84 + (i * 8), Register8bit::H, 0xFF, ~(1 << i), {}, false);
	}
}

TEST(Z80_BitOperationsTest, ResRegL)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x85 + (i * 8), Register8bit::L, 0, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x85 + (i * 8), Register8bit::L, 1 << i, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x85 + (i * 8), Register8bit::L, ~(1 << i), ~(1 << i), { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x85 + (i * 8), Register8bit::L, 0xFF, ~(1 << i), {}, false);
	}
}

TEST(Z80_BitOperationsTest, ResAddressInRegHL)
{
	// Consider Zero flag unaffected
	for (auto i = 0; i < 8; i++)
	{
		TestBitOperation(0xCB, 0x86 + (i * 8), Register16bit::HL, 0, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x86 + (i * 8), Register16bit::HL, 1 << i, 0, { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x86 + (i * 8), Register16bit::HL, ~(1 << i), ~(1 << i), { Flags::Carry }, true);
		TestBitOperation(0xCB, 0x86 + (i * 8), Register16bit::HL, 0xFF, ~(1 << i), {}, false);
	}
}
#pragma endregion
