#include "Registers.h"

#include <stdexcept>
#include <type_traits>

uint8_t Registers::Read(Register8bit reg) const
{
	uint8_t value{ 0 };
	switch (reg)
	{
	case Register8bit::A:
		value = a_;
		break;
	case Register8bit::B:
		value = b_;
		break;
	case Register8bit::C:
		value = c_;
		break;
	case Register8bit::D:
		value = d_;
		break;
	case Register8bit::E:
		value = e_;
		break;
	case Register8bit::H:
		value = h_;
		break;
	case Register8bit::L:
		value = l_;
		break;
	default:
		throw std::logic_error("Read: invalid register");
		break;
	}
	return value;
}

void Registers::Write(Register8bit reg, uint8_t value)
{
	switch (reg)
	{
	case Register8bit::A:
		a_ = value;
		break;
	case Register8bit::B:
		b_ = value;
		break;
	case Register8bit::C:
		c_ = value;
		break;
	case Register8bit::D:
		d_ = value;
		break;
	case Register8bit::E:
		e_ = value;
		break;
	case Register8bit::H:
		h_ = value;
		break;
	case Register8bit::L:
		l_ = value;
		break;
	default:
		throw std::logic_error("Write: invalid register");
		break;
	}
}

uint16_t Registers::Read(Register16bit reg) const
{
	uint16_t value{ 0 };
	switch (reg)
	{
	case Register16bit::AF:
		value = 255 * a_ + f_;
		break;
	case Register16bit::BC:
		value = 255 * b_ + c_;
		break;
	case Register16bit::DE:
		value = 255 * d_ + e_;
		break;
	case Register16bit::HL:
		value = 255 * h_ + l_;
		break;
	case Register16bit::PC:
		value = pc_;
		break;
	case Register16bit::SP:
		value = sp_;
		break;
	default:
		throw std::logic_error("Read (16 bit): invalid register");
		break;
	}
	return value;
}

void Registers::Write(Register16bit reg, uint16_t value)
{
	switch (reg)
	{
	case Register16bit::AF:
		a_ = (value & 0xFF00) >> 8;
		f_ = value & 0x00FF;
		break;
	case Register16bit::BC:
		b_ = (value & 0xFF00) >> 8;
		c_ = value & 0x00FF;
		break;
	case Register16bit::DE:
		d_ = (value & 0xFF00) >> 8;
		e_ = value & 0x00FF;
		break;
	case Register16bit::HL:
		h_ = (value & 0xFF00) >> 8;
		l_ = value & 0x00FF;
		break;
	case Register16bit::PC:
		pc_ = value;
		break;
	case Register16bit::SP:
		sp_ = value;
		break;
	default:
		throw std::logic_error("Write (16 bit): invalid register");
		break;
	}
}

void Registers::Increment(Register16bit reg)
{
	Write(reg, Read(reg) + 1);
}

void Registers::Decrement(Register16bit reg)
{
	Write(reg, Read(reg) - 1);
}

void Registers::SetFlag(Flags flag, bool bSet)
{
	if (bSet)
	{
		f_ |= static_cast<std::underlying_type_t<Flags>>(flag);
	}
	else
	{
		f_ &= !(static_cast<std::underlying_type_t<Flags>>(flag));
	}
	
}

bool Registers::IsFlagSet(Flags flag) const
{
	return (f_ & static_cast<std::underlying_type_t<Flags>>(flag)) != 0;
}