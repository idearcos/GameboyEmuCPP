#pragma once

#include <cstdint>
#include "Memory.h"

class IMMUObserver
{
public:
	virtual void OnMemoryWrite(const Memory::Address &address, uint8_t value) = 0;
};
