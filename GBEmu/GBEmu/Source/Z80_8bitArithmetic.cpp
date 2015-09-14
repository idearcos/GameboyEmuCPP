#include "Z80.h"

// ADD A, n
Clock Z80::Add(uint8_t value)
{
	registers.ResetFlags();
	if (((registers.Read(Register8bit::A) & 0x0F) + (value & 0x0F)) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint16_t result = registers.Read(Register8bit::A) + value;
	if (result & 0x0100)
	{
		registers.SetFlag(Flags::Carry);
	}
	if ((result & 0xFF) == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 7);
}

// ADD A, r
Clock Z80::Add(Register8bit reg)
{
	Add(registers.Read(reg));
	return Clock(1, 4);
}

// ADD A, (dd)
Clock Z80::Add(Register16bit source_addr)
{
	Add(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// ADC A, n
Clock Z80::AddPlusCarry(uint8_t value)
{
	registers.ResetFlags();
	if (((registers.Read(Register8bit::A) & 0x0F) + (value & 0x0F) + (registers.Read(Register8bit::A) ? 1 : 0)) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint16_t result = registers.Read(Register8bit::A) + value + registers.Read(Register8bit::A) ? 1 : 0;
	if (result & 0x0100)
	{
		registers.SetFlag(Flags::Carry);
	}
	if ((result & 0xFF) == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 7);
}

// ADC A, r
Clock Z80::AddPlusCarry(Register8bit reg)
{
	AddPlusCarry(registers.Read(reg));
	return Clock(1, 4);
}

// ADC A, (dd)
Clock Z80::AddPlusCarry(Register16bit source_addr)
{
	AddPlusCarry(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// SUB A, n
Clock Z80::Sub(uint8_t value)
{
	registers.ResetFlags();
	registers.SetFlag(Flags::Subtract);
	if (((registers.Read(Register8bit::A) & 0x0F) - (value & 0x0F)) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint16_t result = registers.Read(Register8bit::A) - value;
	if (result & 0x0100)
	{
		registers.SetFlag(Flags::Carry);
	}
	if ((result & 0xFF) == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 7);
}

// SUB A, r
Clock Z80::Sub(Register8bit reg)
{
	Sub(registers.Read(reg));
	return Clock(1, 4);
}

// SUB A, (dd)
Clock Z80::Sub(Register16bit source_addr)
{
	Sub(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// SBC A, n
Clock Z80::SubMinusCarry(uint8_t value)
{
	registers.ResetFlags();
	registers.SetFlag(Flags::Subtract);
	if (((registers.Read(Register8bit::A) & 0x0F) - (value & 0x0F) - (registers.Read(Register8bit::A) ? 1 : 0)) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint16_t result = registers.Read(Register8bit::A) - value - (registers.Read(Register8bit::A) ? 1 : 0);
	if (result & 0x0100)
	{
		registers.SetFlag(Flags::Carry);
	}
	if ((result & 0xFF) == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(Register8bit::A, static_cast<uint8_t>(result & 0xFF));

	return Clock(2, 7);
}

// SBC A, r
Clock Z80::SubMinusCarry(Register8bit reg)
{
	SubMinusCarry(registers.Read(reg));
	return Clock(1, 4);
}

// SBC A, (dd)
Clock Z80::SubMinusCarry(Register16bit source_addr)
{
	SubMinusCarry(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// AND A, n
Clock Z80::And(uint8_t value)
{
	registers.ResetFlags();
	registers.SetFlag(Flags::HalfCarry);

	uint8_t result = registers.Read(Register8bit::A) & value;
	if (result == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(Register8bit::A, result);

	return Clock(2, 7);
}

// AND A, r
Clock Z80::And(Register8bit reg)
{
	And(registers.Read(reg));
	return Clock(1, 4);
}

// AND A, (dd)
Clock Z80::And(Register16bit source_addr)
{
	And(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// OR A, n
Clock Z80::Or(uint8_t value)
{
	registers.ResetFlags();

	uint8_t result = registers.Read(Register8bit::A) | value;
	if (result == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(Register8bit::A, result);

	return Clock(2, 7);
}

// OR A, r
Clock Z80::Or(Register8bit reg)
{
	Or(registers.Read(reg));
	return Clock(1, 4);
}

// OR A, (dd)
Clock Z80::Or(Register16bit source_addr)
{
	Or(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// XOR A, n
Clock Z80::Xor(uint8_t value)
{
	registers.ResetFlags();

	uint8_t result = ~(registers.Read(Register8bit::A) & value);
	if (result == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(Register8bit::A, result);

	return Clock(2, 7);
}

// XOR A, r
Clock Z80::Xor(Register8bit reg)
{
	Xor(registers.Read(reg));
	return Clock(1, 4);
}

// XOR A, (dd)
Clock Z80::Xor(Register16bit source_addr)
{
	Xor(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// CP A, n
Clock Z80::Compare(uint8_t value)
{
	registers.ResetFlags();
	registers.SetFlag(Flags::Subtract);
	if (((registers.Read(Register8bit::A) & 0x0F) - (value & 0x0F)) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint16_t result = registers.Read(Register8bit::A) - value;
	if (result & 0x0100)
	{
		registers.SetFlag(Flags::Carry);
	}
	if ((result & 0xFF) == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	return Clock(2, 7);
}

// CP A, r
Clock Z80::Compare(Register8bit reg)
{
	Compare(registers.Read(reg));
	return Clock(1, 4);
}

// CP A, (dd)
Clock Z80::Compare(Register16bit source_addr)
{
	Compare(mmu.Read8bitFromMemory(registers.Read(source_addr)));
	return Clock(2, 7);
}

// INC r
// (Z80 p.163)
Clock Z80::Increment(Register8bit reg)
{
	{auto carry_was_set = registers.IsFlagSet(Flags::Carry);
	registers.ResetFlags();
	if (carry_was_set)
	{
		registers.SetFlag(Flags::Carry);
	}}
	if (((registers.Read(reg) & 0x0F) + 1) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint8_t result = registers.Read(reg) + 1;
	if (result == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(reg, result);

	return Clock(1, 4);
}

// INC (dd)
// (Z80 p.165)
Clock Z80::Increment(Register16bit reg_addr)
{
	{auto carry_was_set = registers.IsFlagSet(Flags::Carry);
	registers.ResetFlags();
	if (carry_was_set)
	{
		registers.SetFlag(Flags::Carry);
	}}
	if (((mmu.Read8bitFromMemory(registers.Read(reg_addr)) & 0x0F) + 1) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint8_t result = mmu.Read8bitFromMemory(registers.Read(reg_addr)) + 1;
	if (result == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	mmu.Write8bitToMemory(registers.Read(reg_addr), result);

	return Clock(3, 11);
}

// DEC r
// (Z80 p.168)
Clock Z80::Decrement(Register8bit reg)
{
	{auto carry_was_set = registers.IsFlagSet(Flags::Carry);
	registers.ResetFlags();
	registers.SetFlag(Flags::Subtract);
	if (carry_was_set)
	{
		registers.SetFlag(Flags::Carry);
	}}
	if (((registers.Read(reg) & 0x0F) - 1) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint8_t result = registers.Read(reg) - 1;
	if (result == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	registers.Write(reg, result);

	return Clock(1, 4);
}

// DEC (dd)
// (Z80 p.168)
Clock Z80::Decrement(Register16bit reg_addr)
{
	{auto carry_was_set = registers.IsFlagSet(Flags::Carry);
	registers.ResetFlags();
	registers.SetFlag(Flags::Subtract);
	if (carry_was_set)
	{
		registers.SetFlag(Flags::Carry);
	}}
	if (((mmu.Read8bitFromMemory(registers.Read(reg_addr)) & 0x0F) - 1) & 0x10)
	{
		registers.SetFlag(Flags::HalfCarry);
	}
	uint8_t result = mmu.Read8bitFromMemory(registers.Read(reg_addr)) - 1;
	if (result == 0)
	{
		registers.SetFlag(Flags::Zero);
	}

	mmu.Write8bitToMemory(registers.Read(reg_addr), result);

	return Clock(3, 11);
}
