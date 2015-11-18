#pragma once

#include <cstdint>

class IMbcControllerObserver
{
public:
	virtual void OnRomBankSwitchRequested(uint8_t requested_rom_bank_number) = 0;
};
