#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <map>
#include "IMMU.h"
#include "Subject.h"
#include "MMUObserver.h"
#include "Memory.h"
#include "RomInfo.h"

class MMUObserver;

class MMU final : public IMMU, public Subject<MMUObserver>
{
public:
	MMU();
	~MMU() = default;

	uint8_t Read8bitFromMemory(const Memory::Address &address) const override;
	uint16_t Read16bitFromMemory(const Memory::Address &address) const override;
	void Write8bitToMemory(const Memory::Address &address, uint8_t value) override;
	void Write16bitToMemory(const Memory::Address &address, uint16_t value) override;

	void LoadRom(std::string rom_file_path);

private:
	std::map<Memory::Region, std::vector<uint8_t>> memory_regions_;

	const std::vector<uint8_t> bios_;
	mutable bool bios_loaded_{ false };

	size_t currently_loaded_rom_bank_{ 1 };
	size_t current_rom_bank_set{ 0 };

private:
	MMU(const MMU&) = delete;
	MMU(MMU&&) = delete;
	MMU& operator=(const MMU&) = delete;
	MMU& operator=(MMU&&) = delete;
};
