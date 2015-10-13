#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void Add8bitRegToAccumulator(uint8_t opcode, Register8bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.Execute(opcode);

	ASSERT_EQ(0xDA, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(1, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

TEST(Z80Test, AddRegAToAccumulator)
{
	Add8bitRegToAccumulator(0x87, Register8bit::A);
}

TEST(Z80Test, AddRegBToAccumulator)
{
	Add8bitRegToAccumulator(0x80, Register8bit::B);
}

TEST(Z80Test, AddRegCToAccumulator)
{
	Add8bitRegToAccumulator(0x81, Register8bit::C);
}

TEST(Z80Test, AddRegDToAccumulator)
{
	Add8bitRegToAccumulator(0x82, Register8bit::D);
}

TEST(Z80Test, AddRegEToAccumulator)
{
	Add8bitRegToAccumulator(0x83, Register8bit::E);
}

TEST(Z80Test, AddRegHToAccumulator)
{
	Add8bitRegToAccumulator(0x84, Register8bit::H);
}

TEST(Z80Test, AddRegLToAccumulator)
{
	Add8bitRegToAccumulator(0x85, Register8bit::L);
}
