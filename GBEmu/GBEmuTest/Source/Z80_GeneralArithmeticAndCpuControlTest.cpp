#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void TestDAA(uint8_t opcode, uint8_t previous_opcode, uint8_t acc_value, Register8bit other_reg, uint8_t other_reg_value, uint8_t expected_acc_value, std::list<Flags> &&expected_flags)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	expected_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), expected_flags.begin(), expected_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	auto expected_duration = Clock(1, 4);
	z80.GetRegisters().Write(other_reg, other_reg_value);
	z80.GetRegisters().Write(Register8bit::A, acc_value);

	z80.Execute(previous_opcode);
	expected_duration += z80.GetClock();
	z80.Execute(opcode);

	ASSERT_EQ(expected_acc_value, z80.GetRegisters().Read(Register8bit::A)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::A)) << " (input value: 0x" << std::hex << static_cast<size_t>(acc_value) << ")";
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(acc_value) << ")";
	ASSERT_EQ(expected_duration, z80.GetClock()) << "Unexpected operation duration: "
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

TEST(Z80_GeneralArithmeticAndCpuControlTest, DAA)
{
	TestDAA(0x27, 0x80, 0x45, Register8bit::B, 0x38, 0x83, { Flags::HalfCarry });
	TestDAA(0x27, 0x80, 0x54, Register8bit::B, 0x83, 0x37, { Flags::Carry });
	TestDAA(0x27, 0x80, 0x66, Register8bit::B, 0x66, 0x32, { Flags::HalfCarry, Flags::Carry });
	TestDAA(0x27, 0x90, 0x83, Register8bit::B, 0x38, 0x45, { Flags::Subtract, Flags::HalfCarry });
	TestDAA(0x27, 0x90, 0x55, Register8bit::B, 0x60, 0x95, { Flags::Subtract, Flags::Carry });
	TestDAA(0x27, 0x90, 0x55, Register8bit::B, 0x66, 0x89, { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestDAA(0x27, 0x00, 0x00, Register8bit::B, 0x38, 0x00, { Flags::Zero });
}

void TestCpl(uint8_t opcode, uint8_t acc_value, uint8_t expected_acc_value, std::list<Flags> &&set_flags, std::list<Flags> &&expected_flags)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	expected_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), expected_flags.begin(), expected_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	for (const auto& flag : set_flags)
	{
		z80.GetRegisters().SetFlag(flag, true);
	}
	z80.GetRegisters().Write(Register8bit::A, acc_value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_acc_value, z80.GetRegisters().Read(Register8bit::A)) << "Register read unexpected value: "
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

TEST(Z80_GeneralArithmeticAndCpuControlTest, ComplementAccumulator)
{
	TestCpl(0x2F, 0x00, 0xFF, { Flags::Zero, Flags::Carry }, { Flags::Subtract, Flags::HalfCarry, Flags::Zero, Flags::Carry });
	TestCpl(0x2F, 0xFF, 0x00, {}, { Flags::Subtract, Flags::HalfCarry });
	TestCpl(0x2F, 0xF0, 0x0F, {}, { Flags::Subtract, Flags::HalfCarry });
	TestCpl(0x2F, 0x35, 0xCA, {}, { Flags::Subtract, Flags::HalfCarry });
}

void TestCarryComplementOrSet(uint8_t opcode, std::list<Flags> &&set_flags, std::list<Flags> &&expected_flags)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };
	all_flags.sort();
	expected_flags.sort();
	std::list<Flags> unexpected_flags;
	auto it = std::set_difference(all_flags.begin(), all_flags.end(), expected_flags.begin(), expected_flags.end(), std::back_inserter(unexpected_flags));

	TesterMMU mmu;
	Z80 z80(mmu);

	for (const auto& flag : set_flags)
	{
		z80.GetRegisters().SetFlag(flag, true);
	}
	z80.Execute(opcode);

	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(1, 4), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	for (const auto& flag : expected_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag;
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag;
	}
}

TEST(Z80_GeneralArithmeticAndCpuControlTest, ComplementCarryFlag)
{
	// HalfCarry behavior extracted from the regular Z80 manual (it is missing from the Gameboy Programming Manual)
	TestCarryComplementOrSet(0x3F, {}, { Flags::Carry });
	TestCarryComplementOrSet(0x3F, { Flags::Zero }, { Flags::Zero, Flags::Carry });
	TestCarryComplementOrSet(0x3F, { Flags::Carry }, { Flags::HalfCarry });
	TestCarryComplementOrSet(0x3F, { Flags::HalfCarry, Flags::Carry }, { Flags::HalfCarry });
	TestCarryComplementOrSet(0x3F, { Flags::Zero, Flags::Carry }, { Flags::Zero, Flags::HalfCarry });
	TestCarryComplementOrSet(0x3F, { Flags::Subtract, Flags::HalfCarry, }, { Flags::Carry });
}

TEST(Z80_GeneralArithmeticAndCpuControlTest, SetCarryFlag)
{
	TestCarryComplementOrSet(0x37, {}, { Flags::Carry });
	TestCarryComplementOrSet(0x37, { Flags::Zero }, { Flags::Zero, Flags::Carry });
	TestCarryComplementOrSet(0x37, { Flags::Carry }, { Flags::Carry });
	TestCarryComplementOrSet(0x37, { Flags::Zero, Flags::Carry }, { Flags::Zero, Flags::Carry });
	TestCarryComplementOrSet(0x37, { Flags::Subtract, Flags::HalfCarry, }, { Flags::Carry });
}

void TestEnableOrDisableInterrupts(uint8_t opcode, bool initial_interrupts_enable, bool expected_interrupts_enable)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };

	TesterMMU mmu;
	Z80 z80(mmu);

	z80.SetInterruptMasterEnable(false);
	z80.Execute(opcode);

	ASSERT_EQ(expected_interrupts_enable, z80.GetInterruptMasterEnable()) << "Interrupts master enable unexpected value: "
		<< std::boolalpha << z80.GetInterruptMasterEnable();
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(1, 4), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	for (const auto& flag : all_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag;
	}
}

TEST(Z80_GeneralArithmeticAndCpuControlTest, DisableInterrupts)
{
	TestEnableOrDisableInterrupts(0xF3, true, false);
	TestEnableOrDisableInterrupts(0xF3, false, false);
}

TEST(Z80_GeneralArithmeticAndCpuControlTest, EnableInterrupts)
{
	TestEnableOrDisableInterrupts(0xFB, false, true);
	TestEnableOrDisableInterrupts(0xFB, true, true);
}

void TestNopHaltOrStop(uint8_t opcode, Z80::State expected_state)
{
	std::list<Flags> all_flags{ Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry };

	TesterMMU mmu;
	Z80 z80(mmu);

	z80.SetInterruptMasterEnable(false);
	z80.Execute(opcode);

	ASSERT_EQ(expected_state, z80.GetState()) << "Z80 state unexpected value: "
		<< std::boolalpha << z80.GetState();
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(1, 4), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	for (const auto& flag : all_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag;
	}
}

TEST(Z80_GeneralArithmeticAndCpuControlTest, Nop)
{
	TestNopHaltOrStop(0x00, Z80::State::Running);
}

TEST(Z80_GeneralArithmeticAndCpuControlTest, Halt)
{
	TestNopHaltOrStop(0x76, Z80::State::Halted);
}

TEST(Z80_GeneralArithmeticAndCpuControlTest, Stop)
{
	ASSERT_THROW(TestNopHaltOrStop(0x10, Z80::State::Stopped), std::runtime_error);
}
