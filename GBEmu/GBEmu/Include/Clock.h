#pragma once

#include <cstdint>

class Clock
{
public:
	Clock() = default;
	Clock(size_t m, size_t t) : m_(m), t_(t) {}
	~Clock() = default;

	size_t GetTicks() const
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
	size_t m_{ 0 };
	size_t t_{ 0 };
};