#include "GPU.h"

GPU::GPU() :
	states_(InitStateMap())
{

}

std::map<GPU::Mode, std::unique_ptr<GPU::State>> GPU::InitStateMap()
{
	std::map<Mode, std::unique_ptr<State>> states;
	states.emplace(Mode::ReadingOAM, std::make_unique<ReadingOAM>());
	states.emplace(Mode::ReadingVRAM, std::make_unique<ReadingVRAM>());
	states.emplace(Mode::HBlank, std::make_unique<HBlank>());
	states.emplace(Mode::VBlank, std::make_unique<VBlank>());
	return states;
}

void GPU::Lapse(Clock clock)
{
	ticks_current_period_ += clock.GetTicks();

	std::tie(current_mode_, ticks_current_period_) = states_.at(current_mode_)->Lapse(ticks_current_period_);
}

std::tuple<GPU::Mode, uint8_t> GPU::State::Lapse(uint8_t ticks_current_period)
{

}

std::tuple<GPU::Mode, uint8_t> GPU::ReadingOAM::Transition(uint8_t ticks_current_period)
{


	return Mode::ReadingVRAM;
}

std::tuple<GPU::Mode, uint8_t> GPU::ReadingVRAM::Transition(uint8_t ticks_current_period)
{


	return Mode::HBlank;
}

std::tuple<GPU::Mode, uint8_t> GPU::HBlank::Transition(uint8_t ticks_current_period)
{


	return Mode::VBlank;
}

std::tuple<GPU::Mode, uint8_t> GPU::VBlank::Transition(uint8_t ticks_current_period)
{
	if (ticks_current_period >= 4560)
	{
		return std::make_tuple(Mode::ReadingOAM, ticks_current_period - 4560);
	}
	else
	{
		return std::make_tuple(Mode::VBlank, ticks_current_period);
	}
}
