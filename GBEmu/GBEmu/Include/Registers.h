#pragma once

#include <cstdint>

enum class Register8bit : uint8_t
{	
	B = 0,
	C = 1,
	D = 2,
	E = 3,
	H = 4,
	L = 5,
	A = 7
};

enum class Register16bit
{
	AF,
	BC,
	DE,
	HL,
	PC,
	SP
};

// Z S H C X X X X
enum class Flags : uint8_t
{
	Zero = 0x80,
	Subtract = 0x40,
	HalfCarry = 0x20,
	Carry = 0x10,
};

class Registers
{
public:
	Registers() = default;
	~Registers() = default;

	uint8_t Read(Register8bit reg) const;
	void Write(Register8bit reg, uint8_t value);
	uint16_t Read(Register16bit reg) const;
	void Write(Register16bit reg, uint16_t value);

	void Increment(Register16bit reg);
	void Decrement(Register16bit reg);

	void SetFlag(Flags flag, bool bSet);
	bool IsFlagSet(Flags flag) const;

private:
	uint8_t a_{ 0 };
	uint8_t f_{ 0 };
	uint8_t b_{ 0 };
	uint8_t c_{ 0 };
	uint8_t d_{ 0 };
	uint8_t e_{ 0 };
	uint8_t h_{ 0 };
	uint8_t l_{ 0 };
	uint8_t i_{ 0 };
	uint8_t r_{ 0 };

	uint16_t sp_{ 0 };
	uint16_t pc_{ 0 };
};
