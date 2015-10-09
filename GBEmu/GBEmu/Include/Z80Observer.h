#pragma once

#include "Clock.h"

class Z80Observer
{
public:
	virtual void OnClockLapse(const Clock &op_duration) = 0;
};
