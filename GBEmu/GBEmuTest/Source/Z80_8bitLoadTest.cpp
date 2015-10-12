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

TEST(Z80Test, LoadRegAFromValue)
{
	Load8bitRegFromValue(0x3E, Register8bit::A);
}

TEST(Z80Test, LoadRegBFromValue)
{
	Load8bitRegFromValue(0x06, Register8bit::B);
}

TEST(Z80Test, LoadRegCFromValue)
{
	Load8bitRegFromValue(0x0E, Register8bit::C);
}

TEST(Z80Test, LoadRegDFromValue)
{
	Load8bitRegFromValue(0x16, Register8bit::D);
}

TEST(Z80Test, LoadRegEFromValue)
{
	Load8bitRegFromValue(0x1E, Register8bit::E);
}

TEST(Z80Test, LoadRegHFromValue)
{
	Load8bitRegFromValue(0x26, Register8bit::H);
}

TEST(Z80Test, LoadRegLFromValue)
{
	Load8bitRegFromValue(0x2E, Register8bit::L);
}

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

TEST(Z80Test, LoadRegAFromRegA)
{
	Load8bitRegFrom8bitReg(0x7F, Register8bit::A, Register8bit::A);
}

TEST(Z80Test, LoadRegAFromRegB)
{
	Load8bitRegFrom8bitReg(0x78, Register8bit::A, Register8bit::B);
}

TEST(Z80Test, LoadRegAFromRegC)
{
	Load8bitRegFrom8bitReg(0x79, Register8bit::A, Register8bit::C);
}

TEST(Z80Test, LoadRegAFromRegD)
{
	Load8bitRegFrom8bitReg(0x7A, Register8bit::A, Register8bit::D);
}

TEST(Z80Test, LoadRegAFromRegE)
{
	Load8bitRegFrom8bitReg(0x7B, Register8bit::A, Register8bit::E);
}

TEST(Z80Test, LoadRegAFromRegH)
{
	Load8bitRegFrom8bitReg(0x7C, Register8bit::A, Register8bit::H);
}

TEST(Z80Test, LoadRegAFromRegL)
{
	Load8bitRegFrom8bitReg(0x7D, Register8bit::A, Register8bit::L);
}

TEST(Z80Test, LoadRegBFromRegA)
{
	Load8bitRegFrom8bitReg(0x47, Register8bit::B, Register8bit::A);
}

TEST(Z80Test, LoadRegBFromRegB)
{
	Load8bitRegFrom8bitReg(0x40, Register8bit::B, Register8bit::B);
}

TEST(Z80Test, LoadRegBFromRegC)
{
	Load8bitRegFrom8bitReg(0x41, Register8bit::B, Register8bit::C);
}

TEST(Z80Test, LoadRegBFromRegD)
{
	Load8bitRegFrom8bitReg(0x42, Register8bit::B, Register8bit::D);
}

TEST(Z80Test, LoadRegBFromRegE)
{
	Load8bitRegFrom8bitReg(0x43, Register8bit::B, Register8bit::E);
}

TEST(Z80Test, LoadRegBFromRegH)
{
	Load8bitRegFrom8bitReg(0x44, Register8bit::B, Register8bit::H);
}

TEST(Z80Test, LoadRegBFromRegL)
{
	Load8bitRegFrom8bitReg(0x45, Register8bit::B, Register8bit::L);
}

TEST(Z80Test, LoadRegCFromRegA)
{
	Load8bitRegFrom8bitReg(0x4F, Register8bit::C, Register8bit::A);
}

TEST(Z80Test, LoadRegCFromRegB)
{
	Load8bitRegFrom8bitReg(0x48, Register8bit::C, Register8bit::B);
}

TEST(Z80Test, LoadRegCFromRegC)
{
	Load8bitRegFrom8bitReg(0x49, Register8bit::C, Register8bit::C);
}

TEST(Z80Test, LoadRegCFromRegD)
{
	Load8bitRegFrom8bitReg(0x4A, Register8bit::C, Register8bit::D);
}

TEST(Z80Test, LoadRegCFromRegE)
{
	Load8bitRegFrom8bitReg(0x4B, Register8bit::C, Register8bit::E);
}

TEST(Z80Test, LoadRegCFromRegH)
{
	Load8bitRegFrom8bitReg(0x4C, Register8bit::C, Register8bit::H);
}

TEST(Z80Test, LoadRegCFromRegL)
{
	Load8bitRegFrom8bitReg(0x4D, Register8bit::C, Register8bit::L);
}

TEST(Z80Test, LoadRegDFromRegA)
{
	Load8bitRegFrom8bitReg(0x57, Register8bit::D, Register8bit::A);
}

TEST(Z80Test, LoadRegDFromRegB)
{
	Load8bitRegFrom8bitReg(0x50, Register8bit::D, Register8bit::B);
}

TEST(Z80Test, LoadRegDFromRegC)
{
	Load8bitRegFrom8bitReg(0x51, Register8bit::D, Register8bit::C);
}

TEST(Z80Test, LoadRegDFromRegD)
{
	Load8bitRegFrom8bitReg(0x52, Register8bit::D, Register8bit::D);
}

TEST(Z80Test, LoadRegDFromRegE)
{
	Load8bitRegFrom8bitReg(0x53, Register8bit::D, Register8bit::E);
}

TEST(Z80Test, LoadRegDFromRegH)
{
	Load8bitRegFrom8bitReg(0x54, Register8bit::D, Register8bit::H);
}

TEST(Z80Test, LoadRegDFromRegL)
{
	Load8bitRegFrom8bitReg(0x55, Register8bit::D, Register8bit::L);
}

TEST(Z80Test, LoadRegEFromRegA)
{
	Load8bitRegFrom8bitReg(0x5F, Register8bit::E, Register8bit::A);
}

TEST(Z80Test, LoadRegEFromRegB)
{
	Load8bitRegFrom8bitReg(0x58, Register8bit::E, Register8bit::B);
}

TEST(Z80Test, LoadRegEFromRegC)
{
	Load8bitRegFrom8bitReg(0x59, Register8bit::E, Register8bit::C);
}

TEST(Z80Test, LoadRegEFromRegD)
{
	Load8bitRegFrom8bitReg(0x5A, Register8bit::E, Register8bit::D);
}

TEST(Z80Test, LoadRegEFromRegE)
{
	Load8bitRegFrom8bitReg(0x5B, Register8bit::E, Register8bit::E);
}

TEST(Z80Test, LoadRegEFromRegH)
{
	Load8bitRegFrom8bitReg(0x5C, Register8bit::E, Register8bit::H);
}

TEST(Z80Test, LoadRegEFromRegL)
{
	Load8bitRegFrom8bitReg(0x5D, Register8bit::E, Register8bit::L);
}

TEST(Z80Test, LoadRegHFromRegA)
{
	Load8bitRegFrom8bitReg(0x67, Register8bit::H, Register8bit::A);
}

TEST(Z80Test, LoadRegHFromRegB)
{
	Load8bitRegFrom8bitReg(0x60, Register8bit::H, Register8bit::B);
}

TEST(Z80Test, LoadRegHFromRegC)
{
	Load8bitRegFrom8bitReg(0x61, Register8bit::H, Register8bit::C);
}

TEST(Z80Test, LoadRegHFromRegD)
{
	Load8bitRegFrom8bitReg(0x62, Register8bit::H, Register8bit::D);
}

TEST(Z80Test, LoadRegHFromRegE)
{
	Load8bitRegFrom8bitReg(0x63, Register8bit::H, Register8bit::E);
}

TEST(Z80Test, LoadRegHFromRegH)
{
	Load8bitRegFrom8bitReg(0x64, Register8bit::H, Register8bit::H);
}

TEST(Z80Test, LoadRegHFromRegL)
{
	Load8bitRegFrom8bitReg(0x65, Register8bit::H, Register8bit::L);
}

TEST(Z80Test, LoadRegLFromRegA)
{
	Load8bitRegFrom8bitReg(0x6F, Register8bit::L, Register8bit::A);
}

TEST(Z80Test, LoadRegLFromRegB)
{
	Load8bitRegFrom8bitReg(0x68, Register8bit::L, Register8bit::B);
}

TEST(Z80Test, LoadRegLFromRegC)
{
	Load8bitRegFrom8bitReg(0x69, Register8bit::L, Register8bit::C);
}

TEST(Z80Test, LoadRegLFromRegD)
{
	Load8bitRegFrom8bitReg(0x6A, Register8bit::L, Register8bit::D);
}

TEST(Z80Test, LoadRegLFromRegE)
{
	Load8bitRegFrom8bitReg(0x6B, Register8bit::L, Register8bit::E);
}

TEST(Z80Test, LoadRegLFromRegH)
{
	Load8bitRegFrom8bitReg(0x6C, Register8bit::L, Register8bit::H);
}

TEST(Z80Test, LoadRegLFromRegL)
{
	Load8bitRegFrom8bitReg(0x6D, Register8bit::L, Register8bit::L);
}

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

TEST(Z80Test, LoadRegAFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x7E, Register8bit::A, Register16bit::HL);
}

TEST(Z80Test, LoadRegBFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x46, Register8bit::B, Register16bit::HL);
}

TEST(Z80Test, LoadRegCFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x4E, Register8bit::C, Register16bit::HL);
}

TEST(Z80Test, LoadRegDFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x56, Register8bit::D, Register16bit::HL);
}

TEST(Z80Test, LoadRegEFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x5E, Register8bit::E, Register16bit::HL);
}

TEST(Z80Test, LoadRegHFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x66, Register8bit::H, Register16bit::HL);
}

TEST(Z80Test, LoadRegLFromAddressInRegHL)
{
	Load8bitRegFromAddressIn16bitReg(0x6E, Register8bit::L, Register16bit::HL);
}

TEST(Z80Test, LoadRegAFromAddressInRegBC)
{
	Load8bitRegFromAddressIn16bitReg(0x0A, Register8bit::A, Register16bit::BC);
}

TEST(Z80Test, LoadRegAFromAddressInRegDE)
{
	Load8bitRegFromAddressIn16bitReg(0x1A, Register8bit::A, Register16bit::DE);
}

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

TEST(Z80Test, LoadAddressInRegHLFromRegB)
{
	LoadAddressIn16bitRegFrom8bitReg(0x70, Register16bit::HL, Register8bit::B);
}

TEST(Z80Test, LoadAddressInRegHLFromRegC)
{
	LoadAddressIn16bitRegFrom8bitReg(0x71, Register16bit::HL, Register8bit::C);
}

TEST(Z80Test, LoadAddressInRegHLFromRegD)
{
	LoadAddressIn16bitRegFrom8bitReg(0x72, Register16bit::HL, Register8bit::D);
}

TEST(Z80Test, LoadAddressInRegHLFromRegE)
{
	LoadAddressIn16bitRegFrom8bitReg(0x73, Register16bit::HL, Register8bit::E);
}

TEST(Z80Test, LoadAddressInRegHLFromRegH)
{
	LoadAddressIn16bitRegFrom8bitReg(0x74, Register16bit::HL, Register8bit::H);
}

TEST(Z80Test, LoadAddressInRegHLFromRegL)
{
	LoadAddressIn16bitRegFrom8bitReg(0x75, Register16bit::HL, Register8bit::L);
}

TEST(Z80Test, LoadAddressInRegBCFromRegA)
{
	LoadAddressIn16bitRegFrom8bitReg(0x02, Register16bit::BC, Register8bit::A);
}

TEST(Z80Test, LoadAddressInRegDEFromRegA)
{
	LoadAddressIn16bitRegFrom8bitReg(0x12, Register16bit::DE, Register8bit::A);
}

TEST(Z80Test, LoadAddressInRegHLFromRegA)
{
	LoadAddressIn16bitRegFrom8bitReg(0x77, Register16bit::HL, Register8bit::A);
}

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

TEST(Z80Test, LoadAddressInRegHLFromValue)
{
	LoadAddressIn16bitRegFromValue(0x36, Register16bit::HL);
}

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

TEST(Z80Test, LoadRegAFromValueInAddress)
{
	Load8bitRegFromValueInAddress(0xFA, Register8bit::A);
}

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

TEST(Z80Test, LoadAddressFromRegA)
{
	LoadAddressFrom8bitReg(0xEA, Register8bit::A);
}

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

TEST(Z80Test, LoadRegAFromIOAddressPlusReg)
{
	Load8bitRegFromIOAddress(0xF2, Register8bit::A, Register8bit::C);
}

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

TEST(Z80Test, LoadIOAddressPlusRegFromRegA)
{
	LoadIOAddressFrom8bitReg(0xE2, Register8bit::C, Register8bit::A);
}

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

TEST(Z80Test, LoadRegAAndDecrement)
{
	Load8bitRegAndDecrement(0x3A, Register8bit::A, Register16bit::HL);
}

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

TEST(Z80Test, LoadAddressAndDecrement)
{
	LoadAddressAndDecrement(0x32, Register16bit::HL, Register8bit::A);
}

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

TEST(Z80Test, LoadRegAAndIncrement)
{
	Load8bitRegAndIncrement(0x2A, Register8bit::A, Register16bit::HL);
}

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

TEST(Z80Test, LoadAddressAndIncrement)
{
	LoadAddressAndIncrement(0x22, Register16bit::HL, Register8bit::A);
}

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

TEST(Z80Test, LoadRegAFromIOAddressPlusValue)
{
	Load8bitRegFromIOAddress(0xF0, Register8bit::A);
}

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

TEST(Z80Test, LoadIOAddressPlusValueFromRegA)
{
	LoadIOAddressFrom8bitReg(0xE0, Register8bit::A);
}
