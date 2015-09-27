#pragma once

#include <cstdint>
#include <array>
#include <map>
#include "Observer.h"

class MMUObserver;

class MMU : public Subject<MMUObserver>
{
public:
	enum class Region
	{
		VRAM,
		IO
	};

	MMU();
	~MMU() = default;

	uint8_t Read8bitFromMemory(uint16_t absolute_address) const;
	uint16_t Read16bitFromMemory(uint16_t absolute_address) const;
	uint8_t Read8bitFromMemory(Region region, uint16_t local_address) const;
	void Write8bitToMemory(uint16_t absolute_address, uint8_t value);
	void Write16bitToMemory(uint16_t absolute_address, uint16_t value);

	std::tuple<Region, uint16_t> AbsoluteToLocalAddress(uint16_t absolute_address) const;
	uint16_t LocalToAbsoluteAddress(Region region, uint16_t local_address) const;

private:
	static const uint16_t start_vram_{ 0x8000 };
	static const uint16_t start_eram_{ 0xA000 };
	static const uint16_t start_io_{ 0xFF00 };
	static const uint16_t start_zero_page_{ 0xFF80 };
	std::map<Region, uint16_t> region_start_addresses_;

	const std::array<uint8_t, 256> bios_;
	std::array<uint8_t, 8192> vram_;
	std::array<uint8_t, 128> io_;

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
