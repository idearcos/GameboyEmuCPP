#include "Registers.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>

uint8_t Registers::Read(Register8bit reg) const
{
	switch (reg)
	{
	case Register8bit::A:
		return a_;
	case Register8bit::F:
		return f_;
	case Register8bit::B:
		return b_;
	case Register8bit::C:
		return c_;
	case Register8bit::D:
		return d_;
	case Register8bit::E:
		return e_;
	case Register8bit::H:
		return h_;
	case Register8bit::L:
		return l_;
	default:
		std::stringstream msg;
		msg << "Trying to read invalid register: " << reg;
		throw std::logic_error(msg.str());
	}
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
	switch (reg)
	{
	case Register16bit::AF:
		return af_;
	case Register16bit::BC:
		return bc_;
	case Register16bit::DE:
		return de_;
	case Register16bit::HL:
		return hl_;
	case Register16bit::PC:
		return pc_;
	case Register16bit::SP:
		return sp_;
	default:
		std::stringstream msg;
		msg << "Trying to read invalid register: " << reg;
		throw std::logic_error(msg.str());
	}
}

void Registers::Write(Register16bit reg, uint16_t value)
{
	switch (reg)
	{
	case Register16bit::AF:
		af_ = value;
		break;
	case Register16bit::BC:
		bc_ = value;
		break;
	case Register16bit::DE:
		de_ = value;
		break;
	case Register16bit::HL:
		hl_ = value;
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
