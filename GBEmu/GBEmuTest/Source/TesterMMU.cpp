#include "TesterMMU.h"

TesterMMU::TesterMMU()
{
	memory_.fill(0);
}

uint8_t TesterMMU::Read8bitFromMemory(const Memory::Address &address) const
{
	return memory_[address.GetAbsoluteAddress()];
}

uint16_t TesterMMU::Read16bitFromMemory(const Memory::Address &address) const
{
	uint16_t value = Read8bitFromMemory(address);
	value += (Read8bitFromMemory(address + 1) << 8);
	return value;
}

void TesterMMU::Write8bitToMemory(const Memory::Address &address, uint8_t value)
{
	memory_[address.GetAbsoluteAddress()] = value;
}

void TesterMMU::Write16bitToMemory(const Memory::Address &address, uint16_t value)
{
	Write8bitToMemory(address, value & 0xFF);
	Write8bitToMemory(address + 1, (value >> 8) & 0xFF);
}
