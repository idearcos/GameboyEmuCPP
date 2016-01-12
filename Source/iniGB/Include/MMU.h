#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include "IMMU.h"
#include "Subject.h"
#include "IMMUObserver.h"
#include "Memory.h"
#include "RomInfo.h"
#include "IMbcController.h"
#include "IMbcControllerObserver.h"

class IMMUObserver;

class MMU final : public IMMU, public Subject<IMMUObserver>, public IMbcControllerObserver
{
public:
	MMU();
	~MMU() = default;

	uint8_t Read8bitFromMemory(const Memory::Address &address) const override;
	uint16_t Read16bitFromMemory(const Memory::Address &address) const override;
	void Write8bitToMemory(const Memory::Address &address, uint8_t value) override;
	void Write16bitToMemory(const Memory::Address &address, uint16_t value) override;

	void LoadRom(std::string rom_file_path);

	void OnRomBankSwitchRequested(uint8_t requested_rom_bank_number) override;
	void OnRamBankSwitchRequested(uint8_t requested_ram_bank_number) override;
	void OnExternalRamEnabled(bool enabled) override;

private:
	std::map<Memory::Region, std::vector<uint8_t>> memory_regions_;

	const std::vector<uint8_t> bios_;
	mutable bool bios_loaded_{ true };

	std::vector<std::vector<uint8_t>> rom_banks_;
	std::vector<std::vector<uint8_t>> ram_banks_;
	size_t currently_loaded_rom_bank_{ 1 };
	size_t currently_loaded_ram_bank_{ 0 };
	bool external_ram_enabled_{ false };
	std::unique_ptr<IMbcController> mbc_controller_;

private:
	MMU(const MMU&) = delete;
	MMU(MMU&&) = delete;
	MMU& operator=(const MMU&) = delete;
	MMU& operator=(MMU&&) = delete;
};
