#pragma once

#include <cstdint>
#include <tuple>

enum class Mode : uint8_t
{
	ReadingOAM = 2,
	ReadingVRAM = 3,
	HBlank = 0,
	VBlank = 1
};

class GPU;

class State
{
public:
	State(size_t ticks_per_period) : ticks_per_period_(ticks_per_period) {}
	// Returns the new mode, and the remainder of ticks that have to be accounted for the next period
	std::tuple<Mode, size_t> Lapse(size_t ticks_current_period, GPU &gpu);

private:
	virtual Mode CurrentMode() const = 0;
	virtual Mode Transition(GPU &gpu) const = 0;
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
	Mode CurrentMode() const override { return Mode::ReadingOAM; }
	Mode Transition(GPU&) const override;
};

class ReadingVRAM final : public State
{
public:
	ReadingVRAM() : State(172) {}
private:
	Mode CurrentMode() const override { return Mode::ReadingVRAM; }
	Mode Transition(GPU&) const override;
};

class HBlank final : public State
{
public:
	HBlank() : State(204) {}
private:
	Mode CurrentMode() const override { return Mode::HBlank; }
	Mode Transition(GPU &gpu) const override;
};

class VBlank final : public State
{
public:
	VBlank() : State(456) {}
private:
	Mode CurrentMode() const override { return Mode::VBlank; }
	Mode Transition(GPU &gpu) const override;
};
