#pragma once

#include <cstdint>

class IMbcControllerObserver
{
public:
	virtual void OnRomBankSwitchRequested(uint8_t requested_rom_bank_number) = 0;
	virtual void OnRamBankSwitchRequested(uint8_t requested_ram_bank_number) = 0;
	virtual void OnExternalRamEnabled(bool enabled) = 0;
};
