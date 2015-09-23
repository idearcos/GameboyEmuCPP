#pragma once

#include <cstdint>
#include <array>
#include "Observer.h"

class MMUObserver;

class MMU : public Subject<MMUObserver>
{
public:
	enum class Region
	{
		Vram
	};

	MMU();
	~MMU() = default;

	uint8_t Read8bitFromMemory(uint16_t addr) const;
	uint16_t Read16bitFromMemory(uint16_t addr) const;
	void Write8bitToMemory(uint16_t addr, uint8_t value);
	void Write16bitToMemory(uint16_t addr, uint16_t value);

	inline bool IsInVram(uint16_t addr) const;

private:
	static const uint16_t start_vram_{ 0x8000 };
	static const uint16_t start_eram_{ 0xA000 };
	const std::array<uint8_t, 256> bios_;
	bool bios_loaded_{ true };

private:
	MMU(const MMU&) = delete;
	MMU(MMU&&) = delete;
	MMU& operator=(const MMU&) = delete;
	MMU& operator=(MMU&&) = delete;
};

class MMUObserver
{
public:
	virtual void OnMemoryWrite(MMU::Region region, uint16_t address, uint8_t value) = 0;
};
