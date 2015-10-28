#pragma once

#include <cstdint>
#include <iostream>

enum class Register8bit : uint8_t
{	
	B,
	C,
	D,
	E,
	H,
	L,
	A,
	F
};

std::ostream& operator << (std::ostream& os, const Register8bit& reg);

enum class Register16bit : uint8_t
{
	AF,
	BC,
	DE,
	HL,
	PC,
	SP
};

std::ostream& operator << (std::ostream& os, const Register16bit& reg);

// Z S H C X X X X
enum class Flags : uint8_t
{
	Zero = 0x80,
	Subtract = 0x40,
	HalfCarry = 0x20,
	Carry = 0x10,
};

std::ostream& operator << (std::ostream& os, const Flags& flag);

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
	union
	{
		struct
		{
			uint8_t f_;
			uint8_t a_;
		};
		uint16_t af_{ 0 };
	};
	union
	{
		struct
		{
			uint8_t c_;
			uint8_t b_;
		};
		uint16_t bc_{ 0 };
	};
	union
	{
		struct
		{
			uint8_t e_;
			uint8_t d_;
		};
		uint16_t de_{ 0 };
	};
	union
	{
		struct
		{
			uint8_t l_;
			uint8_t h_;
		};
		uint16_t hl_{ 0 };
	};

	uint16_t sp_{ 0 };
	uint16_t pc_{ 0 };
};
