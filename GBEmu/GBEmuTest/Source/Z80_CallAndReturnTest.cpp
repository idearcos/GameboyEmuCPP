#include <list>
#include <algorithm>
#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void TestCall(uint8_t opcode, uint16_t initial_pc_value, uint16_t call_address, uint16_t expected_pc_value, uint16_t initial_sp_value, uint16_t expected_sp_value, uint16_t expected_content_pointed_to_by_sp, Clock &&expected_time, std::list<Flags> &&set_flags)
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
	z80.GetRegisters().Write(Register16bit::SP, initial_sp_value);
	z80.GetRegisters().Write(Register16bit::PC, initial_pc_value);
	mmu.Write16bitToMemory(initial_pc_value, call_address);
	z80.Execute(opcode);

	ASSERT_EQ(expected_pc_value, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(call_address) << ")";
	ASSERT_EQ(expected_sp_value, z80.GetRegisters().Read(Register16bit::SP)) << "SP read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::SP)) << " (input value: 0x" << std::hex << static_cast<size_t>(call_address) << ")";
	ASSERT_EQ(expected_content_pointed_to_by_sp, mmu.Read16bitFromMemory(z80.GetRegisters().Read(Register16bit::SP))) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read16bitFromMemory(z80.GetRegisters().Read(Register16bit::SP))) << " (input value: 0x" << std::hex << static_cast<size_t>(call_address) << ")";
	// Cycles information extracted from GameBoyProgrammingManual.pdf
	ASSERT_EQ(expected_time, z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(call_address) << ")";
	for (const auto& flag : set_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(call_address) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(call_address) << ")";
	}
}

TEST(Z80_CallAndReturnTest, Call)
{
	TestCall(0xCD, 0x0123, 0x4567, 0x4567, 0xFF02, 0xFF00, 0x0125, Clock(6, 24), { Flags::Zero });
	TestCall(0xCD, 0x4567, 0x0123, 0x0123, 0xFF02, 0xFF00, 0x4569, Clock(6, 24), { Flags::Subtract });
	TestCall(0xCD, 0x89AB, 0xCDEF, 0xCDEF, 0xFF02, 0xFF00, 0x89AD, Clock(6, 24), { Flags::HalfCarry });
	TestCall(0xCD, 0xCDEF, 0x89AB, 0x89AB, 0xFF02, 0xFF00, 0xCDF1, Clock(6, 24), { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, CallIfNonZero)
{
	TestCall(0xC4, 0x0123, 0x4567, 0x4567, 0xFF02, 0xFF00, 0x0125, Clock(6, 24), {});
	TestCall(0xC4, 0x4567, 0x0123, 0x0123, 0xFF02, 0xFF00, 0x4569, Clock(6, 24), { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestCall(0xC4, 0x89AB, 0xCDEF, 0x89AB + 2, 0xFF02, 0xFF02, 0, Clock(3, 12), { Flags::Zero });
	TestCall(0xC4, 0xCDEF, 0x89AB, 0xCDEF + 2, 0xFF02, 0xFF02, 0, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_CallAndReturnTest, CallIfZero)
{
	TestCall(0xCC, 0x0123, 0x4567, 0x4567, 0xFF02, 0xFF00, 0x0125, Clock(6, 24), { Flags::Zero });
	TestCall(0xCC, 0x4567, 0x0123, 0x0123, 0xFF02, 0xFF00, 0x4569, Clock(6, 24), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestCall(0xCC, 0x89AB, 0xCDEF, 0x89AB + 2, 0xFF02, 0xFF02, 0, Clock(3, 12), {});
	TestCall(0xCC, 0xCDEF, 0x89AB, 0xCDEF + 2, 0xFF02, 0xFF02, 0, Clock(3, 12), { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_CallAndReturnTest, CallIfNonCarry)
{
	TestCall(0xD4, 0x0123, 0x4567, 0x4567, 0xFF02, 0xFF00, 0x0125, Clock(6, 24), {});
	TestCall(0xD4, 0x4567, 0x0123, 0x0123, 0xFF02, 0xFF00, 0x4569, Clock(6, 24), { Flags::Zero, Flags::Subtract, Flags::HalfCarry });
	TestCall(0xD4, 0x89AB, 0xCDEF, 0x89AB + 2, 0xFF02, 0xFF02, 0, Clock(3, 12), { Flags::Carry });
	TestCall(0xD4, 0xCDEF, 0x89AB, 0xCDEF + 2, 0xFF02, 0xFF02, 0, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_CallAndReturnTest, CallIfCarry)
{
	TestCall(0xDC, 0x0123, 0x4567, 0x4567, 0xFF02, 0xFF00, 0x0125, Clock(6, 24), { Flags::Carry });
	TestCall(0xDC, 0x4567, 0x0123, 0x0123, 0xFF02, 0xFF00, 0x4569, Clock(6, 24), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestCall(0xDC, 0x89AB, 0xCDEF, 0x89AB + 2, 0xFF02, 0xFF02, 0, Clock(3, 12), {});
	TestCall(0xDC, 0xCDEF, 0x89AB, 0xCDEF + 2, 0xFF02, 0xFF02, 0, Clock(3, 12), { Flags::Zero, Flags::Subtract, Flags::HalfCarry });
}

void TestReturn(uint8_t opcode, uint16_t initial_content_pointed_to_by_sp, uint16_t expected_pc_value, uint16_t initial_sp_value, uint16_t expected_sp_value, Clock &&expected_time, std::list<Flags> &&set_flags, bool expected_interrupt_master_enable = false)
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
	z80.SetInterruptMasterEnable(false);
	z80.GetRegisters().Write(Register16bit::SP, initial_sp_value);
	mmu.Write16bitToMemory(initial_sp_value, initial_content_pointed_to_by_sp);
	z80.Execute(opcode);

	ASSERT_EQ(expected_pc_value, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (input value: 0x" << std::hex << static_cast<size_t>(initial_content_pointed_to_by_sp) << ")";
	ASSERT_EQ(expected_sp_value, z80.GetRegisters().Read(Register16bit::SP)) << "SP read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::SP)) << " (input value: 0x" << std::hex << static_cast<size_t>(initial_content_pointed_to_by_sp) << ")";
	// Cycles information extracted from GameBoyProgrammingManual.pdf
	ASSERT_EQ(expected_time, z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (input value: 0x" << std::hex << static_cast<size_t>(initial_content_pointed_to_by_sp) << ")";
	for (const auto& flag : set_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(initial_content_pointed_to_by_sp) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (input value: 0x" << std::hex << static_cast<size_t>(initial_content_pointed_to_by_sp) << ")";
	}
	ASSERT_EQ(expected_interrupt_master_enable, z80.GetInterruptMasterEnable()) << "Unexpected interrupt master enable value: "
		<< std::boolalpha << z80.GetInterruptMasterEnable() << " (input value: 0x" << std::hex << static_cast<size_t>(initial_content_pointed_to_by_sp) << ")";
}

TEST(Z80_CallAndReturnTest, Return)
{
	TestReturn(0xC9, 0x0123, 0x0123, 0xFF00, 0xFF02, Clock(4, 16), { Flags::Zero });
	TestReturn(0xC9, 0x4567, 0x4567, 0xFF00, 0xFF02, Clock(4, 16), { Flags::Subtract });
	TestReturn(0xC9, 0x89AB, 0x89AB, 0xFF00, 0xFF02, Clock(4, 16), { Flags::HalfCarry });
	TestReturn(0xC9, 0xCDEF, 0xCDEF, 0xFF00, 0xFF02, Clock(4, 16), { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, ReturnIfNonZero)
{
	TestReturn(0xC0, 0x0123, 0x0123, 0xFF00, 0xFF02, Clock(5, 20), {});
	TestReturn(0xC0, 0x4567, 0x4567, 0xFF00, 0xFF02, Clock(5, 20), { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestReturn(0xC0, 0x89AB, 0, 0xFF00, 0xFF00, Clock(2, 8), { Flags::Zero });
	TestReturn(0xC0, 0xCDEF, 0, 0xFF00, 0xFF00, Clock(2, 8), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_CallAndReturnTest, ReturnIfZero)
{
	TestReturn(0xC8, 0x0123, 0x0123, 0xFF00, 0xFF02, Clock(5, 20), { Flags::Zero });
	TestReturn(0xC8, 0x4567, 0x4567, 0xFF00, 0xFF02, Clock(5, 20), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestReturn(0xC8, 0x89AB, 0, 0xFF00, 0xFF00, Clock(2, 8), {});
	TestReturn(0xC8, 0xCDEF, 0, 0xFF00, 0xFF00, Clock(2, 8), { Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_CallAndReturnTest, ReturnIfNonCarry)
{
	TestReturn(0xD0, 0x0123, 0x0123, 0xFF00, 0xFF02, Clock(5, 20), {});
	TestReturn(0xD0, 0x4567, 0x4567, 0xFF00, 0xFF02, Clock(5, 20), { Flags::Zero, Flags::Subtract, Flags::HalfCarry });
	TestReturn(0xD0, 0x89AB, 0, 0xFF00, 0xFF00, Clock(2, 8), { Flags::Carry });
	TestReturn(0xD0, 0xCDEF, 0, 0xFF00, 0xFF00, Clock(2, 8), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
}

TEST(Z80_CallAndReturnTest, ReturnIfCarry)
{
	TestReturn(0xD8, 0x0123, 0x0123, 0xFF00, 0xFF02, Clock(5, 20), { Flags::Carry });
	TestReturn(0xD8, 0x4567, 0x4567, 0xFF00, 0xFF02, Clock(5, 20), { Flags::Zero, Flags::Subtract, Flags::HalfCarry, Flags::Carry });
	TestReturn(0xD8, 0x89AB, 0, 0xFF00, 0xFF00, Clock(2, 8), {});
	TestReturn(0xD8, 0xCDEF, 0, 0xFF00, 0xFF00, Clock(2, 8), { Flags::Zero, Flags::Subtract, Flags::HalfCarry });
}

TEST(Z80_CallAndReturnTest, ReturnFromInterrupt)
{
	TestReturn(0xD9, 0x0123, 0x0123, 0xFF00, 0xFF02, Clock(4, 16), { Flags::Zero }, true);
	TestReturn(0xD9, 0x4567, 0x4567, 0xFF00, 0xFF02, Clock(4, 16), { Flags::Subtract }, true);
	TestReturn(0xD9, 0x89AB, 0x89AB, 0xFF00, 0xFF02, Clock(4, 16), { Flags::HalfCarry }, true);
	TestReturn(0xD9, 0xCDEF, 0xCDEF, 0xFF00, 0xFF02, Clock(4, 16), { Flags::Carry }, true);
}

void TestRestart(uint8_t opcode, uint16_t initial_pc_value, uint16_t expected_pc_value, uint16_t initial_sp_value, uint16_t expected_sp_value, std::list<Flags> &&set_flags)
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
	z80.GetRegisters().Write(Register16bit::SP, initial_sp_value);
	z80.GetRegisters().Write(Register16bit::PC, initial_pc_value);
	z80.Execute(opcode);

	ASSERT_EQ(expected_pc_value, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC)) << " (opcode: 0x" << std::hex << static_cast<size_t>(opcode) << ")";
	ASSERT_EQ(expected_sp_value, z80.GetRegisters().Read(Register16bit::SP)) << "SP read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::SP)) << " (opcode: 0x" << std::hex << static_cast<size_t>(opcode) << ")";
	ASSERT_EQ(initial_pc_value, mmu.Read16bitFromMemory(z80.GetRegisters().Read(Register16bit::SP))) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read16bitFromMemory(z80.GetRegisters().Read(Register16bit::SP))) << " (opcode: 0x" << std::hex << static_cast<size_t>(opcode) << ")";
	// Cycles information extracted from GameBoyProgrammingManual.pdf
	ASSERT_EQ(Clock(4, 16), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks()) << " (opcode: 0x" << std::hex << static_cast<size_t>(opcode) << ")";
	for (const auto& flag : set_flags)
	{
		ASSERT_TRUE(z80.GetRegisters().IsFlagSet(flag)) << "Expected flag is not set: " << flag << " (opcode: 0x" << std::hex << static_cast<size_t>(opcode) << ")";
	}
	for (const auto& flag : unexpected_flags)
	{
		ASSERT_FALSE(z80.GetRegisters().IsFlagSet(flag)) << "Unexpected flag is set: " << flag << " (opcode: 0x" << std::hex << static_cast<size_t>(opcode) << ")";
	}
}

TEST(Z80_CallAndReturnTest, Restart0x00)
{
	TestRestart(0xC7, 0x0123, 0x0000, 0xFF02, 0xFF00, { Flags::Zero });
	TestRestart(0xC7, 0x4567, 0x0000, 0xFF02, 0xFF00, { Flags::Subtract });
	TestRestart(0xC7, 0x89AB, 0x0000, 0xFF02, 0xFF00, { Flags::HalfCarry });
	TestRestart(0xC7, 0xCDEF, 0x0000, 0xFF02, 0xFF00, { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, Restart0x08)
{
	TestRestart(0xCF, 0x0123, 0x0008, 0xFF02, 0xFF00, { Flags::Zero });
	TestRestart(0xCF, 0x4567, 0x0008, 0xFF02, 0xFF00, { Flags::Subtract });
	TestRestart(0xCF, 0x89AB, 0x0008, 0xFF02, 0xFF00, { Flags::HalfCarry });
	TestRestart(0xCF, 0xCDEF, 0x0008, 0xFF02, 0xFF00, { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, Restart0x10)
{
	TestRestart(0xD7, 0x0123, 0x0010, 0xFF02, 0xFF00, { Flags::Zero });
	TestRestart(0xD7, 0x4567, 0x0010, 0xFF02, 0xFF00, { Flags::Subtract });
	TestRestart(0xD7, 0x89AB, 0x0010, 0xFF02, 0xFF00, { Flags::HalfCarry });
	TestRestart(0xD7, 0xCDEF, 0x0010, 0xFF02, 0xFF00, { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, Restart0x18)
{
	TestRestart(0xDF, 0x0123, 0x0018, 0xFF02, 0xFF00, { Flags::Zero });
	TestRestart(0xDF, 0x4567, 0x0018, 0xFF02, 0xFF00, { Flags::Subtract });
	TestRestart(0xDF, 0x89AB, 0x0018, 0xFF02, 0xFF00, { Flags::HalfCarry });
	TestRestart(0xDF, 0xCDEF, 0x0018, 0xFF02, 0xFF00, { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, Restart0x20)
{
	TestRestart(0xE7, 0x0123, 0x0020, 0xFF02, 0xFF00, { Flags::Zero });
	TestRestart(0xE7, 0x4567, 0x0020, 0xFF02, 0xFF00, { Flags::Subtract });
	TestRestart(0xE7, 0x89AB, 0x0020, 0xFF02, 0xFF00, { Flags::HalfCarry });
	TestRestart(0xE7, 0xCDEF, 0x0020, 0xFF02, 0xFF00, { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, Restart0x28)
{
	TestRestart(0xEF, 0x0123, 0x0028, 0xFF02, 0xFF00, { Flags::Zero });
	TestRestart(0xEF, 0x4567, 0x0028, 0xFF02, 0xFF00, { Flags::Subtract });
	TestRestart(0xEF, 0x89AB, 0x0028, 0xFF02, 0xFF00, { Flags::HalfCarry });
	TestRestart(0xEF, 0xCDEF, 0x0028, 0xFF02, 0xFF00, { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, Restart0x30)
{
	TestRestart(0xF7, 0x0123, 0x0030, 0xFF02, 0xFF00, { Flags::Zero });
	TestRestart(0xF7, 0x4567, 0x0030, 0xFF02, 0xFF00, { Flags::Subtract });
	TestRestart(0xF7, 0x89AB, 0x0030, 0xFF02, 0xFF00, { Flags::HalfCarry });
	TestRestart(0xF7, 0xCDEF, 0x0030, 0xFF02, 0xFF00, { Flags::Carry });
}

TEST(Z80_CallAndReturnTest, Restart0x38)
{
	TestRestart(0xFF, 0x0123, 0x0038, 0xFF02, 0xFF00, { Flags::Zero });
	TestRestart(0xFF, 0x4567, 0x0038, 0xFF02, 0xFF00, { Flags::Subtract });
	TestRestart(0xFF, 0x89AB, 0x0038, 0xFF02, 0xFF00, { Flags::HalfCarry });
	TestRestart(0xFF, 0xCDEF, 0x0038, 0xFF02, 0xFF00, { Flags::Carry });
}
