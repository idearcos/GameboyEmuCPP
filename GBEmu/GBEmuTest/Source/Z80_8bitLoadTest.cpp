#include "gtest/gtest.h"
#include "Z80.h"
#include "TesterMMU.h"

void Load8bitRegFromValue(uint8_t opcode, Register8bit dest)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0, 0xDA);
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

#pragma region LD r, n
TEST(Z80_8bitLoadTest, LoadRegAFromValue)
{
	Load8bitRegFromValue(0x3E, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadRegBFromValue)
{
	Load8bitRegFromValue(0x06, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadRegCFromValue)
{
	Load8bitRegFromValue(0x0E, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadRegDFromValue)
{
	Load8bitRegFromValue(0x16, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadRegEFromValue)
{
	Load8bitRegFromValue(0x1E, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadRegHFromValue)
{
	Load8bitRegFromValue(0x26, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadRegLFromValue)
{
	Load8bitRegFromValue(0x2E, Register8bit::L);
}
#pragma endregion

void Load8bitRegFrom8bitReg(uint8_t opcode, Register8bit dest, Register8bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(source, 0xDA);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(1, 4), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LD r, r*
TEST(Z80_8bitLoadTest, LoadRegAFromRegA)
{
	Load8bitRegFrom8bitReg(0x7F, Register8bit::A, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadRegAFromRegB)
{
	Load8bitRegFrom8bitReg(0x78, Register8bit::A, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadRegAFromRegC)
{
	Load8bitRegFrom8bitReg(0x79, Register8bit::A, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadRegAFromRegD)
{
	Load8bitRegFrom8bitReg(0x7A, Register8bit::A, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadRegAFromRegE)
{
	Load8bitRegFrom8bitReg(0x7B, Register8bit::A, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadRegAFromRegH)
{
	Load8bitRegFrom8bitReg(0x7C, Register8bit::A, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadRegAFromRegL)
{
	Load8bitRegFrom8bitReg(0x7D, Register8bit::A, Register8bit::L);
}

TEST(Z80_8bitLoadTest, LoadRegBFromRegA)
{
	Load8bitRegFrom8bitReg(0x47, Register8bit::B, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadRegBFromRegB)
{
	Load8bitRegFrom8bitReg(0x40, Register8bit::B, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadRegBFromRegC)
{
	Load8bitRegFrom8bitReg(0x41, Register8bit::B, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadRegBFromRegD)
{
	Load8bitRegFrom8bitReg(0x42, Register8bit::B, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadRegBFromRegE)
{
	Load8bitRegFrom8bitReg(0x43, Register8bit::B, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadRegBFromRegH)
{
	Load8bitRegFrom8bitReg(0x44, Register8bit::B, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadRegBFromRegL)
{
	Load8bitRegFrom8bitReg(0x45, Register8bit::B, Register8bit::L);
}

TEST(Z80_8bitLoadTest, LoadRegCFromRegA)
{
	Load8bitRegFrom8bitReg(0x4F, Register8bit::C, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadRegCFromRegB)
{
	Load8bitRegFrom8bitReg(0x48, Register8bit::C, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadRegCFromRegC)
{
	Load8bitRegFrom8bitReg(0x49, Register8bit::C, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadRegCFromRegD)
{
	Load8bitRegFrom8bitReg(0x4A, Register8bit::C, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadRegCFromRegE)
{
	Load8bitRegFrom8bitReg(0x4B, Register8bit::C, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadRegCFromRegH)
{
	Load8bitRegFrom8bitReg(0x4C, Register8bit::C, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadRegCFromRegL)
{
	Load8bitRegFrom8bitReg(0x4D, Register8bit::C, Register8bit::L);
}

TEST(Z80_8bitLoadTest, LoadRegDFromRegA)
{
	Load8bitRegFrom8bitReg(0x57, Register8bit::D, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadRegDFromRegB)
{
	Load8bitRegFrom8bitReg(0x50, Register8bit::D, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadRegDFromRegC)
{
	Load8bitRegFrom8bitReg(0x51, Register8bit::D, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadRegDFromRegD)
{
	Load8bitRegFrom8bitReg(0x52, Register8bit::D, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadRegDFromRegE)
{
	Load8bitRegFrom8bitReg(0x53, Register8bit::D, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadRegDFromRegH)
{
	Load8bitRegFrom8bitReg(0x54, Register8bit::D, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadRegDFromRegL)
{
	Load8bitRegFrom8bitReg(0x55, Register8bit::D, Register8bit::L);
}

TEST(Z80_8bitLoadTest, LoadRegEFromRegA)
{
	Load8bitRegFrom8bitReg(0x5F, Register8bit::E, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadRegEFromRegB)
{
	Load8bitRegFrom8bitReg(0x58, Register8bit::E, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadRegEFromRegC)
{
	Load8bitRegFrom8bitReg(0x59, Register8bit::E, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadRegEFromRegD)
{
	Load8bitRegFrom8bitReg(0x5A, Register8bit::E, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadRegEFromRegE)
{
	Load8bitRegFrom8bitReg(0x5B, Register8bit::E, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadRegEFromRegH)
{
	Load8bitRegFrom8bitReg(0x5C, Register8bit::E, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadRegEFromRegL)
{
	Load8bitRegFrom8bitReg(0x5D, Register8bit::E, Register8bit::L);
}

TEST(Z80_8bitLoadTest, LoadRegHFromRegA)
{
	Load8bitRegFrom8bitReg(0x67, Register8bit::H, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadRegHFromRegB)
{
	Load8bitRegFrom8bitReg(0x60, Register8bit::H, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadRegHFromRegC)
{
	Load8bitRegFrom8bitReg(0x61, Register8bit::H, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadRegHFromRegD)
{
	Load8bitRegFrom8bitReg(0x62, Register8bit::H, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadRegHFromRegE)
{
	Load8bitRegFrom8bitReg(0x63, Register8bit::H, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadRegHFromRegH)
{
	Load8bitRegFrom8bitReg(0x64, Register8bit::H, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadRegHFromRegL)
{
	Load8bitRegFrom8bitReg(0x65, Register8bit::H, Register8bit::L);
}

TEST(Z80_8bitLoadTest, LoadRegLFromRegA)
{
	Load8bitRegFrom8bitReg(0x6F, Register8bit::L, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadRegLFromRegB)
{
	Load8bitRegFrom8bitReg(0x68, Register8bit::L, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadRegLFromRegC)
{
	Load8bitRegFrom8bitReg(0x69, Register8bit::L, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadRegLFromRegD)
{
	Load8bitRegFrom8bitReg(0x6A, Register8bit::L, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadRegLFromRegE)
{
	Load8bitRegFrom8bitReg(0x6B, Register8bit::L, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadRegLFromRegH)
{
	Load8bitRegFrom8bitReg(0x6C, Register8bit::L, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadRegLFromRegL)
{
	Load8bitRegFrom8bitReg(0x6D, Register8bit::L, Register8bit::L);
}
#pragma endregion

void Load8bitRegFromAddressIn16bitReg(uint8_t opcode, Register8bit dest, Register16bit source_addr)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0x1234, 0xDA);
	z80.GetRegisters().Write(source_addr, 0x1234);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LD r, (rr)
TEST(Z80_8bitLoadTest, LoadRegAFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x7E, Register8bit::A, Register16bit::HL);
}

TEST(Z80_8bitLoadTest, LoadRegBFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x46, Register8bit::B, Register16bit::HL);
}

TEST(Z80_8bitLoadTest, LoadRegCFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x4E, Register8bit::C, Register16bit::HL);
}

TEST(Z80_8bitLoadTest, LoadRegDFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x56, Register8bit::D, Register16bit::HL);
}

TEST(Z80_8bitLoadTest, LoadRegEFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x5E, Register8bit::E, Register16bit::HL);
}

TEST(Z80_8bitLoadTest, LoadRegHFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x66, Register8bit::H, Register16bit::HL);
}

TEST(Z80_8bitLoadTest, LoadRegLFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x6E, Register8bit::L, Register16bit::HL);
}

TEST(Z80_8bitLoadTest, LoadRegAFromAddressInRegBC)
{
	Load8bitRegFromAddressIn16bitReg(0x0A, Register8bit::A, Register16bit::BC);
}

TEST(Z80_8bitLoadTest, LoadRegAFromAddressInRegDE)
{
	Load8bitRegFromAddressIn16bitReg(0x1A, Register8bit::A, Register16bit::DE);
}
#pragma endregion

void LoadAddressIn16bitRegFrom8bitReg(uint8_t opcode, Register16bit dest_addr, Register8bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(dest_addr, 0x1234);
	z80.GetRegisters().Write(source, 0xDA);
	const auto address = z80.GetRegisters().Read(dest_addr);
	const auto value = z80.GetRegisters().Read(source);
	z80.Execute(opcode);

	ASSERT_EQ(value, mmu.Read8bitFromMemory(address)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(address));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LD (rr), r
TEST(Z80_8bitLoadTest, LoadAddressInRegHLFromRegB)
{
	LoadAddressIn16bitRegFrom8bitReg(0x70, Register16bit::HL, Register8bit::B);
}

TEST(Z80_8bitLoadTest, LoadAddressInRegHLFromRegC)
{
	LoadAddressIn16bitRegFrom8bitReg(0x71, Register16bit::HL, Register8bit::C);
}

TEST(Z80_8bitLoadTest, LoadAddressInRegHLFromRegD)
{
	LoadAddressIn16bitRegFrom8bitReg(0x72, Register16bit::HL, Register8bit::D);
}

TEST(Z80_8bitLoadTest, LoadAddressInRegHLFromRegE)
{
	LoadAddressIn16bitRegFrom8bitReg(0x73, Register16bit::HL, Register8bit::E);
}

TEST(Z80_8bitLoadTest, LoadAddressInRegHLFromRegH)
{
	LoadAddressIn16bitRegFrom8bitReg(0x74, Register16bit::HL, Register8bit::H);
}

TEST(Z80_8bitLoadTest, LoadAddressInRegHLFromRegL)
{
	LoadAddressIn16bitRegFrom8bitReg(0x75, Register16bit::HL, Register8bit::L);
}

TEST(Z80_8bitLoadTest, LoadAddressInRegBCFromRegA)
{
	LoadAddressIn16bitRegFrom8bitReg(0x02, Register16bit::BC, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadAddressInRegDEFromRegA)
{
	LoadAddressIn16bitRegFrom8bitReg(0x12, Register16bit::DE, Register8bit::A);
}

TEST(Z80_8bitLoadTest, LoadAddressInRegHLFromRegA)
{
	LoadAddressIn16bitRegFrom8bitReg(0x77, Register16bit::HL, Register8bit::A);
}
#pragma endregion

void LoadAddressIn16bitRegFromValue(uint8_t opcode, Register16bit dest_addr)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(dest_addr, 0x1234);
	mmu.Write8bitToMemory(0, 0xDA);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, mmu.Read8bitFromMemory(z80.GetRegisters().Read(dest_addr))) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(z80.GetRegisters().Read(dest_addr)));
	ASSERT_EQ(1, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(3, 12), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LD (rr), n
TEST(Z80_8bitLoadTest, LoadAddressInRegHLFromValue)
{
	LoadAddressIn16bitRegFromValue(0x36, Register16bit::HL);
}
#pragma endregion

void Load8bitRegFromValueInAddress(uint8_t opcode, Register8bit dest)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0x1234, 0xDA);
	mmu.Write16bitToMemory(0, 0x1234);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(2, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(4, 16), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LD r, (nn)
TEST(Z80_8bitLoadTest, LoadRegAFromValueInAddress)
{
	Load8bitRegFromValueInAddress(0xFA, Register8bit::A);
}
#pragma endregion

void LoadAddressFrom8bitReg(uint8_t opcode, Register8bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write16bitToMemory(0, 0x1234);
	z80.GetRegisters().Write(source, 0xDA);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, mmu.Read8bitFromMemory(0x1234)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(0x1234));
	ASSERT_EQ(2, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(4, 16), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LD (nn), r
TEST(Z80_8bitLoadTest, LoadAddressFromRegA)
{
	LoadAddressFrom8bitReg(0xEA, Register8bit::A);
}
#pragma endregion

void Load8bitRegFromIOAddress(uint8_t opcode, Register8bit dest, Register8bit reg_displacement)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(reg_displacement, 0x12);
	mmu.Write8bitToMemory(0xFF00 + z80.GetRegisters().Read(reg_displacement), 0xDA);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LDIO r, (r*)
TEST(Z80_8bitLoadTest, LoadRegAFromIOAddressPlusReg)
{
	Load8bitRegFromIOAddress(0xF2, Register8bit::A, Register8bit::C);
}
#pragma endregion

void LoadIOAddressFrom8bitReg(uint8_t opcode, Register8bit reg_displacement, Register8bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(source, 0xDA);
	z80.GetRegisters().Write(reg_displacement, 0x12);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, mmu.Read8bitFromMemory(0xFF00 + z80.GetRegisters().Read(reg_displacement))) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(0xFF00 + z80.GetRegisters().Read(reg_displacement)));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LDIO (r), r*
TEST(Z80_8bitLoadTest, LoadIOAddressPlusRegFromRegA)
{
	LoadIOAddressFrom8bitReg(0xE2, Register8bit::C, Register8bit::A);
}
#pragma endregion

void Load8bitRegAndDecrement(uint8_t opcode, Register8bit dest, Register16bit source_addr)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0x1234, 0xDA);
	z80.GetRegisters().Write(source_addr, 0x1234);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, z80.GetRegisters().Read(dest)) << "Destination register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(0x1233, z80.GetRegisters().Read(source_addr)) << "Source register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(source_addr));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LDD r, (rr)
TEST(Z80_8bitLoadTest, LoadRegAAndDecrement)
{
	Load8bitRegAndDecrement(0x3A, Register8bit::A, Register16bit::HL);
}
#pragma endregion

void LoadAddressAndDecrement(uint8_t opcode, Register16bit dest_addr, Register8bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(dest_addr, 0x1234);
	z80.GetRegisters().Write(source, 0xDA);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, mmu.Read8bitFromMemory(0x1234)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(0x1234));
	ASSERT_EQ(0x1233, z80.GetRegisters().Read(dest_addr)) << "Destination register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest_addr));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LDD (rr),r
TEST(Z80_8bitLoadTest, LoadAddressAndDecrement)
{
	LoadAddressAndDecrement(0x32, Register16bit::HL, Register8bit::A);
}
#pragma endregion

void Load8bitRegAndIncrement(uint8_t opcode, Register8bit dest, Register16bit source_addr)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0x1234, 0xDA);
	z80.GetRegisters().Write(source_addr, 0x1234);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, z80.GetRegisters().Read(dest)) << "Destination register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(0x1235, z80.GetRegisters().Read(source_addr)) << "Source register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(source_addr));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LDI r, (rr)
TEST(Z80_8bitLoadTest, LoadRegAAndIncrement)
{
	Load8bitRegAndIncrement(0x2A, Register8bit::A, Register16bit::HL);
}
#pragma endregion

void LoadAddressAndIncrement(uint8_t opcode, Register16bit dest_addr, Register8bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	z80.GetRegisters().Write(dest_addr, 0x1234);
	z80.GetRegisters().Write(source, 0xDA);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, mmu.Read8bitFromMemory(0x1234)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(0x1234));
	ASSERT_EQ(0x1235, z80.GetRegisters().Read(dest_addr)) << "Destination register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest_addr));
	ASSERT_EQ(0, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(2, 8), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LDI (rr), r
TEST(Z80_8bitLoadTest, LoadAddressAndIncrement)
{
	LoadAddressAndIncrement(0x22, Register16bit::HL, Register8bit::A);
}
#pragma endregion

void Load8bitRegFromIOAddress(uint8_t opcode, Register8bit dest)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0, 0x12);
	mmu.Write8bitToMemory(0xFF00 + mmu.Read8bitFromMemory(0), 0xDA);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, z80.GetRegisters().Read(dest)) << "Register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(dest));
	ASSERT_EQ(1, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(3, 12), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LDIO r, (n)
TEST(Z80_8bitLoadTest, LoadRegAFromIOAddressPlusValue)
{
	Load8bitRegFromIOAddress(0xF0, Register8bit::A);
}
#pragma endregion

void LoadIOAddressFrom8bitReg(uint8_t opcode, Register8bit source)
{
	TesterMMU mmu;
	Z80 z80(mmu);

	mmu.Write8bitToMemory(0, 0x12);
	z80.GetRegisters().Write(source, 0xDA);
	z80.Execute(opcode);

	ASSERT_EQ(0xDA, mmu.Read8bitFromMemory(0xFF00 + 0x12)) << "Memory read unexpected value: "
		<< static_cast<size_t>(mmu.Read8bitFromMemory(0xFF00 + 0x12));
	ASSERT_EQ(1, z80.GetRegisters().Read(Register16bit::PC)) << "PC read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register16bit::PC));
	ASSERT_EQ(Clock(3, 12), z80.GetClock()) << "Unexpected operation duration: "
		<< static_cast<size_t>(z80.GetClock().GetTicks());
	ASSERT_EQ(0, z80.GetRegisters().Read(Register8bit::F)) << "Flags register read unexpected value: "
		<< static_cast<size_t>(z80.GetRegisters().Read(Register8bit::F));
}

#pragma region LDIO (n), r
TEST(Z80_8bitLoadTest, LoadIOAddressPlusValueFromRegA)
{
	LoadIOAddressFrom8bitReg(0xE0, Register8bit::A);
}
#pragma endregion
