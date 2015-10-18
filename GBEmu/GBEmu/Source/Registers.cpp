#include "Registers.h"

#include <iostream>
#include <sstream>
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
	case Register8bit::F:
		value = f_;
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
		std::stringstream msg;
		msg << "Trying to read invalid register: " << reg;
		throw std::logic_error(msg.str());
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
		std::stringstream msg;
		msg << "Trying to write invalid register: " << reg;
		throw std::logic_error(msg.str());
	}
}

uint16_t Registers::Read(Register16bit reg) const
{
	uint16_t value{ 0 };
	switch (reg)
	{
	case Register16bit::AF:
		value = (a_ << 8) + f_;
		break;
	case Register16bit::BC:
		value = (b_ << 8) + c_;
		break;
	case Register16bit::DE:
		value = (d_ << 8) + e_;
		break;
	case Register16bit::HL:
		value = (h_ << 8) + l_;
		break;
	case Register16bit::PC:
		value = pc_;
		break;
	case Register16bit::SP:
		value = sp_;
		break;
	default:
		std::stringstream msg;
		msg << "Trying to read invalid register: " << reg;
		throw std::logic_error(msg.str());
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
		std::stringstream msg;
		msg << "Trying to write invalid register: " << reg;
		throw std::logic_error(msg.str());
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
		f_ &= ~(static_cast<std::underlying_type_t<Flags>>(flag));
	}
	
}

bool Registers::IsFlagSet(Flags flag) const
{
	return (f_ & static_cast<std::underlying_type_t<Flags>>(flag)) != 0;
}

std::ostream& operator << (std::ostream& os, const Flags& flag)
{
	switch (flag)
	{
	case Flags::Zero:
		os << "Zero";
		break;
	case Flags::Subtract:
		os << "Subtract";
		break;
	case Flags::HalfCarry:
		os << "HalfCarry";
		break;
	case Flags::Carry:
		os << "Carry";
		break;
	default:
		os << static_cast<size_t>(flag);
		break;
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const Register8bit& reg)
{
	switch (reg)
	{
	case Register8bit::A:
		os << "A";
		break;
	case Register8bit::F:
		os << "F";
		break;
	case Register8bit::B:
		os << "B";
		break;
	case Register8bit::C:
		os << "C";
		break;
	case Register8bit::D:
		os << "D";
		break;
	case Register8bit::E:
		os << "E";
		break;
	case Register8bit::H:
		os << "H";
		break;
	case Register8bit::L:
		os << "L";
		break;
	default:
		os << static_cast<size_t>(reg);
		break;
	}

	return os;
}

std::ostream& operator << (std::ostream& os, const Register16bit& reg)
{
	switch (reg)
	{
	case Register16bit::AF:
		os << "AF";
		break;
	case Register16bit::BC:
		os << "BC";
		break;
	case Register16bit::DE:
		os << "DE";
		break;
	case Register16bit::HL:
		os << "HL";
		break;
	case Register16bit::PC:
		os << "PC";
		break;
	case Register16bit::SP:
		os << "SP";
		break;
	default:
		os << static_cast<size_t>(reg);
		break;
	}

	return os;
}
