#pragma once

#include <array>
#include "IMMU.h"

class TesterMMU final : public IMMU
{
public:
	TesterMMU();
	virtual ~TesterMMU() {}

	uint8_t Read8bitFromMemory(uint16_t absolute_address) const override;
	uint16_t Read16bitFromMemory(uint16_t absolute_address) const override;
	uint8_t Read8bitFromMemory(Region region, uint16_t local_address) const override;
	void Write8bitToMemory(uint16_t absolute_address, uint8_t value) override;
	void Write16bitToMemory(uint16_t absolute_address, uint16_t value) override;
	void Write8bitToMemory(Region region, uint16_t local_address, uint8_t value) override;

private:
	std::array<uint8_t, UINT16_MAX> memory_;
};
