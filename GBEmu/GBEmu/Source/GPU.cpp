#include "GPU.h"

GPU::GPU() :
	state_clock_ticks_{ { State::ReadingOAM, 80 }, { State::ReadingVRAM, 172 },
	{ State::HBlank, 204 }, { State::VBlank, 4560 } }
{

}

void GPU::Lapse(Clock clock)
{

}
