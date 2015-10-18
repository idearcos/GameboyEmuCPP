#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void Load16bitRegFromValue(uint8_t opcode, Register16bit dest)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write16bitToMemory(0, 0xDAAD);
	z80.Execute(opcode);

	ASSERT_EQ(0xDAAD, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(2, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(3, 12), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

TEST(Z80_16bitLoadTest, LoadRegBCFromValue)
{
	Load16bitRegFromValue(0x01, Register16bit::BC);
}

TEST(Z80_16bitLoadTest, LoadRegDEFromValue)
{
	Load16bitRegFromValue(0x11, Register16bit::DE);
}

TEST(Z80_16bitLoadTest, LoadRegHLFromValue)
{
	Load16bitRegFromValue(0x21, Register16bit::HL);
}

TEST(Z80_16bitLoadTest, LoadRegSPFromValue)
{
	Load16bitRegFromValue(0x31, Register16bit::SP);
}

void Load16bitRegFrom16bitReg(uint8_t opcode, Register16bit dest, Register16bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(source, 0xDAAD);
	z80.Execute(opcode);

	ASSERT_EQ(0xDAAD, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

TEST(Z80_16bitLoadTest, LoadRegSPFromRegHL)
{
	Load16bitRegFrom16bitReg(0xF9, Register16bit::SP, Register16bit::HL);
}

TEST(Z80_16bitLoadTest, LoadRegHLFromRegSPPlusDisplacement)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0, static_cast<uint8_t>(127));
	z80.GetRegisters().Write(Register16bit::SP, 0xFFAD);
	z80.Execute(0xF8);

	ASSERT_EQ((0xFFAD + 127) & 0xFFFF, z80.GetRegisters().Read(Register16bit::HL)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::HL));
	ASSERT_EQ(1, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(3, 12), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(true, z80.GetRegisters().IsFlagSet(Flags::HalfCarry)) << "Half-carry flag is not set";
	ASSERT_EQ(true, z80.GetRegisters().IsFlagSet(Flags::Carry)) << "Carry flag is not set";
}

TEST(Z80_16bitLoadTest, LoadRegHLFromRegSPMinusDisplacement)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0, static_cast<uint8_t>(-128));
	z80.GetRegisters().Write(Register16bit::SP, 0xFFAD);
	z80.Execute(0xF8);

	ASSERT_EQ((0xFFAD -128) & 0xFFFF, z80.GetRegisters().Read(Register16bit::HL)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::HL));
	ASSERT_EQ(1, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(3, 12), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(false, z80.GetRegisters().IsFlagSet(Flags::HalfCarry)) << "Half-carry flag is not set";
	ASSERT_EQ(false, z80.GetRegisters().IsFlagSet(Flags::Carry)) << "Carry flag is not set";
}

void LoadAddressFrom16bitReg(uint8_t opcode, Register16bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write16bitToMemory(0, 0x1234);
	z80.GetRegisters().Write(source, 0xDAAD);
	z80.Execute(opcode);

	ASSERT_EQ(0xDAAD, mmu.Read16bitFromMemory(0x1234)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read16bitFromMemory(0x1234));
	ASSERT_EQ(2, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(5, 20), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

TEST(Z80_16bitLoadTest, LoadAddressFromRegSP)
{
	LoadAddressFrom16bitReg(0x08, Register16bit::SP);
}

void Push(uint8_t opcode, Register16bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(Register16bit::SP, 0x1234);
	z80.GetRegisters().Write(source, 0xDA07);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA07, mmu.Read16bitFromMemory(0x1232)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read16bitFromMemory(0x1232));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(0x1232, z80.GetRegisters().Read(Register16bit::SP)) << "SP read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::SP));
	ASSERT_EQ(Clock(4, 16), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, (z80.GetRegisters().Read(Register8bit::F) & 0xF0)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

TEST(Z80_16bitLoadTest, PushRegAF)
{
	Push(0xF5, Register16bit::AF);
}

TEST(Z80_16bitLoadTest, PushRegBC)
{
	Push(0xC5, Register16bit::BC);
}

TEST(Z80_16bitLoadTest, PushRegDE)
{
	Push(0xD5, Register16bit::DE);
}

TEST(Z80_16bitLoadTest, PushRegHL)
{
	Push(0xE5, Register16bit::HL);
}

void Pop(uint8_t opcode, Register16bit dest)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(Register16bit::SP, 0x1234);
	mmu.Write16bitToMemory(0x1234, 0xDA07);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA07, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(mmu.Read16bitFromMemory(0x1234));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(0x1236, z80.GetRegisters().Read(Register16bit::SP)) << "SP read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::SP));
	ASSERT_EQ(Clock(3, 12), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, (z80.GetRegisters().Read(Register8bit::F) & 0xF0)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

TEST(Z80_16bitLoadTest, PopRegAF)
{
	Pop(0xF1, Register16bit::AF);
}

TEST(Z80_16bitLoadTest, PopRegBC)
{
	Pop(0xC1, Register16bit::BC);
}

TEST(Z80_16bitLoadTest, PopRegDE)
{
	Pop(0xD1, Register16bit::DE);
}

TEST(Z80_16bitLoadTest, PopRegHL)
{
	Pop(0xE1, Register16bit::HL);
}
