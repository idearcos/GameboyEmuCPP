#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void TestJumpToImmediateAddress(uint8_t opcode, uint16_t address, uint16_t expected_pc_value, Clock &&expected_time, std::list<Flags> &&set_flags)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	set_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), set_flags.begin(), set_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	for (const auto& flag : set_flags)
	{
		z80.GetRegisters().SetFlag(flag, true);
	}
	mmu.Write16bitToMemory(0, address);
	z80.Execute(opcode);

	ASSERT_EQ(expected_pc_value, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(address) << ")";
	// Cycles information extracted from GameBoyProgrammingManual.pdf
	ASSERT_EQ(expected_time, z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(address) << ")";
	for (const auto& flag : set_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(address) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(address) << ")";
	}
}

TEST(Z80_JumpTest, JumpToImmediateAddress)
{
	TestJumpToImmediateAddress(0xC3, 0x0123, 0x0123, Clock(4, 16), { Flags::Zero });
	TestJumpToImmediateAddress(0xC3, 0x4567, 0x4567, Clock(4, 16), { Flags::Subtract });
	TestJumpToImmediateAddress(0xC3, 0x89AB, 0x89AB, Clock(4, 16), { Flags::HalfCarry });
	TestJumpToImmediateAddress(0xC3, 0xCDEF, 0xCDEF, Clock(4, 16), { Flags::Carry });
}

TEST(Z80_JumpTest, JumpToImmediateAddressIfNonZero)
{
	TestJumpToImmediateAddress(0xC2, 0x0123, 0x0123, Clock(4, 16), {});
	TestJumpToImmediateAddress(0xC2, 0x0123, 0x0123, Clock(4, 16), { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestJumpToImmediateAddress(0xC2, 0x0123, 0x0002, Clock(3, 12), { Flags::Zero });
	TestJumpToImmediateAddress(0xC2, 0x0123, 0x0002, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_JumpTest, JumpToImmediateAddressIfZero)
{
	TestJumpToImmediateAddress(0xCA, 0x4567, 0x4567, Clock(4, 16), { Flags::Zero });
	TestJumpToImmediateAddress(0xCA, 0x4567, 0x4567, Clock(4, 16), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestJumpToImmediateAddress(0xCA, 0x4567, 0x0002, Clock(3, 12), {});
	TestJumpToImmediateAddress(0xCA, 0x4567, 0x0002, Clock(3, 12), { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_JumpTest, JumpToImmediateAddressIfNonCarry)
{
	TestJumpToImmediateAddress(0xD2, 0x89AB, 0x89AB, Clock(4, 16), {});
	TestJumpToImmediateAddress(0xD2, 0x89AB, 0x89AB, Clock(4, 16), { Flags::Zero, Flags::Subtract, Flags::HalfCarry });
	TestJumpToImmediateAddress(0xD2, 0x89AB, 0x0002, Clock(3, 12), { Flags::Carry });
	TestJumpToImmediateAddress(0xD2, 0x89AB, 0x0002, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_JumpTest, JumpToImmediateAddressIfCarry)
{
	TestJumpToImmediateAddress(0xDA, 0xCDEF, 0xCDEF, Clock(4, 16), { Flags::Carry });
	TestJumpToImmediateAddress(0xDA, 0xCDEF, 0xCDEF, Clock(4, 16), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestJumpToImmediateAddress(0xDA, 0xCDEF, 0x0002, Clock(3, 12), {});
	TestJumpToImmediateAddress(0xDA, 0xCDEF, 0x0002, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry });
}

void TestJumpToAddressInRegHL(uint8_t opcode, uint16_t address, std::list<Flags> &&set_flags)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	set_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), set_flags.begin(), set_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	for (const auto& flag : set_flags)
	{
		z80.GetRegisters().SetFlag(flag, true);
	}
	z80.GetRegisters().Write(Register16bit::HL, address);
	z80.Execute(opcode);

	ASSERT_EQ(address, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(address) << ")";
	// Cycles information extracted from GameBoyProgrammingManual.pdf
	ASSERT_EQ(Clock(1, 4), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(address) << ")";
	for (const auto& flag : set_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(address) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(address) << ")";
	}
}

TEST(Z80_JumpTest, JumpToAddressInRegHL)
{
	TestJumpToAddressInRegHL(0xE9, 0x0123, { Flags::Zero });
	TestJumpToAddressInRegHL(0xE9, 0x4567, { Flags::Subtract });
	TestJumpToAddressInRegHL(0xE9, 0x89AB, { Flags::HalfCarry });
	TestJumpToAddressInRegHL(0xE9, 0xCDEF, { Flags::Carry });
}

void TestJumpToAddressDisplacement(uint8_t opcode, int8_t address_displacement, uint16_t expected_pc_value, Clock &&expected_time, std::list<Flags> &&set_flags)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	set_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), set_flags.begin(), set_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	for (const auto& flag : set_flags)
	{
		z80.GetRegisters().SetFlag(flag, true);
	}
	mmu.Write8bitToMemory(0, address_displacement);
	z80.Execute(opcode);

	ASSERT_EQ(expected_pc_value, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(address_displacement) << ")";
	// Cycles information extracted from GameBoyProgrammingManual.pdf
	ASSERT_EQ(expected_time, z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(address_displacement) << ")";
	for (const auto& flag : set_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(address_displacement) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(address_displacement) << ")";
	}
}

TEST(Z80_JumpTest, JumpToAddressDisplacement)
{
	TestJumpToAddressDisplacement(0x18, 127, 128, Clock(3, 12), { Flags::Zero });
	TestJumpToAddressDisplacement(0x18, -128, -127, Clock(3, 12), { Flags::Subtract });
	TestJumpToAddressDisplacement(0x18, 0, 1, Clock(3, 12), { Flags::HalfCarry });
	TestJumpToAddressDisplacement(0x18, -1, 0, Clock(3, 12), { Flags::Carry });
}

TEST(Z80_JumpTest, JumpToAddressDisplacementIfNonZero)
{
	TestJumpToAddressDisplacement(0x20, 127, 128, Clock(3, 12), {});
	TestJumpToAddressDisplacement(0x20, -128, -127, Clock(3, 12), { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestJumpToAddressDisplacement(0x20, 127, 1, Clock(2, 8), { Flags::Zero });
	TestJumpToAddressDisplacement(0x20, -128, 1, Clock(2, 8), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_JumpTest, JumpToAddressDisplacementIfZero)
{
	TestJumpToAddressDisplacement(0x28, 127, 128, Clock(3, 12), { Flags::Zero });
	TestJumpToAddressDisplacement(0x28, -128, -127, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestJumpToAddressDisplacement(0x28, 127, 1, Clock(2, 8), {});
	TestJumpToAddressDisplacement(0x28, -128, 1, Clock(2, 8), { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_JumpTest, JumpToAddressDisplacementIfNonCarry)
{
	TestJumpToAddressDisplacement(0x30, 127, 128, Clock(3, 12), {});
	TestJumpToAddressDisplacement(0x30, -128, -127, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry });
	TestJumpToAddressDisplacement(0x30, 127, 1, Clock(2, 8), { Flags::Carry });
	TestJumpToAddressDisplacement(0x30, -128, 1, Clock(2, 8), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_JumpTest, JumpToAddressDisplacementIfCarry)
{
	TestJumpToAddressDisplacement(0x38, 127, 128, Clock(3, 12), { Flags::Carry });
	TestJumpToAddressDisplacement(0x38, -128, -127, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestJumpToAddressDisplacement(0x38, 127, 1, Clock(2, 8), {});
	TestJumpToAddressDisplacement(0x38, -128, 1, Clock(2, 8), { Flags::Zero, Flags::Subtract, Flags::HalfCarry });
}
