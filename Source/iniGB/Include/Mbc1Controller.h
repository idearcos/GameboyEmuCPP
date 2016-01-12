#pragma once

#include "IMbcController.h"

class Mbc1Controller final : public IMbcController
{
	enum class BankingMode
	{
		Rom, // max 2 MByte ROM, max 8 kByte RAM
		Ram // max 512 kByte ROM, max 32 kByte RAM
	};

public:
	Mbc1Controller() = default;
	~Mbc1Controller() = default;

	void OnMemoryWrite(const Memory::Address &address, uint8_t value) override;

private:
	BankingMode banking_mode_{ BankingMode::Rom };
	uint8_t rom_bank_lower_5_bits_{ 0 };
	uint8_t current_rom_bank_set_{ 0 };
};
