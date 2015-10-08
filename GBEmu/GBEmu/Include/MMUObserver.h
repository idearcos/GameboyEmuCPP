#pragma once

#include <cstdint>
#include "MemoryMap.h"

class MMUObserver
{
public:
	virtual void OnMemoryWrite(Region region, uint16_t address, uint8_t value) = 0;
};
