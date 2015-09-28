#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <map>
#include "Observer.h"

class MMUObserver;

class MMU : public Subject<MMUObserver>
{
public:
	enum class Region
	{
		BIOS,
		ROM,
		VRAM,
		ERAM,
		WRAM,
		OAM,
		IO,
		ZeroPage
	};

	MMU();
	~MMU() = default;

	uint8_t Read8bitFromMemory(uint16_t absolute_address);
	uint16_t Read16bitFromMemory(uint16_t absolute_address);
	uint8_t Read8bitFromMemory(Region region, uint16_t local_address);
	void Write8bitToMemory(uint16_t absolute_address, uint8_t value);
	void Write16bitToMemory(uint16_t absolute_address, uint16_t value);

	void LoadRom(std::string rom_file_path);

private:
	std::tuple<Region, uint16_t> AbsoluteToLocalAddress(uint16_t absolute_address) const;
	uint16_t LocalToAbsoluteAddress(Region region, uint16_t local_address) const;

private:
	std::map<Region, std::vector<uint8_t>> memory_regions_;
	std::map<Region, uint16_t> region_start_addresses_;

	const std::vector<uint8_t> bios_;

	static const uint16_t start_rom_bank0_{ 0x0000 };
	static const uint16_t start_rom_other_banks_{ 0x4000 };
	static const uint16_t start_vram_{ 0x8000 };
	static const uint16_t start_eram_{ 0xA000 };
	static const uint16_t start_wram_{ 0xC000 };
	static const uint16_t start_wram_echo_{ 0xE000 };
	static const uint16_t start_oam_{ 0xFE00 };
	static const uint16_t start_io_{ 0xFF00 };
	static const uint16_t start_zero_page_{ 0xFF80 };

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
