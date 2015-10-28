#pragma once

#include <cstdint>
#include "Memory.h"

class IMMU
{
public:
	IMMU() = default;
	virtual ~IMMU() {}

	virtual uint8_t Read8bitFromMemory(const Memory::Address &address) const = 0;
	virtual uint16_t Read16bitFromMemory(const Memory::Address &address) const = 0;
	virtual void Write8bitToMemory(const Memory::Address &address, uint8_t value) = 0;
	virtual void Write16bitToMemory(const Memory::Address &address, uint16_t value) = 0;
};
