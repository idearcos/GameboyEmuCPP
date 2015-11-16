#pragma once

#include "Z80Observer.h"
#include "Memory.h"
#include "IMMU.h"
#include "MMUObserver.h"

class Timer final : public Z80Observer, public MMUObserver
{
public:
	Timer(IMMU &mmu);
	~Timer() = default;

	void OnClockLapse(const Clock &op_duration) override;
	void OnMemoryWrite(const Memory::Address &address, uint8_t value) override;

private:
	void WriteToMmu(const Memory::Address &address, uint8_t value) const;

private:
	const Memory::Address divider_register_{ 0xFF04 };
	const Memory::Address counter_register_{ 0xFF05 };
	const Memory::Address modulo_register_{ 0xFF06 };
	const Memory::Address timer_control_register_{ 0xFF07 };
	const Memory::Address interrupt_flags_register_{ Memory::Region::IO, 0x000F };

	uint8_t divider_{ 0 };
	size_t divider_machine_cycles_{ 0 };
	uint8_t counter_{ 0 };
	size_t counter_machine_cycles_{ 0 };
	uint8_t modulo_{ 0 };

	const size_t machine_cycles_per_divider_update_{ 4 * 16 };
	size_t machine_cycles_per_counter_update_{ 4 };

	bool running_{ false };

	IMMU &mmu_;
	mutable bool writing_to_mmu_{ false };

private:
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;
};
