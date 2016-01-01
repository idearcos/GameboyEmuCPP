#include "GPU_States.h"
#include "GPU.h"

std::tuple<Mode, size_t> State::Lapse(size_t ticks_current_period, GPU &gpu)
{
	if (ticks_current_period >= ticks_per_period_)
	{
		return std::make_tuple(Transition(gpu), ticks_current_period - ticks_per_period_);
	}
	else
	{
		return std::make_tuple(CurrentMode(), ticks_current_period);
	}
}

Mode ReadingOAM::Transition(GPU &) const
{
	return Mode::ReadingVRAM;
}

Mode ReadingVRAM::Transition(GPU &gpu) const
{
	gpu.RenderScanLine();
	return Mode::HBlank;
}

Mode HBlank::Transition(GPU &gpu) const
{
	if (gpu.IncrementCurrentLine() >= 144)
	{
		return Mode::VBlank;
	}
	else
	{
		return Mode::ReadingOAM;
	}
}

Mode VBlank::Transition(GPU &gpu) const
{
	if (gpu.IncrementCurrentLine() == 0)
	{
		return Mode::ReadingOAM;
	}
	else
	{
		return Mode::VBlank;
	}
}
