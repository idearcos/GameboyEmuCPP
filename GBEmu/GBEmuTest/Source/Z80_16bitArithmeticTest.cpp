#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void Test16bitRegisterAdd(uint8_t opcode, Register16bit dest, Register16bit source, uint16_t dest_value, uint16_t src_value, uint16_t expected_result, std::list<Flags> &&expected_flags, bool set_zero)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	expected_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), expected_flags.begin(), expected_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().SetFlag(Flags::Zero, set_zero);
	z80.GetRegisters().Write(dest, dest_value);
	z80.GetRegisters().Write(source, src_value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest)) << " (input value: 0x" << std::hex << static_cast<size_t>(src_value) << ")";
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(src_value) << ")";
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(src_value	) << ")";
	for (const auto& flag : expected_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(src_value) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(src_value) << ")";
	}
}

#pragma region ADD HL, rr
TEST(Z80_16bitArithmeticTest, AddRegBCToRegHL)
{
	Test16bitRegisterAdd(0x09, Register16bit::HL, Register16bit::BC, 0x8A23, 0x0605, 0x9028, { Flags::HalfCarry }, false);
	Test16bitRegisterAdd(0x09, Register16bit::HL, Register16bit::BC, 0x8A23, 0x8A23, 0x1446, { Flags::HalfCarry, Flags::Carry, Flags::Zero }, true);
}

TEST(Z80_16bitArithmeticTest, AddRegDEToRegHL)
{
	Test16bitRegisterAdd(0x19, Register16bit::HL, Register16bit::DE, 0x8A23, 0x0605, 0x9028, { Flags::HalfCarry }, false);
	Test16bitRegisterAdd(0x19, Register16bit::HL, Register16bit::DE, 0x8A23, 0x8A23, 0x1446, { Flags::HalfCarry, Flags::Carry, Flags::Zero }, true);
}

TEST(Z80_16bitArithmeticTest, AddRegHLToRegHL)
{
	Test16bitRegisterAdd(0x29, Register16bit::HL, Register16bit::HL, 0x8A23, 0x8A23, 0x1446, { Flags::HalfCarry, Flags::Carry, Flags::Zero }, true);
}

TEST(Z80_16bitArithmeticTest, AddRegSPToRegHL)
{
	Test16bitRegisterAdd(0x39, Register16bit::HL, Register16bit::SP, 0x8A23, 0x0605, 0x9028, { Flags::HalfCarry }, false);
	Test16bitRegisterAdd(0x39, Register16bit::HL, Register16bit::SP, 0x8A23, 0x8A23, 0x1446, { Flags::HalfCarry, Flags::Carry, Flags::Zero }, true);
}
#pragma endregion

void TestAddValToRegSP(uint8_t opcode, Register16bit dest, uint16_t dest_value, uint8_t value, uint16_t expected_result, std::list<Flags> &&expected_flags)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	expected_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), expected_flags.begin(), expected_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(dest, dest_value);
	mmu.Write8bitToMemory(0, value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
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

#pragma region ADD SP, n
TEST(Z80_16bitArithmeticTest, AddValToRegSP)
{
	TestAddValToRegSP(0xE8, Register16bit::SP, 0xFFF8, 2, 0xFFFA, {});
	TestAddValToRegSP(0xE8, Register16bit::SP, 0xFFFF, 1, 0, { Flags::HalfCarry, Flags::Carry });
}
#pragma endregion

void Test16bitRegisterIncDec(uint8_t opcode, Register16bit reg, uint16_t value, uint16_t expected_result, bool set_flags)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };

	TesterMMU mmu;
	Z80 z80(mmu);

	for (const auto& flag : all_flags)
	{
		z80.GetRegisters().SetFlag(flag, set_flags);
	}
	
	z80.GetRegisters().Write(reg, value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(reg)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(reg)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	for (const auto& flag : all_flags)
	{
		ASSERT_EQ(set_flags, z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	}
}

#pragma region INC rr
TEST(Z80_16bitArithmeticTest, IncRegBC)
{
	Test16bitRegisterIncDec(0x03, Register16bit::BC, 0x235F, 0x2360, false);
	Test16bitRegisterIncDec(0x03, Register16bit::BC, 0xFFFF, 0, true);
}

TEST(Z80_16bitArithmeticTest, IncRegDE)
{
	Test16bitRegisterIncDec(0x13, Register16bit::DE, 0x235F, 0x2360, false);
	Test16bitRegisterIncDec(0x13, Register16bit::DE, 0xFFFF, 0, true);
}

TEST(Z80_16bitArithmeticTest, IncRegHL)
{
	Test16bitRegisterIncDec(0x23, Register16bit::HL, 0x235F, 0x2360, false);
	Test16bitRegisterIncDec(0x23, Register16bit::HL, 0xFFFF, 0, true);
}

TEST(Z80_16bitArithmeticTest, IncRegSP)
{
	Test16bitRegisterIncDec(0x33, Register16bit::SP, 0x235F, 0x2360, false);
	Test16bitRegisterIncDec(0x33, Register16bit::SP, 0xFFFF, 0, true);
}
#pragma endregion

#pragma region DEC rr
TEST(Z80_16bitArithmeticTest, DecRegBC)
{
	Test16bitRegisterIncDec(0x0B, Register16bit::BC, 0x235F, 0x235E, false);
	Test16bitRegisterIncDec(0x0B, Register16bit::BC, 0, 0xFFFF, true);
}

TEST(Z80_16bitArithmeticTest, DecRegDE)
{
	Test16bitRegisterIncDec(0x1B, Register16bit::DE, 0x235F, 0x235E, false);
	Test16bitRegisterIncDec(0x1B, Register16bit::DE, 0, 0xFFFF, true);
}

TEST(Z80_16bitArithmeticTest, DecRegHL)
{
	Test16bitRegisterIncDec(0x2B, Register16bit::HL, 0x235F, 0x235E, false);
	Test16bitRegisterIncDec(0x2B, Register16bit::HL, 0, 0xFFFF, true);
}

TEST(Z80_16bitArithmeticTest, DecRegSP)
{
	Test16bitRegisterIncDec(0x3B, Register16bit::SP, 0x235F, 0x235E, false);
	Test16bitRegisterIncDec(0x3B, Register16bit::SP, 0, 0xFFFF, true);
}
#pragma endregion
