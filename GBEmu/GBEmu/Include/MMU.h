#pragma once

#include <cstdint>

class MMU
{
public:
	uint8_t Read8bitFromMemory(uint16_t addr) const;
	uint16_t Read16bitFromMemory(uint16_t addr) const;
	void Write8bitToMemory(uint16_t addr, uint8_t value);
	void Write16bitToMemory(uint16_t addr, uint16_t value);
};
