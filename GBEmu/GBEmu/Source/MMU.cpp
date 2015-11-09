#include "MMU.h"
#include <fstream>
#include <iostream>

MMU::MMU() :
	bios_({
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
		0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50 })
{
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Memory::Region::VRAM), std::forward_as_tuple(SizeOfRegion(Memory::Region::VRAM), 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Memory::Region::ERAM), std::forward_as_tuple(SizeOfRegion(Memory::Region::ERAM), 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Memory::Region::WRAM), std::forward_as_tuple(SizeOfRegion(Memory::Region::WRAM), 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Memory::Region::WRAM_ECHO), std::forward_as_tuple(SizeOfRegion(Memory::Region::WRAM_ECHO), 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Memory::Region::OAM), std::forward_as_tuple(SizeOfRegion(Memory::Region::OAM), 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Memory::Region::IO), std::forward_as_tuple(SizeOfRegion(Memory::Region::IO), 0));
	memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Memory::Region::ZRAM), std::forward_as_tuple(SizeOfRegion(Memory::Region::ZRAM), 0));
}

uint8_t MMU::Read8bitFromMemory(const Memory::Address &address) const
{
	try
	{
		Memory::Region region{ Memory::Region::ROM };
		uint16_t relative_address{ 0 };
		std::tie(region, relative_address) = address.GetRelativeAddress();
		if (bios_loaded_ && (Memory::Region::ROM == region) && (relative_address < bios_.size()))
		{
			if (relative_address == bios_.size() - 1)
			{
				bios_loaded_ = false;
			}
			return bios_.at(relative_address);
		}
		return memory_regions_.at(region).at(relative_address);
	}
	catch (std::out_of_range &)
	{
		std::cout << "Trying to read out of memory region range: " <<
			"Address: 0x" << std::hex << address.GetAbsoluteAddress() << std::endl;
		return 0;
	}
}

uint16_t MMU::Read16bitFromMemory(const Memory::Address &address) const
{
	uint16_t value{ Read8bitFromMemory(address) };
	value += static_cast<uint16_t>(Read8bitFromMemory(address + 1)) << 8;
	return value;
}

void MMU::Write8bitToMemory(const Memory::Address &address, uint8_t value)
{
	Memory::Region region{ Memory::Region::ROM };
	uint16_t relative_address{ 0 };
	std::tie(region, relative_address) = address.GetRelativeAddress();

	if (Memory::Region::ROM == region)
	{
		std::cout << "Trying to write into ROM" << std::endl;
	}

	try
	{
		memory_regions_.at(region).at(relative_address) = value;
		if (Memory::Region::WRAM_ECHO == region)
		{
			memory_regions_.at(Memory::Region::WRAM).at(relative_address) = value;
		}
		else if ((Memory::Region::WRAM == region) && (relative_address < memory_regions_.at(Memory::Region::WRAM_ECHO).size()))
		{
			memory_regions_.at(Memory::Region::WRAM_ECHO).at(relative_address) = value;
		}
	}
	catch (std::out_of_range &)
	{
		std::cout << "Trying to write out of memory region range: " <<
			"Address: 0x" << std::hex << address.GetAbsoluteAddress() << ", value: " << static_cast<size_t>(value) << std::endl;
	}

	Notify(&MMUObserver::OnMemoryWrite, address, value);
}

void MMU::Write16bitToMemory(const Memory::Address &address, uint16_t value)
{
	Write8bitToMemory(address, static_cast<uint8_t>(value & 0xFF));
	Write8bitToMemory(address + 1, static_cast<uint8_t>(value >> 8));
}

void MMU::LoadRom(std::string rom_file_path)
{
	try
	{
		std::ifstream rom_file(rom_file_path, std::ifstream::binary);
		if (rom_file)
		{
			rom_file.seekg(0, rom_file.end);
			auto file_size = static_cast<size_t>(rom_file.tellg());
			rom_file.seekg(0, rom_file.beg);

			memory_regions_.emplace(std::piecewise_construct, std::forward_as_tuple(Memory::Region::ROM), std::forward_as_tuple(file_size, 0));

			rom_file.read(reinterpret_cast<char*>(memory_regions_.at(Memory::Region::ROM).data()), file_size);
			if (!rom_file)
			{
				throw std::runtime_error("ROM file could not be completely read");
			}

			RomInfo rom_info{ memory_regions_.at(Memory::Region::ROM) };
			uint8_t header_checksum{ 0 };
			for (auto i = 0x0134; i <= 0x014C; i++)
			{
				header_checksum -= memory_regions_.at(Memory::Region::ROM).at(i) + 1;
			}
			if (header_checksum != rom_info.header_checksum_)
			{
				throw std::runtime_error("Header checksum of cartridge failed");
			}
			uint16_t global_checksum{ 0 };
			for (auto i = 0; i < memory_regions_.at(Memory::Region::ROM).size(); i++)
			{
				if ((i == 0x14E) || (i == 0x14F))
				{
					continue;
				}
				global_checksum -= memory_regions_.at(Memory::Region::ROM).at(i) + 1;
			}
			if (global_checksum != rom_info.global_checksum_)
			{
				std::cout << "Global checksum of cartridge failed" << std::endl;
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
