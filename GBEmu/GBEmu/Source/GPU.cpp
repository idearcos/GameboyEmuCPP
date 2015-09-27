#include "GPU.h"

#include <list>

GPU::GPU() :
	states_(InitStateMap()),
	tileset_(num_tiles_in_set, tile_width, tile_height)
{
	vram_.fill(0);
}

std::map<Mode, std::unique_ptr<State>> GPU::InitStateMap()
{
	std::map<Mode, std::unique_ptr<State>> states;
	states.emplace(Mode::ReadingOAM, std::make_unique<ReadingOAM>());
	states.emplace(Mode::ReadingVRAM, std::make_unique<ReadingVRAM>());
	states.emplace(Mode::HBlank, std::make_unique<HBlank>());
	states.emplace(Mode::VBlank, std::make_unique<VBlank>());
	return states;
}

bool GPU::IsAddressInTileSet(uint16_t address) const
{
	return (address >= tileset1_start) && (address < (tileset1_start + tileset_total_size));
}

bool GPU::IsAddressInTileMap(uint16_t address, TileMap::Number tilemap_number) const
{
	switch (tilemap_number)
	{
	case TileMap::Number::Zero:
		return ((address >= tilemap0_start) && (address < (tilemap0_start + tilemap_size)));
	case TileMap::Number::One:
		return ((address >= tilemap1_start) && (address < (tilemap1_start + tilemap_size)));
	default:
		throw std::logic_error("Trying to verify address in wrong numbered tilemap");
	}
}

size_t GPU::GetAbsoluteTileNumber(uint8_t tile_number, TileSet::Number tileset_number) const
{
	switch (tileset_number)
	{
	case TileSet::Number::Zero:
		return static_cast<int8_t>(tile_number) + 256;
	case TileSet::Number::One:
		return tile_number;
	default:
		break;
	}
}

void GPU::Lapse(const Clock &clock)
{
	ticks_current_period_ += clock.GetTicks();

	std::tie(current_mode_, ticks_current_period_) = states_.at(current_mode_)->Lapse(ticks_current_period_, *this);
}

void GPU::OnMemoryWrite(MMU::Region region, uint16_t addr, uint8_t value)
{
	if (MMU::Region::Vram == region)
	{
		vram_[addr] = value;

		if (IsAddressInTileSet(addr))
		{
			// There are 384 tiles in the tile set. Get the index of the tile being modified (upper 12 bits)
			const uint16_t tile_index{ addr >> 4 };

			// The lower 4 bits of the address point to one of the 16 bytes that a tile is made of.
			const uint8_t byte_in_tile{ addr & 0xF };

			// Each row of the tile consists of 2 bytes (2 bits per pixel * 8 pixels = 16 bits)
			// Due to the memory structure (high-bit and low-bit in consecutive bytes), each byte affects the whole row of the tile.
			// Get the y (row index)
			const uint8_t y{ byte_in_tile >> 1 };

			// Get the address of the first byte in this row (either current address, or the previous byte)
			const uint16_t row_beginning = (addr & 1) != 0 ? addr - 1 : addr;

			for (uint8_t x = 0; x < 8; x++)
			{
				uint8_t value = (vram_[row_beginning] & (1 << (7 - x))) | ((vram_[row_beginning + 1] & (1 << (7 - x))) << 1);
			}
		}

		if (IsAddressInTileMap(addr, TileMap::Number::Zero))
		{
			const uint8_t tile_number = (addr - tileset1_start) / (tileset_size / num_tiles_in_set);
		}
		else if (IsAddressInTileMap(addr, TileMap::Number::One))
		{
			const int8_t tile_number = (addr - (tileset0_start + (tileset_size / 2))) / (tileset_size / num_tiles_in_set);
		}

		// There are 384 tiles in the tile set. Get the index of the tile being modified (upper 12 bits)
		const uint16_t tile_index = addr >> 4;
		
	}
}

void GPU::RenderScanLine()
{
	if (lcd_on_)
	{
		if (background_on_)
		{
			const auto tile_number = tilemaps_.at(current_bg_tilemap_).GetTileNumber(current_line_, bg_scroll_y_, bg_scroll_x_);

			// The first line index inside the tile to be used is given by the remainder of the previous division.
			//const uint8_t line_in_tile{ static_cast<uint8_t>((current_line_ + bg_scroll_y_) & 0x07) };
		}
	}
}
