#include "RomInfo.h"
#include <sstream>

RomInfo::RomInfo(const std::vector<uint8_t> &rom) :
	title_(reinterpret_cast<const char*>(&rom[0x0134]), 16),
	manufacturer_(reinterpret_cast<const char*>(&rom[0x013F]), 4)
{
	if (rom.size() < 0x0150)
	{
		throw std::invalid_argument("Cannot build ROM info because ROM is too small");
	}

	for (size_t i = 0; i < entry_point_.size(); i++)
	{
		entry_point_[i] = rom[0x0100 + i];
	}

	is_color_gb_ = static_cast<ColorGB>(rom[0x0143]);

	new_licensee_code_ = (rom[0x0144] << 8) + rom[0x0145];

	super_gb_functions_ = rom[0x0146] == 0x03;

	cartridge_type_ = static_cast<CartridgeType>(rom[0x0147]);

	switch (rom[0x0148])
	{
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
		num_rom_banks_ = 2 << rom[0x0148];
		break;
	case 0x52:
		num_rom_banks_ = 72;
		break;
	case 0x53:
		num_rom_banks_ = 80;
		break;
	case 0x54:
		num_rom_banks_ = 96;
		break;
	default:
		std::stringstream msg;
		msg << "Unknown ROM size identifier: 0x" << std::hex << rom[0x0148];
		throw std::runtime_error(msg.str());
	}

	switch (rom[0x0149])
	{
	case 0x00:
		external_ram_size_ = 0;
		break;
	case 0x01:
		external_ram_size_ = 2 * 1024;
		break;
	case 0x02:
		external_ram_size_ = 8 * 1024;
		break;
	case 0x03:
		external_ram_size_ = 32 * 1024;
		break;
	case 0x04:
		external_ram_size_ = 128 * 1024;
		break;
	case 0x05:
		external_ram_size_ = 64 * 1024;
		break;
	default:
		std::stringstream msg;
		msg << "Unknown RAM size identifier: 0x" << std::hex << rom[0x0148];
		throw std::runtime_error(msg.str());
	}

	destination_japanese_ = rom[0x014A] == 0x00;

	old_licensee_code_ = rom[0x014B];

	version_number_ = rom[0x014C];

	header_checksum_ = rom[0x014D];

	global_checksum_ = (rom[0x014E] << 8) + rom[0x014F];
}
