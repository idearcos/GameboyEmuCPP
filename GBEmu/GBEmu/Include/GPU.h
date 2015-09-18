#pragma once

#include <map>
#include <memory>
#include "Clock.h"

class GPU
{
private:
	enum class Mode
	{
		ReadingOAM,
		ReadingVRAM,
		HBlank,
		VBlank
	};

	class State
	{
	public:
		State(uint8_t ticks_per_period) : ticks_per_period_(ticks_per_period) {}
		// Returns the new mode, and the remainder of ticks that have to be accounted for the next period
		std::tuple<Mode, uint8_t> Lapse(uint8_t ticks_current_period);
	protected:
		virtual std::tuple<Mode, uint8_t> Transition() = 0;
		const uint8_t ticks_per_period_;
	};

	class ReadingOAM final : public State
	{
		std::tuple<Mode, uint8_t> Transition() override;
	};

	class ReadingVRAM final : public State
	{
		std::tuple<Mode, uint8_t> Transition() override;
	};

	class HBlank final : public State
	{
		std::tuple<Mode, uint8_t> Transition() override;
	};

	class VBlank final : public State
	{
		std::tuple<Mode, uint8_t> Transition() override;
	};

public:
	GPU();
	~GPU() = default;

	void Lapse(Clock clock);

private:
	static std::map<Mode, std::unique_ptr<State>> InitStateMap();

private:
	Mode current_mode_{ Mode::ReadingOAM };
	const std::map<Mode, std::unique_ptr<State>> states_;
	uint8_t ticks_current_period_{ 0 };
	uint8_t num_lines_scanned{ 0 };

private:
	GPU(const GPU&) = delete;
	GPU(GPU&&) = delete;
	GPU& operator=(const GPU&) = delete;
	GPU& operator=(GPU&&) = delete;
};
