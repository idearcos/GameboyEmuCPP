#pragma once

#include <map>
#include "Clock.h"

class GPU
{
private:
	enum class State
	{
		ReadingOAM,
		ReadingVRAM,
		HBlank,
		VBlank
	};

public:
	GPU();
	~GPU() = default;

	void Lapse(Clock clock);

private:
	const std::map<State, uint8_t> state_clock_ticks_;
};
