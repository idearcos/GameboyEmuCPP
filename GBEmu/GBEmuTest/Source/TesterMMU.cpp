#include "TesterMMU.h"

TesterMMU::TesterMMU()
{
	memory_.fill(0);
}

uint8_t TesterMMU::Read8bitFromMemory(uint16_t absolute_address) const
{
	return memory_[absolute_address];
}

uint16_t TesterMMU::Read16bitFromMemory(uint16_t absolute_address) const
{
	uint16_t value = Read8bitFromMemory(absolute_address);
	value += (Read8bitFromMemory(absolute_address + 1) << 8);
	return value;
}

uint8_t TesterMMU::Read8bitFromMemory(Region region, uint16_t local_address) const
{
	throw std::logic_error("Overload of Read8bitFromMemory is out of testing scope");
}

void TesterMMU::Write8bitToMemory(uint16_t absolute_address, uint8_t value)
{
	memory_[absolute_address] = value;
}

void TesterMMU::Write16bitToMemory(uint16_t absolute_address, uint16_t value)
{
	Write8bitToMemory(absolute_address, value & 0xFF);
	Write8bitToMemory(absolute_address + 1, (value >> 8) & 0xFF);
}

void TesterMMU::Write8bitToMemory(Region region, uint16_t local_address, uint8_t value)
{
	throw std::logic_error("Overload of Write8bitToMemory is out of testing scope");
}
