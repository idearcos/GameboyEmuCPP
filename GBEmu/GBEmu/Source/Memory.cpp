#include "Memory.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace Memory
{
	std::tuple<Region, uint16_t> AbsoluteToRelativeAddress(uint16_t absolute_address);
	uint16_t RelativeToAbsoluteAddress(Region region, uint16_t relative_address);

	Address::Address(uint16_t absolute_address) :
		absolute_address_{ absolute_address }
	{
		std::tie(region_, relative_address_) = AbsoluteToRelativeAddress(absolute_address);
	}

	Address::Address(Region region, uint16_t relative_address) :
		region_{ region },
		relative_address_{ relative_address },
		absolute_address_{ RelativeToAbsoluteAddress(region, relative_address) }
	{

	}

	bool operator== (const Address &lhs, const Address &rhs)
	{
		return (lhs.absolute_address_ == rhs.absolute_address_);
	}

	Address operator+ (const Address &address, size_t offset)
	{
		return Address{ static_cast<uint16_t>(address.absolute_address_ + offset) };
	}

	size_t SizeOfRegion(Region region)
	{
		switch (region)
		{
		//case Region::ROM_BANK0:
		//case Region::ROM_OTHER_BANKS:
		//	return 0x4000;
		case Region::ROM:
			return 0x8000;
		case Region::VRAM:
		case Region::ERAM:
		case Region::WRAM:
			return 0x2000;
		case Region::WRAM_ECHO:
			return 0x1E00;
		case Region::OAM:
			return 0x00A0;
		case Region::Invalid:
			return 0x0060;
		case Region::IO:
		case Region::ZRAM:
			return 0x0080;
		default:
			std::stringstream msg;
			msg << "SizeOf: invalid memory region 0x" << std::hex << std::setfill('0') << std::setw(4) << static_cast<size_t>(region);
			throw std::invalid_argument(msg.str());
		}
	}

	std::tuple<Region, uint16_t> AbsoluteToRelativeAddress(uint16_t absolute_address)
	{
		/*if ((absolute_address >= static_cast<uint16_t>(Region::ROM_BANK0)) && (absolute_address < static_cast<uint16_t>(Region::ROM_OTHER_BANKS)))
		{
			return std::make_tuple(Region::ROM_BANK0, absolute_address - static_cast<uint16_t>(Region::ROM_BANK0));
		}
		else if ((absolute_address >= static_cast<uint16_t>(Region::ROM_OTHER_BANKS)) && (absolute_address < static_cast<uint16_t>(Region::VRAM)))
		{
			return std::make_tuple(Region::ROM_OTHER_BANKS, absolute_address - static_cast<uint16_t>(Region::ROM_OTHER_BANKS));
		}*/
		if ((absolute_address >= static_cast<uint16_t>(Region::ROM)) && (absolute_address < static_cast<uint16_t>(Region::VRAM)))
		{
			return std::make_tuple(Region::ROM, absolute_address - static_cast<uint16_t>(Region::ROM));
		}
		else if ((absolute_address >= static_cast<uint16_t>(Region::VRAM)) && (absolute_address < static_cast<uint16_t>(Region::ERAM)))
		{
			return std::make_tuple(Region::VRAM, absolute_address - static_cast<uint16_t>(Region::VRAM));
		}
		else if ((absolute_address >= static_cast<uint16_t>(Region::ERAM)) && (absolute_address < static_cast<uint16_t>(Region::WRAM)))
		{
			return std::make_tuple(Region::ERAM, absolute_address - static_cast<uint16_t>(Region::ERAM));
		}
		else if ((absolute_address >= static_cast<uint16_t>(Region::WRAM)) && (absolute_address < static_cast<uint16_t>(Region::WRAM_ECHO)))
		{
			return std::make_tuple(Region::WRAM, absolute_address - static_cast<uint16_t>(Region::WRAM));
		}
		else if ((absolute_address >= static_cast<uint16_t>(Region::WRAM_ECHO)) && (absolute_address < static_cast<uint16_t>(Region::OAM)))
		{
			return std::make_tuple(Region::WRAM_ECHO, absolute_address - static_cast<uint16_t>(Region::WRAM_ECHO));
		}
		else if ((absolute_address >= static_cast<uint16_t>(Region::OAM)) && (absolute_address < static_cast<uint16_t>(Region::Invalid)))
		{
			return std::make_tuple(Region::OAM, absolute_address - static_cast<uint16_t>(Region::OAM));
		}
		else if ((absolute_address >= static_cast<uint16_t>(Region::Invalid)) && (absolute_address < static_cast<uint16_t>(Region::IO)))
		{
			return std::make_tuple(Region::Invalid, absolute_address - static_cast<uint16_t>(Region::Invalid));
		}
		else if ((absolute_address >= static_cast<uint16_t>(Region::IO)) && (absolute_address < static_cast<uint16_t>(Region::ZRAM)))
		{
			return std::make_tuple(Region::IO, absolute_address - static_cast<uint16_t>(Region::IO));
		}
		else if (absolute_address >= static_cast<uint16_t>(Region::ZRAM))
		{
			return std::make_tuple(Region::ZRAM, absolute_address - static_cast<uint16_t>(Region::ZRAM));
		}
		else
		{
			std::stringstream msg;
			msg << "Could not convert absolute address 0x" << std::hex << std::setfill('0') << std::setw(4) << absolute_address;
			throw std::runtime_error(msg.str());
		}
	}

	uint16_t RelativeToAbsoluteAddress(Region region, uint16_t relative_address)
	{
		return relative_address + static_cast<uint16_t>(region);
	}
}
