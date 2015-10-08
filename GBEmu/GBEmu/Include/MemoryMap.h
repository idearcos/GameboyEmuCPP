#pragma once

#include <cstdint>

enum class Region : uint8_t
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
