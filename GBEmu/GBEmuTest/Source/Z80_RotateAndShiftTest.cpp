#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void TestRotateOperationWithAccumulator(uint8_t opcode, uint8_t acc_value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry = false)
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
	z80.Execute(opcode);

	ASSERT_EQ(expected_result, z80.GetRegisters().Read(Register8bit::A)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::A)) << " (input value: 0x" << std::hex << static_cast<size_t>(acc_value) << ")";
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(acc_value) << ")";
	ASSERT_EQ(Clock(1, 4), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(acc_value) << ")";
	for (const auto& flag : expected_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(acc_value) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(acc_value) << ")";
	}
}

TEST(Z80_RotateAndShiftTest, Rlca)
{
	// Consider Zero flag unaffected
	TestRotateOperationWithAccumulator(0x07, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperationWithAccumulator(0x07, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperationWithAccumulator(0x07, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperationWithAccumulator(0x07, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperationWithAccumulator(0x07, 0, 0, {}, true);
	TestRotateOperationWithAccumulator(0x07, 0, 0, {}, false);
}

TEST(Z80_RotateAndShiftTest, Rla)
{
	// Consider Zero flag unaffected
	TestRotateOperationWithAccumulator(0x17, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperationWithAccumulator(0x17, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperationWithAccumulator(0x17, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperationWithAccumulator(0x17, 0x80, 0, { Flags::Carry }, false);
	TestRotateOperationWithAccumulator(0x17, 0, 0x01, {}, true);
	TestRotateOperationWithAccumulator(0x17, 0, 0, {}, false);
}

TEST(Z80_RotateAndShiftTest, Rrca)
{
	// Consider Zero flag unaffected
	TestRotateOperationWithAccumulator(0x0F, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperationWithAccumulator(0x0F, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperationWithAccumulator(0x0F, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperationWithAccumulator(0x0F, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperationWithAccumulator(0x0F, 0, 0, {}, true);
	TestRotateOperationWithAccumulator(0x0F, 0, 0, {}, false);
}

TEST(Z80_RotateAndShiftTest, Rra)
{
	// Consider Zero flag unaffected
	TestRotateOperationWithAccumulator(0x1F, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperationWithAccumulator(0x1F, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperationWithAccumulator(0x1F, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperationWithAccumulator(0x1F, 0x01, 0, { Flags::Carry }, false);
	TestRotateOperationWithAccumulator(0x1F, 0, 0x80, {}, true);
	TestRotateOperationWithAccumulator(0x1F, 0, 0, {}, false);
}

void TestRotateOperation(uint8_t opcode, uint8_t cb_opcode, Register8bit reg, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry = false)
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

void TestRotateOperation(uint8_t opcode, uint8_t cb_opcode, Register16bit addr_reg, uint8_t value, uint8_t expected_result, std::list<Flags> &&expected_flags, bool set_carry = false)
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

#pragma region RLC
TEST(Z80_RotateAndShiftTest, RlcRegA)
{
	TestRotateOperation(0xCB, 0x07, Register8bit::A, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x07, Register8bit::A, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x07, Register8bit::A, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x07, Register8bit::A, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x07, Register8bit::A, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x07, Register8bit::A, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlcRegB)
{
	TestRotateOperation(0xCB, 0x00, Register8bit::B, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x00, Register8bit::B, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x00, Register8bit::B, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x00, Register8bit::B, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x00, Register8bit::B, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x00, Register8bit::B, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlcRegC)
{
	TestRotateOperation(0xCB, 0x01, Register8bit::C, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x01, Register8bit::C, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x01, Register8bit::C, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x01, Register8bit::C, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x01, Register8bit::C, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x01, Register8bit::C, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlcRegD)
{
	TestRotateOperation(0xCB, 0x02, Register8bit::D, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x02, Register8bit::D, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x02, Register8bit::D, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x02, Register8bit::D, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x02, Register8bit::D, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x02, Register8bit::D, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlcRegE)
{
	TestRotateOperation(0xCB, 0x03, Register8bit::E, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x03, Register8bit::E, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x03, Register8bit::E, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x03, Register8bit::E, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x03, Register8bit::E, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x03, Register8bit::E, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlcRegH)
{
	TestRotateOperation(0xCB, 0x04, Register8bit::H, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x04, Register8bit::H, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x04, Register8bit::H, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x04, Register8bit::H, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x04, Register8bit::H, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x04, Register8bit::H, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlcRegL)
{
	TestRotateOperation(0xCB, 0x05, Register8bit::L, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x05, Register8bit::L, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x05, Register8bit::L, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x05, Register8bit::L, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x05, Register8bit::L, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x05, Register8bit::L, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlcAddressInRegHL)
{
	TestRotateOperation(0xCB, 0x06, Register16bit::HL, 0x85, 0x0B, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x06, Register16bit::HL, 0x85, 0x0B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x06, Register16bit::HL, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x06, Register16bit::HL, 0x80, 0x01, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x06, Register16bit::HL, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x06, Register16bit::HL, 0, 0, { Flags::Zero }, false);
}
#pragma endregion

#pragma region RL
TEST(Z80_RotateAndShiftTest, RlRegA)
{
	TestRotateOperation(0xCB, 0x17, Register8bit::A, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x17, Register8bit::A, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x17, Register8bit::A, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x17, Register8bit::A, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x17, Register8bit::A, 0x11, 0x23, {}, true);
	TestRotateOperation(0xCB, 0x17, Register8bit::A, 0x11, 0x22, {}, false);
	TestRotateOperation(0xCB, 0x17, Register8bit::A, 0, 0x01, {}, true);
	TestRotateOperation(0xCB, 0x17, Register8bit::A, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlRegB)
{
	TestRotateOperation(0xCB, 0x10, Register8bit::B, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x10, Register8bit::B, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x10, Register8bit::B, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x10, Register8bit::B, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x10, Register8bit::B, 0x11, 0x23, {}, true);
	TestRotateOperation(0xCB, 0x10, Register8bit::B, 0x11, 0x22, {}, false);
	TestRotateOperation(0xCB, 0x10, Register8bit::B, 0, 0x01, {}, true);
	TestRotateOperation(0xCB, 0x10, Register8bit::B, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlRegC)
{
	TestRotateOperation(0xCB, 0x11, Register8bit::C, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x11, Register8bit::C, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x11, Register8bit::C, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x11, Register8bit::C, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x11, Register8bit::C, 0x11, 0x23, {}, true);
	TestRotateOperation(0xCB, 0x11, Register8bit::C, 0x11, 0x22, {}, false);
	TestRotateOperation(0xCB, 0x11, Register8bit::C, 0, 0x01, {}, true);
	TestRotateOperation(0xCB, 0x11, Register8bit::C, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlRegD)
{
	TestRotateOperation(0xCB, 0x12, Register8bit::D, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x12, Register8bit::D, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x12, Register8bit::D, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x12, Register8bit::D, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x12, Register8bit::D, 0x11, 0x23, {}, true);
	TestRotateOperation(0xCB, 0x12, Register8bit::D, 0x11, 0x22, {}, false);
	TestRotateOperation(0xCB, 0x12, Register8bit::D, 0, 0x01, {}, true);
	TestRotateOperation(0xCB, 0x12, Register8bit::D, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlRegE)
{
	TestRotateOperation(0xCB, 0x13, Register8bit::E, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x13, Register8bit::E, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x13, Register8bit::E, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x13, Register8bit::E, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x13, Register8bit::E, 0x11, 0x23, {}, true);
	TestRotateOperation(0xCB, 0x13, Register8bit::E, 0x11, 0x22, {}, false);
	TestRotateOperation(0xCB, 0x13, Register8bit::E, 0, 0x01, {}, true);
	TestRotateOperation(0xCB, 0x13, Register8bit::E, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlRegH)
{
	TestRotateOperation(0xCB, 0x14, Register8bit::H, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x14, Register8bit::H, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x14, Register8bit::H, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x14, Register8bit::H, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x14, Register8bit::H, 0x11, 0x23, {}, true);
	TestRotateOperation(0xCB, 0x14, Register8bit::H, 0x11, 0x22, {}, false);
	TestRotateOperation(0xCB, 0x14, Register8bit::H, 0, 0x01, {}, true);
	TestRotateOperation(0xCB, 0x14, Register8bit::H, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlRegL)
{
	TestRotateOperation(0xCB, 0x15, Register8bit::L, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x15, Register8bit::L, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x15, Register8bit::L, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x15, Register8bit::L, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x15, Register8bit::L, 0x11, 0x23, {}, true);
	TestRotateOperation(0xCB, 0x15, Register8bit::L, 0x11, 0x22, {}, false);
	TestRotateOperation(0xCB, 0x15, Register8bit::L, 0, 0x01, {}, true);
	TestRotateOperation(0xCB, 0x15, Register8bit::L, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RlAddressInRegHL)
{
	TestRotateOperation(0xCB, 0x16, Register16bit::HL, 0x95, 0x2B, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x16, Register16bit::HL, 0x95, 0x2A, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x16, Register16bit::HL, 0x80, 0x01, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x16, Register16bit::HL, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x16, Register16bit::HL, 0x11, 0x23, {}, true);
	TestRotateOperation(0xCB, 0x16, Register16bit::HL, 0x11, 0x22, {}, false);
	TestRotateOperation(0xCB, 0x16, Register16bit::HL, 0, 0x01, {}, true);
	TestRotateOperation(0xCB, 0x16, Register16bit::HL, 0, 0, { Flags::Zero }, false);
}
#pragma endregion

#pragma region RRC
TEST(Z80_RotateAndShiftTest, RrcRegA)
{
	TestRotateOperation(0xCB, 0x0F, Register8bit::A, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0F, Register8bit::A, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0F, Register8bit::A, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0F, Register8bit::A, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0F, Register8bit::A, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x0F, Register8bit::A, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrcRegB)
{
	TestRotateOperation(0xCB, 0x08, Register8bit::B, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x08, Register8bit::B, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x08, Register8bit::B, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x08, Register8bit::B, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x08, Register8bit::B, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x08, Register8bit::B, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrcRegC)
{
	TestRotateOperation(0xCB, 0x09, Register8bit::C, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x09, Register8bit::C, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x09, Register8bit::C, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x09, Register8bit::C, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x09, Register8bit::C, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x09, Register8bit::C, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrcRegD)
{
	TestRotateOperation(0xCB, 0x0A, Register8bit::D, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0A, Register8bit::D, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0A, Register8bit::D, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0A, Register8bit::D, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0A, Register8bit::D, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x0A, Register8bit::D, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrcRegE)
{
	TestRotateOperation(0xCB, 0x0B, Register8bit::E, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0B, Register8bit::E, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0B, Register8bit::E, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0B, Register8bit::E, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0B, Register8bit::E, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x0B, Register8bit::E, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrcRegH)
{
	TestRotateOperation(0xCB, 0x0C, Register8bit::H, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0C, Register8bit::H, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0C, Register8bit::H, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0C, Register8bit::H, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0C, Register8bit::H, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x0C, Register8bit::H, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrcRegL)
{
	TestRotateOperation(0xCB, 0x0D, Register8bit::L, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0D, Register8bit::L, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0D, Register8bit::L, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0D, Register8bit::L, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0D, Register8bit::L, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x0D, Register8bit::L, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrcAddressInRegHL)
{
	TestRotateOperation(0xCB, 0x0E, Register16bit::HL, 0x3B, 0x9D, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0E, Register16bit::HL, 0x3B, 0x9D, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0E, Register16bit::HL, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x0E, Register16bit::HL, 0x01, 0x80, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x0E, Register16bit::HL, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x0E, Register16bit::HL, 0, 0, { Flags::Zero }, false);
}
#pragma endregion

#pragma region RR
TEST(Z80_RotateAndShiftTest, RrRegA)
{
	TestRotateOperation(0xCB, 0x1F, Register8bit::A, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1F, Register8bit::A, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1F, Register8bit::A, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x1F, Register8bit::A, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x1F, Register8bit::A, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1F, Register8bit::A, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1F, Register8bit::A, 0, 0x80, {}, true);
	TestRotateOperation(0xCB, 0x1F, Register8bit::A, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrRegB)
{
	TestRotateOperation(0xCB, 0x18, Register8bit::B, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x18, Register8bit::B, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x18, Register8bit::B, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x18, Register8bit::B, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x18, Register8bit::B, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x18, Register8bit::B, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x18, Register8bit::B, 0, 0x80, {}, true);
	TestRotateOperation(0xCB, 0x18, Register8bit::B, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrRegC)
{
	TestRotateOperation(0xCB, 0x19, Register8bit::C, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x19, Register8bit::C, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x19, Register8bit::C, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x19, Register8bit::C, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x19, Register8bit::C, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x19, Register8bit::C, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x19, Register8bit::C, 0, 0x80, {}, true);
	TestRotateOperation(0xCB, 0x19, Register8bit::C, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrRegD)
{
	TestRotateOperation(0xCB, 0x1A, Register8bit::D, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1A, Register8bit::D, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1A, Register8bit::D, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x1A, Register8bit::D, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x1A, Register8bit::D, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1A, Register8bit::D, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1A, Register8bit::D, 0, 0x80, {}, true);
	TestRotateOperation(0xCB, 0x1A, Register8bit::D, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrRegE)
{
	TestRotateOperation(0xCB, 0x1B, Register8bit::E, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1B, Register8bit::E, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1B, Register8bit::E, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x1B, Register8bit::E, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x1B, Register8bit::E, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1B, Register8bit::E, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1B, Register8bit::E, 0, 0x80, {}, true);
	TestRotateOperation(0xCB, 0x1B, Register8bit::E, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrRegH)
{
	TestRotateOperation(0xCB, 0x1C, Register8bit::H, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1C, Register8bit::H, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1C, Register8bit::H, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x1C, Register8bit::H, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x1C, Register8bit::H, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1C, Register8bit::H, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1C, Register8bit::H, 0, 0x80, {}, true);
	TestRotateOperation(0xCB, 0x1C, Register8bit::H, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrRegL)
{
	TestRotateOperation(0xCB, 0x1D, Register8bit::L, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1D, Register8bit::L, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1D, Register8bit::L, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x1D, Register8bit::L, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x1D, Register8bit::L, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1D, Register8bit::L, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1D, Register8bit::L, 0, 0x80, {}, true);
	TestRotateOperation(0xCB, 0x1D, Register8bit::L, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, RrAddressInRegHL)
{
	TestRotateOperation(0xCB, 0x1E, Register16bit::HL, 0x81, 0xC0, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1E, Register16bit::HL, 0x81, 0x40, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1E, Register16bit::HL, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x1E, Register16bit::HL, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x1E, Register16bit::HL, 0x01, 0x80, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x1E, Register16bit::HL, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x1E, Register16bit::HL, 0, 0x80, {}, true);
	TestRotateOperation(0xCB, 0x1E, Register16bit::HL, 0, 0, { Flags::Zero }, false);
}
#pragma endregion

#pragma region SLA
TEST(Z80_RotateAndShiftTest, SlaRegA)
{
	TestRotateOperation(0xCB, 0x27, Register8bit::A, 0x80, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x27, Register8bit::A, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x27, Register8bit::A, 0xFF, 0xFE, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x27, Register8bit::A, 0xFF, 0xFE, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x27, Register8bit::A, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x27, Register8bit::A, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SlaRegB)
{
	TestRotateOperation(0xCB, 0x20, Register8bit::B, 0x80, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x20, Register8bit::B, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x20, Register8bit::B, 0xFF, 0xFE, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x20, Register8bit::B, 0xFF, 0xFE, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x20, Register8bit::B, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x20, Register8bit::B, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SlaRegC)
{
	TestRotateOperation(0xCB, 0x21, Register8bit::C, 0x80, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x21, Register8bit::C, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x21, Register8bit::C, 0xFF, 0xFE, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x21, Register8bit::C, 0xFF, 0xFE, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x21, Register8bit::C, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x21, Register8bit::C, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SlaRegD)
{
	TestRotateOperation(0xCB, 0x22, Register8bit::D, 0x80, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x22, Register8bit::D, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x22, Register8bit::D, 0xFF, 0xFE, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x22, Register8bit::D, 0xFF, 0xFE, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x22, Register8bit::D, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x22, Register8bit::D, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SlaRegE)
{
	TestRotateOperation(0xCB, 0x23, Register8bit::E, 0x80, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x23, Register8bit::E, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x23, Register8bit::E, 0xFF, 0xFE, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x23, Register8bit::E, 0xFF, 0xFE, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x23, Register8bit::E, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x23, Register8bit::E, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SlaRegH)
{
	TestRotateOperation(0xCB, 0x24, Register8bit::H, 0x80, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x24, Register8bit::H, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x24, Register8bit::H, 0xFF, 0xFE, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x24, Register8bit::H, 0xFF, 0xFE, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x24, Register8bit::H, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x24, Register8bit::H, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SlaRegL)
{
	TestRotateOperation(0xCB, 0x25, Register8bit::L, 0x80, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x25, Register8bit::L, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x25, Register8bit::L, 0xFF, 0xFE, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x25, Register8bit::L, 0xFF, 0xFE, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x25, Register8bit::L, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x25, Register8bit::L, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SlaAddressInRegHL)
{
	TestRotateOperation(0xCB, 0x26, Register16bit::HL, 0x80, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x26, Register16bit::HL, 0x80, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x26, Register16bit::HL, 0xFF, 0xFE, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x26, Register16bit::HL, 0xFF, 0xFE, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x26, Register16bit::HL, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x26, Register16bit::HL, 0, 0, { Flags::Zero }, false);
}
#pragma endregion

#pragma region SRA
TEST(Z80_RotateAndShiftTest, SraRegA)
{
	TestRotateOperation(0xCB, 0x2F, Register8bit::A, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x2F, Register8bit::A, 0x8A, 0xC5, {}, false);
	TestRotateOperation(0xCB, 0x2F, Register8bit::A, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x2F, Register8bit::A, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x2F, Register8bit::A, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x2F, Register8bit::A, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SraRegB)
{
	TestRotateOperation(0xCB, 0x28, Register8bit::B, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x28, Register8bit::B, 0x8A, 0xC5, {}, false);
	TestRotateOperation(0xCB, 0x28, Register8bit::B, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x28, Register8bit::B, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x28, Register8bit::B, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x28, Register8bit::B, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SraRegC)
{
	TestRotateOperation(0xCB, 0x29, Register8bit::C, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x29, Register8bit::C, 0x8A, 0xC5, {}, false);
	TestRotateOperation(0xCB, 0x29, Register8bit::C, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x29, Register8bit::C, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x29, Register8bit::C, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x29, Register8bit::C, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SraRegD)
{
	TestRotateOperation(0xCB, 0x2A, Register8bit::D, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x2A, Register8bit::D, 0x8A, 0xC5, {}, false);
	TestRotateOperation(0xCB, 0x2A, Register8bit::D, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x2A, Register8bit::D, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x2A, Register8bit::D, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x2A, Register8bit::D, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SraRegE)
{
	TestRotateOperation(0xCB, 0x2B, Register8bit::E, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x2B, Register8bit::E, 0x8A, 0xC5, {}, false);
	TestRotateOperation(0xCB, 0x2B, Register8bit::E, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x2B, Register8bit::E, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x2B, Register8bit::E, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x2B, Register8bit::E, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SraRegH)
{
	TestRotateOperation(0xCB, 0x2C, Register8bit::H, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x2C, Register8bit::H, 0x8A, 0xC5, {}, false);
	TestRotateOperation(0xCB, 0x2C, Register8bit::H, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x2C, Register8bit::H, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x2C, Register8bit::H, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x2C, Register8bit::H, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SraRegL)
{
	TestRotateOperation(0xCB, 0x2D, Register8bit::L, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x2D, Register8bit::L, 0x8A, 0xC5, {}, false);
	TestRotateOperation(0xCB, 0x2D, Register8bit::L, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x2D, Register8bit::L, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x2D, Register8bit::L, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x2D, Register8bit::L, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SraAddressInRegHL)
{
	TestRotateOperation(0xCB, 0x2E, Register16bit::HL, 0x8A, 0xC5, {}, true);
	TestRotateOperation(0xCB, 0x2E, Register16bit::HL, 0x8A, 0xC5, {}, false);
	TestRotateOperation(0xCB, 0x2E, Register16bit::HL, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x2E, Register16bit::HL, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x2E, Register16bit::HL, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x2E, Register16bit::HL, 0, 0, { Flags::Zero }, false);
}
#pragma endregion

#pragma region SRL
TEST(Z80_RotateAndShiftTest, SrlRegA)
{
	TestRotateOperation(0xCB, 0x3F, Register8bit::A, 0x8A, 0x45, {}, true);
	TestRotateOperation(0xCB, 0x3F, Register8bit::A, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x3F, Register8bit::A, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3F, Register8bit::A, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3F, Register8bit::A, 0xFF, 0x7F, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3F, Register8bit::A, 0xFF, 0x7F, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3F, Register8bit::A, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x3F, Register8bit::A, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SrlRegB)
{
	TestRotateOperation(0xCB, 0x38, Register8bit::B, 0x8A, 0x45, {}, true);
	TestRotateOperation(0xCB, 0x38, Register8bit::B, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x38, Register8bit::B, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x38, Register8bit::B, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x38, Register8bit::B, 0xFF, 0x7F, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x38, Register8bit::B, 0xFF, 0x7F, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x38, Register8bit::B, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x38, Register8bit::B, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SrlRegC)
{
	TestRotateOperation(0xCB, 0x39, Register8bit::C, 0x8A, 0x45, {}, true);
	TestRotateOperation(0xCB, 0x39, Register8bit::C, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x39, Register8bit::C, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x39, Register8bit::C, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x39, Register8bit::C, 0xFF, 0x7F, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x39, Register8bit::C, 0xFF, 0x7F, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x39, Register8bit::C, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x39, Register8bit::C, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SrlRegD)
{
	TestRotateOperation(0xCB, 0x3A, Register8bit::D, 0x8A, 0x45, {}, true);
	TestRotateOperation(0xCB, 0x3A, Register8bit::D, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x3A, Register8bit::D, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3A, Register8bit::D, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3A, Register8bit::D, 0xFF, 0x7F, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3A, Register8bit::D, 0xFF, 0x7F, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3A, Register8bit::D, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x3A, Register8bit::D, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SrlRegE)
{
	TestRotateOperation(0xCB, 0x3B, Register8bit::E, 0x8A, 0x45, {}, true);
	TestRotateOperation(0xCB, 0x3B, Register8bit::E, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x3B, Register8bit::E, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3B, Register8bit::E, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3B, Register8bit::E, 0xFF, 0x7F, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3B, Register8bit::E, 0xFF, 0x7F, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3B, Register8bit::E, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x3B, Register8bit::E, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SrlRegH)
{
	TestRotateOperation(0xCB, 0x3C, Register8bit::H, 0x8A, 0x45, {}, true);
	TestRotateOperation(0xCB, 0x3C, Register8bit::H, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x3C, Register8bit::H, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3C, Register8bit::H, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3C, Register8bit::H, 0xFF, 0x7F, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3C, Register8bit::H, 0xFF, 0x7F, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3C, Register8bit::H, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x3C, Register8bit::H, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SrlRegL)
{
	TestRotateOperation(0xCB, 0x3D, Register8bit::L, 0x8A, 0x45, {}, true);
	TestRotateOperation(0xCB, 0x3D, Register8bit::L, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x3D, Register8bit::L, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3D, Register8bit::L, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3D, Register8bit::L, 0xFF, 0x7F, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3D, Register8bit::L, 0xFF, 0x7F, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3D, Register8bit::L, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x3D, Register8bit::L, 0, 0, { Flags::Zero }, false);
}

TEST(Z80_RotateAndShiftTest, SrlAddressInRegHL)
{
	TestRotateOperation(0xCB, 0x3E, Register16bit::HL, 0x8A, 0x45, {}, true);
	TestRotateOperation(0xCB, 0x3E, Register16bit::HL, 0x8A, 0x45, {}, false);
	TestRotateOperation(0xCB, 0x3E, Register16bit::HL, 0x01, 0, { Flags::Zero, Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3E, Register16bit::HL, 0x01, 0, { Flags::Zero, Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3E, Register16bit::HL, 0xFF, 0x7F, { Flags::Carry }, true);
	TestRotateOperation(0xCB, 0x3E, Register16bit::HL, 0xFF, 0x7F, { Flags::Carry }, false);
	TestRotateOperation(0xCB, 0x3E, Register16bit::HL, 0, 0, { Flags::Zero }, true);
	TestRotateOperation(0xCB, 0x3E, Register16bit::HL, 0, 0, { Flags::Zero }, false);
}
#pragma endregion
