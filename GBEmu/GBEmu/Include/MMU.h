#pragma once

#include <cstdint>
#include <array>

class MMU
{
public:
	MMU();
	~MMU() = default;

	uint8_t Read8bitFromMemory(uint16_t addr) const;
	uint16_t Read16bitFromMemory(uint16_t addr) const;
	void Write8bitToMemory(uint16_t addr, uint8_t value);
	void Write16bitToMemory(uint16_t addr, uint16_t value);

private:
	const std::array<uint8_t, 256> bios_;
	bool bios_loaded_{ true };

private:
	MMU(const MMU&) = delete;
	MMU(MMU&&) = delete;
	MMU& operator=(const MMU&) = delete;
	MMU& operator=(MMU&&) = delete;
};
