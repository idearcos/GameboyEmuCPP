#include "MMU.h"

uint8_t MMU::Read8bitFromMemory(uint16_t /*addr*/) const
{
	uint8_t value{ 0 };
	// Do something
	return value;
}

uint16_t MMU::Read16bitFromMemory(uint16_t /*addr*/) const
{
	uint16_t value{ 0 };
	// First load low order, then high order byte
	return value;
}

void MMU::Write8bitToMemory(uint16_t /*addr*/, uint8_t /*value*/)
{
	// Do something
}

void MMU::Write16bitToMemory(uint16_t /*addr*/, uint16_t /*value*/)
{
	// First write high order, then low order byte
}
