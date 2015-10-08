#pragma once

#include <cstdint>
#include "MemoryMap.h"

class IMMU
{
public:
	IMMU() = default;
	virtual ~IMMU() {}

	virtual uint8_t Read8bitFromMemory(uint16_t absolute_address) const = 0;
	virtual uint16_t Read16bitFromMemory(uint16_t absolute_address) const = 0;
	virtual uint8_t Read8bitFromMemory(Region region, uint16_t local_address) const = 0;
	virtual void Write8bitToMemory(uint16_t absolute_address, uint8_t value) = 0;
	virtual void Write16bitToMemory(uint16_t absolute_address, uint16_t value) = 0;
	virtual void Write8bitToMemory(Region region, uint16_t local_address, uint8_t value) = 0;
};
