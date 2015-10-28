#pragma once

#include <array>
#include "IMMU.h"

class TesterMMU final : public IMMU
{
public:
	TesterMMU();
	virtual ~TesterMMU() {}

	uint8_t Read8bitFromMemory(const Memory::Address &address) const override;
	uint16_t Read16bitFromMemory(const Memory::Address &address) const override;
	void Write8bitToMemory(const Memory::Address &address, uint8_t value) override;
	void Write16bitToMemory(const Memory::Address &address, uint16_t value) override;

private:
	std::array<uint8_t, UINT16_MAX> memory_;
};
