#include "gtest/gtest.h"
#include "Registers.h"

TEST(RegistersTest, Constructor)
{
	Registers registers;

	ASSERT_EQ(0x00, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(0x00, registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0x00, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0x00, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0x00, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0x00, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0x00, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0x00, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(0x0000, registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0x0000, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0x0000, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0x0000, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0x0000, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0x0000, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}

TEST(RegistersTest, 8bitWriteRegA)
{
	Registers registers;

	for (size_t i = 0; i <= UINT8_MAX; i++)
	{
		registers.Write(Register8bit::A, i);

		ASSERT_EQ(i, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(i << 8, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 8bitWriteRegF)
{
	Registers registers;

	for (size_t i = 0; i <= UINT8_MAX; i++)
	{
		ASSERT_THROW(registers.Write(Register8bit::F, i), std::logic_error);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 8bitWriteRegB)
{
	Registers registers;

	for (size_t i = 0; i <= UINT8_MAX; i++)
	{
		registers.Write(Register8bit::B, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(i, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(i << 8, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 8bitWriteRegC)
{
	Registers registers;

	for (size_t i = 0; i <= UINT8_MAX; i++)
	{
		registers.Write(Register8bit::C, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(i, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(i, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 8bitWriteRegD)
{
	Registers registers;

	for (size_t i = 0; i <= UINT8_MAX; i++)
	{
		registers.Write(Register8bit::D, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(i, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(i << 8, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 8bitWriteRegE)
{
	Registers registers;

	for (size_t i = 0; i <= UINT8_MAX; i++)
	{
		registers.Write(Register8bit::E, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(i, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(i, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 8bitWriteRegH)
{
	Registers registers;

	for (size_t i = 0; i <= UINT8_MAX; i++)
	{
		registers.Write(Register8bit::H, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(i, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(i << 8, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 8bitWriteRegL)
{
	Registers registers;

	for (size_t i = 0; i <= UINT8_MAX; i++)
	{
		registers.Write(Register8bit::L, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(i, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(i, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 16bitWriteRegAF)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Write(Register16bit::AF, i);

		ASSERT_EQ(i >> 8, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(i & 0xFF, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(i, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 16bitWriteRegBC)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Write(Register16bit::BC, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(i >> 8, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(i & 0xFF, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(i, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 16bitWriteRegDE)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Write(Register16bit::DE, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(i >> 8, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(i & 0xFF, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(i, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 16bitWriteRegHL)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Write(Register16bit::HL, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(i >> 8, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(i & 0xFF, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(i, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 16bitWriteRegPC)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Write(Register16bit::PC, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(i, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, 16bitWriteRegSP)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Write(Register16bit::SP, i);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(i, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, IncrementRegAF)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Increment(Register16bit::AF);

		ASSERT_EQ(((i + 1) >> 8) & 0xFF, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ((i + 1) & 0xFF, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ((i + 1) & 0xFFFF, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, IncrementRegBC)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Increment(Register16bit::BC);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(((i + 1) >> 8) & 0xFF, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ((i + 1) & 0xFF, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ((i + 1) & 0xFFFF, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, IncrementRegDE)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Increment(Register16bit::DE);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(((i + 1) >> 8) & 0xFF, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ((i + 1) & 0xFF, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ((i + 1) & 0xFFFF, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, IncrementRegHL)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Increment(Register16bit::HL);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(((i + 1) >> 8) & 0xFF, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ((i + 1) & 0xFF, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ((i + 1) & 0xFFFF, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, IncrementRegPC)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Increment(Register16bit::PC);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ((i + 1) & 0xFFFF, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, IncrementRegSP)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Increment(Register16bit::SP);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ((i + 1) & 0xFFFF, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, DecrementRegAF)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Decrement(Register16bit::AF);

		ASSERT_EQ(((UINT16_MAX - i) >> 8) & 0xFF, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ((UINT16_MAX - i) & 0xFF, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ((UINT16_MAX - i) & 0xFFFF, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, DecrementRegBC)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Decrement(Register16bit::BC);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(((UINT16_MAX - i) >> 8) & 0xFF, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ((UINT16_MAX - i) & 0xFF, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ((UINT16_MAX - i) & 0xFFFF, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, DecrementRegDE)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Decrement(Register16bit::DE);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(((UINT16_MAX - i) >> 8) & 0xFF, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ((UINT16_MAX - i) & 0xFF, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ((UINT16_MAX - i) & 0xFFFF, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, DecrementRegHL)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Decrement(Register16bit::HL);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(((UINT16_MAX - i) >> 8) & 0xFF, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ((UINT16_MAX - i) & 0xFF, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ((UINT16_MAX - i) & 0xFFFF, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, DecrementRegPC)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Decrement(Register16bit::PC);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ((UINT16_MAX - i) & 0xFFFF, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, DecrementRegSP)
{
	Registers registers;

	for (size_t i = 0; i <= UINT16_MAX; i++)
	{
		registers.Decrement(Register16bit::SP);

		ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Testing input value " << i << ", Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
		ASSERT_EQ(0, registers.Read(Register8bit::F)) << "Testing input value " << i << ", Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
		ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Testing input value " << i << ", Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
		ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Testing input value " << i << ", Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
		ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Testing input value " << i << ", Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
		ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Testing input value " << i << ", Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
		ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Testing input value " << i << ", Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
		ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Testing input value " << i << ", Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
		ASSERT_EQ(0, registers.Read(Register16bit::AF)) << "Testing input value " << i << ", Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
		ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Testing input value " << i << ", Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
		ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Testing input value " << i << ", Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
		ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Testing input value " << i << ", Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
		ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Testing input value " << i << ", Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
		ASSERT_EQ((UINT16_MAX - i) & 0xFFFF, registers.Read(Register16bit::SP)) << "Testing input value " << i << ", Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
	}
}

TEST(RegistersTest, SetFlagZero)
{
	Registers registers;
	registers.SetFlag(Flags::Zero, true);

	ASSERT_TRUE(registers.IsFlagSet(Flags::Zero));
	ASSERT_FALSE(registers.IsFlagSet(Flags::Subtract));
	ASSERT_FALSE(registers.IsFlagSet(Flags::HalfCarry));
	ASSERT_FALSE(registers.IsFlagSet(Flags::Carry));

	ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(static_cast<std::underlying_type_t<Flags>>(Flags::Zero), registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(static_cast<std::underlying_type_t<Flags>>(Flags::Zero), registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}

TEST(RegistersTest, SetFlagSubtract)
{
	Registers registers;
	registers.SetFlag(Flags::Subtract, true);

	ASSERT_FALSE(registers.IsFlagSet(Flags::Zero));
	ASSERT_TRUE(registers.IsFlagSet(Flags::Subtract));
	ASSERT_FALSE(registers.IsFlagSet(Flags::HalfCarry));
	ASSERT_FALSE(registers.IsFlagSet(Flags::Carry));

	ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(static_cast<std::underlying_type_t<Flags>>(Flags::Subtract), registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(static_cast<std::underlying_type_t<Flags>>(Flags::Subtract), registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}

TEST(RegistersTest, SetFlagHalfCarry)
{
	Registers registers;
	registers.SetFlag(Flags::HalfCarry, true);

	ASSERT_FALSE(registers.IsFlagSet(Flags::Zero));
	ASSERT_FALSE(registers.IsFlagSet(Flags::Subtract));
	ASSERT_TRUE(registers.IsFlagSet(Flags::HalfCarry));
	ASSERT_FALSE(registers.IsFlagSet(Flags::Carry));

	ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(static_cast<std::underlying_type_t<Flags>>(Flags::HalfCarry), registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(static_cast<std::underlying_type_t<Flags>>(Flags::HalfCarry), registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}

TEST(RegistersTest, SetFlagCarry)
{
	Registers registers;
	registers.SetFlag(Flags::Carry, true);

	ASSERT_FALSE(registers.IsFlagSet(Flags::Zero));
	ASSERT_FALSE(registers.IsFlagSet(Flags::Subtract));
	ASSERT_FALSE(registers.IsFlagSet(Flags::HalfCarry));
	ASSERT_TRUE(registers.IsFlagSet(Flags::Carry));

	ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(static_cast<std::underlying_type_t<Flags>>(Flags::Carry), registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(static_cast<std::underlying_type_t<Flags>>(Flags::Carry), registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}

TEST(RegistersTest, ResetFlagZero)
{
	Registers registers;
	registers.SetFlag(Flags::Zero, true);
	registers.SetFlag(Flags::Subtract, true);
	registers.SetFlag(Flags::HalfCarry, true);
	registers.SetFlag(Flags::Carry, true);
	registers.SetFlag(Flags::Zero, false);

	ASSERT_FALSE(registers.IsFlagSet(Flags::Zero));
	ASSERT_TRUE(registers.IsFlagSet(Flags::Subtract));
	ASSERT_TRUE(registers.IsFlagSet(Flags::HalfCarry));
	ASSERT_TRUE(registers.IsFlagSet(Flags::Carry));

	const auto reg_f_value = static_cast<std::underlying_type_t<Flags>>(Flags::Subtract)
		| static_cast<std::underlying_type_t<Flags>>(Flags::HalfCarry)
		| static_cast<std::underlying_type_t<Flags>>(Flags::Carry);

	ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(reg_f_value, registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(reg_f_value, registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}

TEST(RegistersTest, ResetFlagSubtract)
{
	Registers registers;
	registers.SetFlag(Flags::Zero, true);
	registers.SetFlag(Flags::Subtract, true);
	registers.SetFlag(Flags::HalfCarry, true);
	registers.SetFlag(Flags::Carry, true);
	registers.SetFlag(Flags::Subtract, false);

	ASSERT_TRUE(registers.IsFlagSet(Flags::Zero));
	ASSERT_FALSE(registers.IsFlagSet(Flags::Subtract));
	ASSERT_TRUE(registers.IsFlagSet(Flags::HalfCarry));
	ASSERT_TRUE(registers.IsFlagSet(Flags::Carry));

	const auto reg_f_value = static_cast<std::underlying_type_t<Flags>>(Flags::Zero)
		| static_cast<std::underlying_type_t<Flags>>(Flags::HalfCarry)
		| static_cast<std::underlying_type_t<Flags>>(Flags::Carry);

	ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(reg_f_value, registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(reg_f_value, registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}

TEST(RegistersTest, ResetFlagHalfCarry)
{
	Registers registers;
	registers.SetFlag(Flags::Zero, true);
	registers.SetFlag(Flags::Subtract, true);
	registers.SetFlag(Flags::HalfCarry, true);
	registers.SetFlag(Flags::Carry, true);
	registers.SetFlag(Flags::HalfCarry, false);

	ASSERT_TRUE(registers.IsFlagSet(Flags::Zero));
	ASSERT_TRUE(registers.IsFlagSet(Flags::Subtract));
	ASSERT_FALSE(registers.IsFlagSet(Flags::HalfCarry));
	ASSERT_TRUE(registers.IsFlagSet(Flags::Carry));

	const auto reg_f_value = static_cast<std::underlying_type_t<Flags>>(Flags::Zero)
		| static_cast<std::underlying_type_t<Flags>>(Flags::Subtract)
		| static_cast<std::underlying_type_t<Flags>>(Flags::Carry);

	ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(reg_f_value, registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(reg_f_value, registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}

TEST(RegistersTest, ResetFlagCarry)
{
	Registers registers;
	registers.SetFlag(Flags::Zero, true);
	registers.SetFlag(Flags::Subtract, true);
	registers.SetFlag(Flags::HalfCarry, true);
	registers.SetFlag(Flags::Carry, true);
	registers.SetFlag(Flags::Carry, false);

	ASSERT_TRUE(registers.IsFlagSet(Flags::Zero));
	ASSERT_TRUE(registers.IsFlagSet(Flags::Subtract));
	ASSERT_TRUE(registers.IsFlagSet(Flags::HalfCarry));
	ASSERT_FALSE(registers.IsFlagSet(Flags::Carry));

	const auto reg_f_value = static_cast<std::underlying_type_t<Flags>>(Flags::Zero)
		| static_cast<std::underlying_type_t<Flags>>(Flags::Subtract)
		| static_cast<std::underlying_type_t<Flags>>(Flags::HalfCarry);

	ASSERT_EQ(0, registers.Read(Register8bit::A)) << "Register A read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::A));
	ASSERT_EQ(reg_f_value, registers.Read(Register8bit::F)) << "Register F read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::F));
	ASSERT_EQ(0, registers.Read(Register8bit::B)) << "Register B read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::B));
	ASSERT_EQ(0, registers.Read(Register8bit::C)) << "Register C read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::C));
	ASSERT_EQ(0, registers.Read(Register8bit::D)) << "Register D read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::D));
	ASSERT_EQ(0, registers.Read(Register8bit::E)) << "Register E read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::E));
	ASSERT_EQ(0, registers.Read(Register8bit::H)) << "Register H read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::H));
	ASSERT_EQ(0, registers.Read(Register8bit::L)) << "Register L read unexpected value: " << static_cast<size_t>(registers.Read(Register8bit::L));
	ASSERT_EQ(reg_f_value, registers.Read(Register16bit::AF)) << "Register AF read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::AF));
	ASSERT_EQ(0, registers.Read(Register16bit::BC)) << "Register BC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::BC));
	ASSERT_EQ(0, registers.Read(Register16bit::DE)) << "Register DE read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::DE));
	ASSERT_EQ(0, registers.Read(Register16bit::HL)) << "Register HL read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::HL));
	ASSERT_EQ(0, registers.Read(Register16bit::PC)) << "Register PC read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::PC));
	ASSERT_EQ(0, registers.Read(Register16bit::SP)) << "Register SP read unexpected value: " << static_cast<size_t>(registers.Read(Register16bit::SP));
}
