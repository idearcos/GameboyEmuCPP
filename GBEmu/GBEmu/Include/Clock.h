#pragma once

#include <cstdint>

class Clock
{
public:
	Clock() = default;
	Clock(uint8_t m, uint8_t t) : m_(m), t_(t) {}
	~Clock() = default;

	uint8_t GetTicks() const
	{
		return t_;
	}

	Clock& operator+=(const Clock& rhs)
	{
		this->m_ += rhs.m_;
		this->t_ += rhs.t_;
		return *this;
	}

private:
	uint8_t m_{ 0 }; // Clock for last instruction
	uint8_t t_{ 0 }; // Clock for last instruction
};