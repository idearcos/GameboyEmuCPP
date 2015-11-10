#pragma once

#include <cstdint>

class Clock
{
public:
	Clock() = default;
	Clock(size_t m, size_t t);
	~Clock() = default;

	size_t GetTicks() const;
	size_t GetMachineCycles() const;

	Clock& operator+=(const Clock& rhs);

	friend bool operator==(const Clock& lhs, const Clock& rhs);

private:
	size_t m_{ 0 };
	size_t t_{ 0 };
};
