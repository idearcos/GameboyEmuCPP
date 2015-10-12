#include "Clock.h"

Clock::Clock(size_t m, size_t t) :
	m_(m),
	t_(t)
{

}

size_t Clock::GetTicks() const
{
	return t_;
}

Clock& Clock::operator += (const Clock& rhs)
{
	this->m_ += rhs.m_;
	this->t_ += rhs.t_;
	return *this;
}

bool operator==(const Clock& lhs, const Clock& rhs)
{
	return ((lhs.m_ == rhs.m_) && (lhs.t_ == rhs.t_));
}
