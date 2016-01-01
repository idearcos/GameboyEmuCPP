#pragma once

#include "IMbcController.h"

class Mbc1Controller final : public IMbcController
{
	enum class MaxMemoryMode
	{
		Rom16Mbit_Ram8kByte,
		Rom4Mbit_Ram32kByte
	};

public:
	Mbc1Controller() = default;
	~Mbc1Controller() = default;

	void OnMemoryWrite(const Memory::Address &address, uint8_t value) override;

private:
	MaxMemoryMode max_memory_mode_{ MaxMemoryMode::Rom16Mbit_Ram8kByte };
	uint8_t current_rom_bank_set{ 0 };
};
