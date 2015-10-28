#pragma once

#include <cstdint>
#include <tuple>

namespace Memory
{
	enum class Region : uint16_t
	{
		ROM = 0x0000,
		//ROM_BANK0 = 0x0000,
		//ROM_OTHER_BANKS = 0x4000,
		VRAM = 0x8000,
		ERAM = 0xA000,
		WRAM = 0xC000,
		WRAM_ECHO = 0xE000,
		OAM = 0xFE00,
		Invalid = 0xFEA0,
		IO = 0xFF00,
		ZRAM = 0xFF80
	};

	size_t SizeOfRegion(Region region);

	class Address
	{
	public:
		Address(uint16_t absolute_address);
		Address(Region region, uint16_t relative_address);

		uint16_t GetAbsoluteAddress() const { return absolute_address_; }
		std::tuple<Region, uint16_t> GetRelativeAddress() const { return std::make_tuple(region_, relative_address_); }

		friend bool operator== (const Address &lhs, const Address &rhs);
		friend Address operator+ (const Address &address, size_t offset);

	private:
		uint16_t absolute_address_{ 0 };
		Region region_{ Region::ROM };
		uint16_t relative_address_{ 0 };
	};
}
