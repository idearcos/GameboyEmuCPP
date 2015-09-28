#include "MMU.h"
#include <fstream>

MMU::MMU()
{
	region_start_addresses_[Region::BIOS] = start_rom_bank0_;
	region_start_addresses_[Region::ROM] = start_rom_bank0_;
	region_start_addresses_[Region::VRAM] = start_vram_;
	region_start_addresses_[Region::ERAM] = start_eram_;
	region_start_addresses_[Region::WRAM] = start_wram_;
	region_start_addresses_[Region::OAM] = start_oam_;
	region_start_addresses_[Region::IO] = start_io_;
	region_start_addresses_[Region::ZeroPage] = start_zero_page_;

	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Region::ROM), std::forward_as_tuple(0x10000, 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Region::VRAM), std::forward_as_tuple(0x2000, 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Region::ERAM), std::forward_as_tuple(0x2000, 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Region::WRAM), std::forward_as_tuple(0x3E00, 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Region::OAM), std::forward_as_tuple(0x0100, 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Region::IO), std::forward_as_tuple(0x0080, 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Region::ZeroPage), std::forward_as_tuple(0x0080, 0));

	memory_regions_.emplace(Region::BIOS, std::initializer_list<uint8_t>{
		0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
		0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
		0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
		0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
		0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
		0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
		0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
		0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
		0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
		0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
		0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
		0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
		0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
		0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
		0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
		0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50 });
}

uint8_t MMU::Read8bitFromMemory(uint16_t absolute_address)
{
	Region region{ Region::VRAM };
	uint16_t local_address(absolute_address);
	std::tie(region, local_address) = AbsoluteToLocalAddress(absolute_address);

	try
	{
		if (Region::BIOS == region)
		{
			if (local_address == memory_regions_.at(Region::BIOS).size() - 1)
			{
				bios_loaded_ = false;
			}
		}
		return memory_regions_.at(region).at(local_address);
	}
	catch (std::out_of_range &)
	{
		throw std::logic_error("Trying to read from non-implemented memory region, or out of memory region range");
	}
}

uint16_t MMU::Read16bitFromMemory(uint16_t absolute_address)
{
	uint16_t value{ Read8bitFromMemory(absolute_address) };
	value += static_cast<uint16_t>(Read8bitFromMemory(absolute_address + 1)) << 8;
	return value;
}

uint8_t MMU::Read8bitFromMemory(Region region, uint16_t local_address)
{
	return Read8bitFromMemory(LocalToAbsoluteAddress(region, local_address));
}

void MMU::Write8bitToMemory(uint16_t absolute_address, uint8_t value)
{
	Region region{ Region::VRAM };
	uint16_t local_address(absolute_address);
	std::tie(region, local_address) = AbsoluteToLocalAddress(absolute_address);

	try
	{
		memory_regions_.at(region).at(local_address) = value;
	}
	catch (std::out_of_range &)
	{
		throw std::logic_error("Trying to write to non-implemented memory region, or out of memory region range");
	}

	Notify(&MMUObserver::OnMemoryWrite, region, local_address, value);
}

void MMU::Write16bitToMemory(uint16_t absolute_address, uint16_t value)
{
	Write8bitToMemory(absolute_address, static_cast<uint8_t>(value & 0xFF));
	Write8bitToMemory(absolute_address + 1, static_cast<uint8_t>(value >> 8));
}

void MMU::LoadRom(std::string rom_file_path)
{
	try
	{
		std::ifstream rom_file(rom_file_path, std::ifstream::binary);
		if (rom_file)
		{
			rom_file.seekg(0, rom_file.end);
			auto file_size = rom_file.tellg();
			rom_file.seekg(0, rom_file.beg);
			if (file_size > memory_regions_.at(Region::ROM).size())
			{
				throw std::runtime_error("Could not open ROM file");
			}

			rom_file.read(reinterpret_cast<char*>(memory_regions_.at(Region::ROM).data()), file_size);
			if (!rom_file)
			{
				throw std::runtime_error("ROM file could not be completely read");
			}
		}
		else
		{
			throw std::runtime_error("Could not open ROM file");
		}
	}
	catch (std::out_of_range &)
	{
		throw std::runtime_error("Tried to anccess invalid memory region or out of range data when loading ROM");
	}
}

std::tuple<MMU::Region, uint16_t> MMU::AbsoluteToLocalAddress(uint16_t absolute_address) const
{
	try
	{
		for (auto& pair : region_start_addresses_)
		{
			if ((absolute_address >= pair.second)
				&& (absolute_address < (pair.second + memory_regions_.at(pair.first).size())))
			{
				if ((pair.first == Region::BIOS) && (!bios_loaded_))
				{
					continue;
				}
				return std::make_tuple(pair.first, absolute_address - pair.second);
			}
		}
		throw std::logic_error("Trying to convert an absolute address not found in any memory region");
	}
	catch (std::out_of_range &)
	{
		throw std::logic_error("Trying to convert an absolute address of a non-implemented memory region");
	}
}

uint16_t MMU::LocalToAbsoluteAddress(Region region, uint16_t local_address) const
{
	try
	{
		return region_start_addresses_.at(region) + local_address;
	}
	catch (std::out_of_range &)
	{
		throw std::logic_error("Trying to convert a local address of a non-implemented memory region");
	}
}
