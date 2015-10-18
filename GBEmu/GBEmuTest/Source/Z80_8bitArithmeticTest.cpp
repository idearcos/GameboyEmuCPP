#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void Test8bitArithmeticOperationWithAccumulator(uint8_t opcode, Register8bit source, uint8_t acc_value, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry = false)
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
	z80.GetRegisters().Write(Register8bit::A, acc_value);
	z80.GetRegisters().Write(source, value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(Register8bit::A)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::A)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(Clock(1, 4), z80.GetClock()) << "Unexpected operation duration: "
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

void Test8bitArithmeticOperationWithAccumulator(uint8_t opcode, Register16bit source_address, uint8_t acc_value, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry = false)
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
	z80.GetRegisters().Write(Register8bit::A, acc_value);
	z80.GetRegisters().Write(source_address, 0x1234);
	mmu.Write8bitToMemory(0x1234, value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(Register8bit::A)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::A)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
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

void Test8bitArithmeticOperationWithAccumulator(uint8_t opcode, uint8_t acc_value, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry = false)
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
	z80.GetRegisters().Write(Register8bit::A, acc_value);
	mmu.Write8bitToMemory(0, value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(Register8bit::A)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::A)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
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

#pragma region ADD
TEST(Z80_8bitArithmeticTest, AddRegAToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0x87, Register8bit::A, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x87, Register8bit::A, 0, 0x08, 0x10, { Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0x87, Register8bit::A, 0, 0x81, 0x02, { Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x87, Register8bit::A, 0, 0x80, 0, { Flags::Zero, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x87, Register8bit::A, 0, 0x88, 0x10, { Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x87, Register8bit::A, 0, 0x77, 0xEE, {});
}

void TestNonAccRegistersAdd(uint8_t opcode, Register8bit source)
{
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x08, 0x08, 0x10, { Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x81, 0x81, 0x02, { Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0xF8, 0x08, 0, { Flags::Zero, Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x80, 0x80, 0, { Flags::Zero, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x88, 0x88, 0x10, { Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x77, 0x77, 0xEE, {});
}

TEST(Z80_8bitArithmeticTest, AddRegBToAccumulator)
{
	TestNonAccRegistersAdd(0x80, Register8bit::B);
}

TEST(Z80_8bitArithmeticTest, AddRegCToAccumulator)
{
	TestNonAccRegistersAdd(0x81, Register8bit::C);
}

TEST(Z80_8bitArithmeticTest, AddRegDToAccumulator)
{
	TestNonAccRegistersAdd(0x82, Register8bit::D);
}

TEST(Z80_8bitArithmeticTest, AddRegEToAccumulator)
{
	TestNonAccRegistersAdd(0x83, Register8bit::E);
}

TEST(Z80_8bitArithmeticTest, AddRegHToAccumulator)
{
	TestNonAccRegistersAdd(0x84, Register8bit::H);
}

TEST(Z80_8bitArithmeticTest, AddRegLToAccumulator)
{
	TestNonAccRegistersAdd(0x85, Register8bit::L);
}

TEST(Z80_8bitArithmeticTest, AddRegHLToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0x86, Register16bit::HL, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x86, Register16bit::HL, 0x08, 0x08, 0x10, { Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0x86, Register16bit::HL, 0x81, 0x81, 0x02, { Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x86, Register16bit::HL, 0xF8, 0x08, 0, { Flags::Zero, Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x86, Register16bit::HL, 0x80, 0x80, 0, { Flags::Zero, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x86, Register16bit::HL, 0x88, 0x88, 0x10, { Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x86, Register16bit::HL, 0x77, 0x77, 0xEE, {});
}

TEST(Z80_8bitArithmeticTest, AddValueToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xC6, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xC6, 0x08, 0x08, 0x10, { Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0xC6, 0x81, 0x81, 0x02, { Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0xC6, 0xF8, 0x08, 0, { Flags::Zero, Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0xC6, 0x80, 0x80, 0, { Flags::Zero, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0xC6, 0x88, 0x88, 0x10, { Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0xC6, 0x77, 0x77, 0xEE, {});
}

#pragma endregion

#pragma region ADC
TEST(Z80_8bitArithmeticTest, AdcRegAToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0x8F, Register8bit::A, 0, 0, 0x01, {}, true);
	Test8bitArithmeticOperationWithAccumulator(0x8F, Register8bit::A, 0, 0x08, 0x11, { Flags::HalfCarry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8F, Register8bit::A, 0, 0x81, 0x03, { Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8F, Register8bit::A, 0, 0x80, 0x01, { Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8F, Register8bit::A, 0, 0x88, 0x11, { Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8F, Register8bit::A, 0, 0x77, 0xEF, {}, true);
}

void TestNonAccRegistersAdc(uint8_t opcode, Register8bit source)
{
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0, 0, 0x01, {}, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x08, 0x08, 0x11, { Flags::HalfCarry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x81, 0x81, 0x03, { Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0xF8, 0x08, 0x01, { Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x80, 0x80, 0x01, { Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x88, 0x88, 0x11, { Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x77, 0x77, 0xEF, {}, true);
}

TEST(Z80_8bitArithmeticTest, AdcRegBToAccumulator)
{
	TestNonAccRegistersAdc(0x88, Register8bit::B);
}

TEST(Z80_8bitArithmeticTest, AdcRegCToAccumulator)
{
	TestNonAccRegistersAdc(0x89, Register8bit::C);
}

TEST(Z80_8bitArithmeticTest, AdcRegDToAccumulator)
{
	TestNonAccRegistersAdc(0x8A, Register8bit::D);
}

TEST(Z80_8bitArithmeticTest, AdcRegEToAccumulator)
{
	TestNonAccRegistersAdc(0x8B, Register8bit::E);
}

TEST(Z80_8bitArithmeticTest, AdcRegHToAccumulator)
{
	TestNonAccRegistersAdc(0x8C, Register8bit::H);
}

TEST(Z80_8bitArithmeticTest, AdcRegLToAccumulator)
{
	TestNonAccRegistersAdc(0x8D, Register8bit::L);
}

TEST(Z80_8bitArithmeticTest, AdcRegHLToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0x8E, Register16bit::HL, 0, 0, 0x01, {}, true);
	Test8bitArithmeticOperationWithAccumulator(0x8E, Register16bit::HL, 0x08, 0x08, 0x11, { Flags::HalfCarry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8E, Register16bit::HL, 0x81, 0x81, 0x03, { Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8E, Register16bit::HL, 0xF8, 0x08, 0x01, { Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8E, Register16bit::HL, 0x80, 0x80, 0x01, { Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8E, Register16bit::HL, 0x88, 0x88, 0x11, { Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x8E, Register16bit::HL, 0x77, 0x77, 0xEF, {}, true);
}

TEST(Z80_8bitArithmeticTest, AdcValueToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xCE, 0, 0, 0x01, {}, true);
	Test8bitArithmeticOperationWithAccumulator(0xCE, 0x08, 0x08, 0x11, { Flags::HalfCarry }, true);
	Test8bitArithmeticOperationWithAccumulator(0xCE, 0x81, 0x81, 0x03, { Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0xCE, 0xF8, 0x08, 0x01, { Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0xCE, 0x80, 0x80, 0x01, { Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0xCE, 0x88, 0x88, 0x11, { Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0xCE, 0x77, 0x77, 0xEF, {}, true);
}
#pragma endregion

#pragma region SUB
TEST(Z80_8bitArithmeticTest, SubRegAFromAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0x97, Register8bit::A, 0, 0, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x97, Register8bit::A, 0, 0x08, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x97, Register8bit::A, 0, 0x81, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x97, Register8bit::A, 0, 0x80, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x97, Register8bit::A, 0, 0x88, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x97, Register8bit::A, 0, 0x77, 0, { Flags::Subtract, Flags::Zero });
}

void TestNonAccRegistersSub(uint8_t opcode, Register8bit source)
{
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0, 0, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x10, 0x0F, 0x01, { Flags::Subtract, Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x80, 0x90, 0xF0, { Flags::Subtract, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x80, 0x80, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x88, 0x99, 0xEF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x88, 0x77, 0x11, { Flags::Subtract });
}

TEST(Z80_8bitArithmeticTest, SubRegBFromAccumulator)
{
	TestNonAccRegistersSub(0x90, Register8bit::B);
}

TEST(Z80_8bitArithmeticTest, SubRegCFromAccumulator)
{
	TestNonAccRegistersSub(0x91, Register8bit::C);
}

TEST(Z80_8bitArithmeticTest, SubRegDFromAccumulator)
{
	TestNonAccRegistersSub(0x92, Register8bit::D);
}

TEST(Z80_8bitArithmeticTest, SubRegEFromAccumulator)
{
	TestNonAccRegistersSub(0x93, Register8bit::E);
}

TEST(Z80_8bitArithmeticTest, SubRegHFromAccumulator)
{
	TestNonAccRegistersSub(0x94, Register8bit::H);
}

TEST(Z80_8bitArithmeticTest, SubRegLFromAccumulator)
{
	TestNonAccRegistersSub(0x95, Register8bit::L);
}

TEST(Z80_8bitArithmeticTest, SubRegHLFromAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0x96, Register16bit::HL, 0, 0, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x96, Register16bit::HL, 0x10, 0x0F, 0x01, { Flags::Subtract, Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0x96, Register16bit::HL, 0x80, 0x90, 0xF0, { Flags::Subtract, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x96, Register16bit::HL, 0x80, 0x80, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0x96, Register16bit::HL, 0x88, 0x99, 0xEF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0x96, Register16bit::HL, 0x88, 0x77, 0x11, { Flags::Subtract });
}

TEST(Z80_8bitArithmeticTest, SubValueFromAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xD6, 0, 0, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xD6, 0x10, 0x0F, 0x01, { Flags::Subtract, Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0xD6, 0x80, 0x90, 0xF0, { Flags::Subtract, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0xD6, 0x80, 0x80, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xD6, 0x88, 0x99, 0xEF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	Test8bitArithmeticOperationWithAccumulator(0xD6, 0x88, 0x77, 0x11, { Flags::Subtract });
}
#pragma endregion

#pragma region SBC
TEST(Z80_8bitArithmeticTest, SbcRegAFromAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0x9F, Register8bit::A, 0, 0, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9F, Register8bit::A, 0, 0x08, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9F, Register8bit::A, 0, 0x81, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9F, Register8bit::A, 0, 0x80, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9F, Register8bit::A, 0, 0x88, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9F, Register8bit::A, 0, 0x77, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
}

void TestNonAccRegistersSbc(uint8_t opcode, Register8bit source)
{
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0, 0, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x10, 0x0F, 0x00, { Flags::Subtract, Flags::Zero, Flags::HalfCarry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x80, 0x90, 0xEF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x80, 0x80, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x88, 0x99, 0xEE, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x88, 0x77, 0x10, { Flags::Subtract }, true);
}

TEST(Z80_8bitArithmeticTest, SbcRegBFromAccumulator)
{
	TestNonAccRegistersSbc(0x98, Register8bit::B);
}

TEST(Z80_8bitArithmeticTest, SbcRegCFromAccumulator)
{
	TestNonAccRegistersSbc(0x99, Register8bit::C);
}

TEST(Z80_8bitArithmeticTest, SbcRegDFromAccumulator)
{
	TestNonAccRegistersSbc(0x9A, Register8bit::D);
}

TEST(Z80_8bitArithmeticTest, SbcRegEFromAccumulator)
{
	TestNonAccRegistersSbc(0x9B, Register8bit::E);
}

TEST(Z80_8bitArithmeticTest, SbcRegHFromAccumulator)
{
	TestNonAccRegistersSbc(0x9C, Register8bit::H);
}

TEST(Z80_8bitArithmeticTest, SbcRegLFromAccumulator)
{
	TestNonAccRegistersSbc(0x9D, Register8bit::L);
}

TEST(Z80_8bitArithmeticTest, SbcRegHLFromAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0x9E, Register16bit::HL, 0, 0, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9E, Register16bit::HL, 0x10, 0x0F, 0x00, { Flags::Subtract, Flags::Zero, Flags::HalfCarry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9E, Register16bit::HL, 0x80, 0x90, 0xEF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9E, Register16bit::HL, 0x80, 0x80, 0xFF, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9E, Register16bit::HL, 0x88, 0x99, 0xEE, { Flags::Subtract, Flags::HalfCarry, Flags::Carry }, true);
	Test8bitArithmeticOperationWithAccumulator(0x9E, Register16bit::HL, 0x88, 0x77, 0x10, { Flags::Subtract }, true);
}
#pragma endregion

#pragma region AND
TEST(Z80_8bitArithmeticTest, AndRegAToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xA7, Register8bit::A, 0, 0, 0, { Flags::HalfCarry, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xA7, Register8bit::A, 0, 0x3F, 0x3F, { Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0xA7, Register8bit::A, 0, 0x38, 0x38, { Flags::HalfCarry });
}

void TestNonAccRegistersAnd(uint8_t opcode, Register8bit source)
{
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x5A, 0, 0, { Flags::HalfCarry, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x5A, 0x3F, 0x1A, { Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x5A, 0x38, 0x18, { Flags::HalfCarry });
}

TEST(Z80_8bitArithmeticTest, AndRegBToAccumulator)
{
	TestNonAccRegistersAnd(0xA0, Register8bit::B);
}

TEST(Z80_8bitArithmeticTest, AndRegCToAccumulator)
{
	TestNonAccRegistersAnd(0xA1, Register8bit::C);
}

TEST(Z80_8bitArithmeticTest, AndRegDToAccumulator)
{
	TestNonAccRegistersAnd(0xA2, Register8bit::D);
}

TEST(Z80_8bitArithmeticTest, AndRegEToAccumulator)
{
	TestNonAccRegistersAnd(0xA3, Register8bit::E);
}

TEST(Z80_8bitArithmeticTest, AndRegHToAccumulator)
{
	TestNonAccRegistersAnd(0xA4, Register8bit::H);
}

TEST(Z80_8bitArithmeticTest, AndRegLToAccumulator)
{
	TestNonAccRegistersAnd(0xA5, Register8bit::L);
}

TEST(Z80_8bitArithmeticTest, AndRegHLToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xA6, Register16bit::HL, 0x5A, 0, 0, { Flags::HalfCarry, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xA6, Register16bit::HL, 0x5A, 0x3F, 0x1A, { Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0xA6, Register16bit::HL, 0x5A, 0x38, 0x18, { Flags::HalfCarry });
}

TEST(Z80_8bitArithmeticTest, AndValueToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xE6, 0x5A, 0, 0, { Flags::HalfCarry, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xE6, 0x5A, 0x3F, 0x1A, { Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0xE6, 0x5A, 0x38, 0x18, { Flags::HalfCarry });
}
#pragma endregion

#pragma region OR
TEST(Z80_8bitArithmeticTest, OrRegAToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xB7, Register8bit::A, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xB7, Register8bit::A, 0, 0x3F, 0x3F, {});
	Test8bitArithmeticOperationWithAccumulator(0xB7, Register8bit::A, 0, 0x38, 0x38, {});
}

void TestNonAccRegistersOr(uint8_t opcode, Register8bit source)
{
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x5A, 0x5A, 0x5A, {});
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x5A, 0x03, 0x5B, {});
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x5A, 0x0F, 0x5F, {});
}

TEST(Z80_8bitArithmeticTest, OrRegBToAccumulator)
{
	TestNonAccRegistersOr(0xB0, Register8bit::B);
}

TEST(Z80_8bitArithmeticTest, OrRegCToAccumulator)
{
	TestNonAccRegistersOr(0xB1, Register8bit::C);
}

TEST(Z80_8bitArithmeticTest, OrRegDToAccumulator)
{
	TestNonAccRegistersOr(0xB2, Register8bit::D);
}

TEST(Z80_8bitArithmeticTest, OrRegEToAccumulator)
{
	TestNonAccRegistersOr(0xB3, Register8bit::E);
}

TEST(Z80_8bitArithmeticTest, OrRegHToAccumulator)
{
	TestNonAccRegistersOr(0xB4, Register8bit::H);
}

TEST(Z80_8bitArithmeticTest, OrRegLToAccumulator)
{
	TestNonAccRegistersOr(0xB5, Register8bit::L);
}

TEST(Z80_8bitArithmeticTest, OrRegHLToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xB6, Register16bit::HL, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xB6, Register16bit::HL, 0x5A, 0x5A, 0x5A, {});
	Test8bitArithmeticOperationWithAccumulator(0xB6, Register16bit::HL, 0x5A, 0x03, 0x5B, {});
	Test8bitArithmeticOperationWithAccumulator(0xB6, Register16bit::HL, 0x5A, 0x0F, 0x5F, {});
}

TEST(Z80_8bitArithmeticTest, OrValueToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xF6, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xF6, 0x5A, 0x5A, 0x5A, {});
	Test8bitArithmeticOperationWithAccumulator(0xF6, 0x5A, 0x03, 0x5B, {});
	Test8bitArithmeticOperationWithAccumulator(0xF6, 0x5A, 0x0F, 0x5F, {});
}
#pragma endregion

#pragma region XOR
TEST(Z80_8bitArithmeticTest, XorRegAToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xAF, Register8bit::A, 0, 0, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xAF, Register8bit::A, 0, 0xFF, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xAF, Register8bit::A, 0, 0x38, 0, { Flags::Zero });
}

void TestNonAccRegistersXor(uint8_t opcode, Register8bit source)
{
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0xFF, 0xFF, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0xFF, 0x0F, 0xF0, {});
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0xFF, 0x8A, 0x75, {});
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0xFF, 0x00, 0xFF, {});
}

TEST(Z80_8bitArithmeticTest, XorRegBToAccumulator)
{
	TestNonAccRegistersXor(0xA8, Register8bit::B);
}

TEST(Z80_8bitArithmeticTest, XorRegCToAccumulator)
{
	TestNonAccRegistersXor(0xA9, Register8bit::C);
}

TEST(Z80_8bitArithmeticTest, XorRegDToAccumulator)
{
	TestNonAccRegistersXor(0xAA, Register8bit::D);
}

TEST(Z80_8bitArithmeticTest, XorRegEToAccumulator)
{
	TestNonAccRegistersXor(0xAB, Register8bit::E);
}

TEST(Z80_8bitArithmeticTest, XorRegHToAccumulator)
{
	TestNonAccRegistersXor(0xAC, Register8bit::H);
}

TEST(Z80_8bitArithmeticTest, XorRegLToAccumulator)
{
	TestNonAccRegistersXor(0xAD, Register8bit::L);
}

TEST(Z80_8bitArithmeticTest, XorRegHLToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xAE, Register16bit::HL, 0xFF, 0xFF, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xAE, Register16bit::HL, 0xFF, 0x0F, 0xF0, {});
	Test8bitArithmeticOperationWithAccumulator(0xAE, Register16bit::HL, 0xFF, 0x8A, 0x75, {});
	Test8bitArithmeticOperationWithAccumulator(0xAE, Register16bit::HL, 0xFF, 0x00, 0xFF, {});
}

TEST(Z80_8bitArithmeticTest, XorValueToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xEE, 0xFF, 0xFF, 0, { Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xEE, 0xFF, 0x0F, 0xF0, {});
	Test8bitArithmeticOperationWithAccumulator(0xEE, 0xFF, 0x8A, 0x75, {});
	Test8bitArithmeticOperationWithAccumulator(0xEE, 0xFF, 0x00, 0xFF, {});
}
#pragma endregion

#pragma region CP
TEST(Z80_8bitArithmeticTest, CpRegAToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xBF, Register8bit::A, 0, 0, 0, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xBF, Register8bit::A, 0, 0xFF, 0xFF, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xBF, Register8bit::A, 0, 0x38, 0x38, { Flags::Subtract, Flags::Zero });
}

void TestNonAccRegistersCp(uint8_t opcode, Register8bit source)
{
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x3C, 0x2F, 0x3C, { Flags::Subtract, Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x3C, 0x3C, 0x3C, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(opcode, source, 0x3C, 0x40, 0x3C, { Flags::Subtract, Flags::Carry });
}

TEST(Z80_8bitArithmeticTest, CpRegBToAccumulator)
{
	TestNonAccRegistersCp(0xB8, Register8bit::B);
}

TEST(Z80_8bitArithmeticTest, CpRegCToAccumulator)
{
	TestNonAccRegistersCp(0xB9, Register8bit::C);
}

TEST(Z80_8bitArithmeticTest, CpRegDToAccumulator)
{
	TestNonAccRegistersCp(0xBA, Register8bit::D);
}

TEST(Z80_8bitArithmeticTest, CpRegEToAccumulator)
{
	TestNonAccRegistersCp(0xBB, Register8bit::E);
}

TEST(Z80_8bitArithmeticTest, CpRegHToAccumulator)
{
	TestNonAccRegistersCp(0xBC, Register8bit::H);
}

TEST(Z80_8bitArithmeticTest, CpRegLToAccumulator)
{
	TestNonAccRegistersCp(0xBD, Register8bit::L);
}

TEST(Z80_8bitArithmeticTest, CpRegHLToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xBE, Register16bit::HL, 0x3C, 0x2F, 0x3C, { Flags::Subtract, Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0xBE, Register16bit::HL, 0x3C, 0x3C, 0x3C, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xBE, Register16bit::HL, 0x3C, 0x40, 0x3C, { Flags::Subtract, Flags::Carry });
}

TEST(Z80_8bitArithmeticTest, CpValueToAccumulator)
{
	Test8bitArithmeticOperationWithAccumulator(0xFE, 0x3C, 0x2F, 0x3C, { Flags::Subtract, Flags::HalfCarry });
	Test8bitArithmeticOperationWithAccumulator(0xFE, 0x3C, 0x3C, 0x3C, { Flags::Subtract, Flags::Zero });
	Test8bitArithmeticOperationWithAccumulator(0xFE, 0x3C, 0x40, 0x3C, { Flags::Subtract, Flags::Carry });
}
#pragma endregion

void Test8bitArithmeticOperation(uint8_t opcode, Register8bit reg, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry)
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
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(reg)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(reg)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(Clock(1, 4), z80.GetClock()) << "Unexpected operation duration: "
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

void Test8bitArithmeticOperation(uint8_t opcode, Register16bit addr_reg, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry)
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
	z80.GetRegisters().Write(addr_reg, 0x1234);
	mmu.Write8bitToMemory(0x1234, value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, mmu.Read8bitFromMemory(0x1234)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(0x1234)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(value) << ")";
	ASSERT_EQ(Clock(3, 12), z80.GetClock()) << "Unexpected operation duration: "
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

#pragma region INC
TEST(Z80_8bitArithmeticTest, IncRegA)
{
	Test8bitArithmeticOperation(0x3C, Register8bit::A, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x3C, Register8bit::A, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, IncRegB)
{
	Test8bitArithmeticOperation(0x04, Register8bit::B, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x04, Register8bit::B, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, IncRegC)
{
	Test8bitArithmeticOperation(0x0C, Register8bit::C, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x0C, Register8bit::C, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, IncRegD)
{
	Test8bitArithmeticOperation(0x14, Register8bit::D, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x14, Register8bit::D, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, IncRegE)
{
	Test8bitArithmeticOperation(0x1C, Register8bit::E, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x1C, Register8bit::E, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, IncRegH)
{
	Test8bitArithmeticOperation(0x24, Register8bit::H, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x24, Register8bit::H, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, IncRegL)
{
	Test8bitArithmeticOperation(0x2C, Register8bit::L, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x2C, Register8bit::L, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, IncAddressInRegHL)
{
	Test8bitArithmeticOperation(0x34, Register16bit::HL, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x34, Register16bit::HL, 0x50, 0x51, { Flags::Carry }, true);
}
#pragma endregion

#pragma region DEC
TEST(Z80_8bitArithmeticTest, DecRegA)
{
	Test8bitArithmeticOperation(0x3D, Register8bit::A, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x3D, Register8bit::A, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, DecRegB)
{
	Test8bitArithmeticOperation(0x05, Register8bit::B, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x05, Register8bit::B, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, DecRegC)
{
	Test8bitArithmeticOperation(0x0D, Register8bit::C, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x0D, Register8bit::C, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, DecRegD)
{
	Test8bitArithmeticOperation(0x15, Register8bit::D, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x15, Register8bit::D, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, DecRegE)
{
	Test8bitArithmeticOperation(0x1D, Register8bit::E, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x1D, Register8bit::E, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, DecRegH)
{
	Test8bitArithmeticOperation(0x25, Register8bit::H, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x25, Register8bit::H, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, DecRegL)
{
	Test8bitArithmeticOperation(0x2D, Register8bit::L, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x2D, Register8bit::L, 0x50, 0x51, { Flags::Carry }, true);
}

TEST(Z80_8bitArithmeticTest, DecAddressInRegHL)
{
	Test8bitArithmeticOperation(0x35, Register16bit::HL, 0xFF, 0, { Flags::Zero, Flags::HalfCarry }, false);
	Test8bitArithmeticOperation(0x35, Register16bit::HL, 0x50, 0x51, { Flags::Carry }, true);
}
#pragma endregion

