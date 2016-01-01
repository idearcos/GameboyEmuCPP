#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <array>

enum class ColorGB : uint8_t
{
	NoColorGB,
	Supported = 0x80,
	Exclusive = 0xC0
};

enum class CartridgeType : uint8_t
{
	ROM_Only = 0x00,
	MBC1 = 0x01,
	MBC1_RAM = 0x02,
	MBC1_RAM_Battery = 0x03,
	MBC2 = 0x05,
	MBC2_Battery = 0x06,
	RAM = 0x08,
	RAM_Battery = 0x09,
	MMM01 = 0x0B,
	MMM01_RAM = 0x0C,
	MMM01_RAM_Battery = 0x0D,
	MBC3_Timer_Battery = 0x0F,
	MBC3_Timer_RAM_Battery = 0x10,
	MBC3 = 0x11,
	MBC3_RAM = 0x12,
	MBC3_RAM_Battery = 0x13,
	MBC4 = 0x15,
	MBC4_RAM = 0x16,
	MBC4_RAM_Battery = 0x17,
	MBC5 = 0x19,
	MBC5_RAM = 0x1A,
	MBC5_RAM_Battery = 0x1B,
	MBC5_Rumble = 0x1C,
	MBC5_Rumble_RAM = 0x1D,
	MBC5_Rumble_RAM_Battery = 0x1E,
	PocketCamera = 0xFC,
	BandaiTama5 = 0xFD,
	HuC3 = 0xFE,
	HuC1_RAM_Battery = 0xFF
};

struct RomInfo
{
	RomInfo(const std::vector<uint8_t> &rom);

	std::array<uint8_t, 4> entry_point_;
	std::string title_;
	std::string manufacturer_;
	ColorGB is_color_gb_{ ColorGB::NoColorGB };
	uint16_t new_licensee_code_{ 0 };
	bool super_gb_functions_{ false };
	CartridgeType cartridge_type_{ CartridgeType::ROM_Only };
	size_t num_rom_banks_{ 0 };
	size_t external_ram_size_{ 0 };
	bool destination_japanese_{ 0 };
	uint8_t old_licensee_code_{ 0 };
	uint8_t version_number_{ 0 };
	uint8_t header_checksum_{ 0 };
	uint16_t global_checksum_{ 0 };
};
