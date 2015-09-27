#pragma once

#include <cstdint>
#include <tuple>

enum class Mode
{
	ReadingOAM,
	ReadingVRAM,
	HBlank,
	VBlank
};

class GPU;

class State
{
public:
	State(size_t ticks_per_period) : ticks_per_period_(ticks_per_period) {}
	// Returns the new mode, and the remainder of ticks that have to be accounted for the next period
	std::tuple<Mode, size_t> Lapse(size_t ticks_current_period, GPU &gpu);

private:
	virtual Mode Transition(GPU &gpu) = 0;
	const size_t ticks_per_period_;

private:
	State(const State&) = delete;
	State(State&&) = delete;
	State& operator=(const State&) = delete;
	State& operator=(State&&) = delete;
};

class ReadingOAM final : public State
{
public:
	ReadingOAM() : State(80) {}
private:
	Mode Transition(GPU&) override;
};

class ReadingVRAM final : public State
{
public:
	ReadingVRAM() : State(172) {}
private:
	Mode Transition(GPU&) override;
};

class HBlank final : public State
{
public:
	HBlank() : State(204) {}
private:
	Mode Transition(GPU &gpu) override;
};

class VBlank final : public State
{
public:
	VBlank() : State(456) {}
private:
	Mode Transition(GPU &gpu) override;
};
