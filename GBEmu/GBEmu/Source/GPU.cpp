#include "GPU.h"

#include <list>

GPU::GPU(MMU &mmu) :
	states_(InitStateMap()),
	tileset_(num_tiles_in_set_, tile_width_, tile_height_),
	mmu_(mmu),
	framebuffer_(screen_width_ * screen_height_ * 3, 0)
{
	tilemaps_.emplace(std::piecewise_construct, std::forward_as_tuple(TileMap::Number::Zero), std::forward_as_tuple(map_width_, map_height_, tile_width_, tile_height_));
	tilemaps_.emplace(std::piecewise_construct, std::forward_as_tuple(TileMap::Number::One), std::forward_as_tuple(map_width_, map_height_, tile_width_, tile_height_));

	for (auto i = 0; i < 4; i++)
	{
		palette_[i] = std::array<uint8_t, 3>{{ 255, 255, 255 }};
	}
	
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
	return (address >= tileset1_start_) && (address < (tileset1_start_ + tileset_total_size_));
}

bool GPU::IsAddressInTileMap(uint16_t address, TileMap::Number tilemap_number) const
{
	switch (tilemap_number)
	{
	case TileMap::Number::Zero:
		return ((address >= tilemap0_start_) && (address < (tilemap0_start_ + tilemap_size_)));
	case TileMap::Number::One:
		return ((address >= tilemap1_start_) && (address < (tilemap1_start_ + tilemap_size_)));
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
		throw std::logic_error("Trying to get tile number from non-implemented tileset");
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
	if (MMU::Region::VRAM == region)
	{
		if (IsAddressInTileSet(addr))
		{
			// There are 384 tiles in the tile set. Get the index of the tile being modified (upper 12 bits)
			const size_t tile_index = addr >> 4;

			// The lower 4 bits of the address point to one of the 16 bytes that a tile is made of.
			const uint8_t byte_in_tile{ static_cast<uint8_t>(addr & 0xF) };

			// Each row of the tile consists of 2 bytes (2 bits per pixel * 8 pixels = 16 bits)
			// Due to the memory structure (high-bit and low-bit in consecutive bytes), each byte affects the whole row of the tile.
			// Get the y (row index)
			const uint8_t y{ static_cast<uint8_t>(byte_in_tile >> 1) };

			// Get the address of the first byte in this row (either current address, or the previous byte)
			const uint16_t row_beginning = (addr & 1) != 0 ? addr - 1 : addr;

			for (uint8_t x = 0; x < 8; x++)
			{
				const uint8_t value = (mmu_.Read8bitFromMemory(MMU::Region::VRAM, row_beginning) & (1 << (7 - x)))
					| ((mmu_.Read8bitFromMemory(MMU::Region::VRAM, row_beginning + 1) & (1 << (7 - x))) << 1);
				tileset_.WritePixel(tile_index, x, y, value);
			}
		}
		else if (IsAddressInTileMap(addr, TileMap::Number::Zero))
		{
			const auto index = addr - tilemap0_start_;
			tilemaps_.at(TileMap::Number::Zero).SetTileNumber(index, value);
		}
		else if (IsAddressInTileMap(addr, TileMap::Number::One))
		{
			const auto index = addr - tilemap1_start_;
			tilemaps_.at(TileMap::Number::One).SetTileNumber(index, value);
		}		
	}
	else if (MMU::Region::IO == region)
	{
		if (0x0040 == addr)
		{
			background_on_ = (value & 0x01) != 0;
			sprites_on_ = (value & 0x02) != 0;
			//TODO sprite_size = value & 0x04
			current_bg_tilemap_ = ((value & 0x08) != 0) ? TileMap::Number::One : TileMap::Number::Zero;
			current_bg_tileset_ = ((value & 0x10) != 0) ? TileSet::Number::One : TileSet::Number::Zero;
			window_on_ = (value & 0x20) != 0;
			current_window_tilemap_ = ((value & 0x40) != 0) ? TileMap::Number::One : TileMap::Number::Zero;
			lcd_on_ = (value & 0x80) != 0;
		}
		else if (0x0042 == addr)
		{
			bg_scroll_y_ = value;
		}
		else if (0x0043 == addr)
		{
			bg_scroll_x_ = value;
		}
		else if (0x0044 == addr)
		{
			throw std::runtime_error("Trying to write current scanline (read-only register)");
		}
		else if (0x0047 == addr)
		{
			for (auto i = 0; i < 4; i++)
			{
				switch ((value >> (i * 2)) & 3)
				{
				case 0:
					palette_[i] = std::array<uint8_t, 3>{{ 255, 255, 255 }};
					break;
				case 1:
					palette_[i] = std::array<uint8_t, 3>{{ 192, 192, 192 }};
					break;
				case 2:
					palette_[i] = std::array<uint8_t, 3>{{ 96, 96, 96 }};
					break;
				case 3:
					palette_[i] = std::array<uint8_t, 3>{{ 0, 0, 0 }};
					break;
				}
			}
		}
	}
}

void GPU::RenderScanLine()
{
	if (lcd_on_)
	{
		if (background_on_)
		{
			// The pixel offset inside the first tile to be drawn, depending on the horizontal scroll of the background (last 3 bits, 0-7)
			auto x_offset_in_tile = bg_scroll_x_ & 0x07;

			// The line (of the tiles) to be drawn, depending on the vertical scroll of the background and the current line being drawn (last 3 bits, 0-7)
			const uint8_t line_in_tile{ static_cast<uint8_t>((current_line_ + bg_scroll_y_) & 0x07) };

			size_t pixels_drawn = 0;
			while (pixels_drawn < screen_width_)
			{
				const auto tile_number = tilemaps_.at(current_bg_tilemap_).GetTileNumber(current_line_, bg_scroll_y_, bg_scroll_x_ + pixels_drawn);

				const auto tile = tileset_.GetTile(tile_number);

				for (auto x_in_tile = x_offset_in_tile; (x_in_tile < tile_width_) && (pixels_drawn < screen_width_); x_in_tile++)
				{
					try
					{
						const auto rgb = palette_.at(tile.ReadPixel(x_in_tile, line_in_tile));
						for (int i = 0; i < rgb.size(); i++)
						{
							framebuffer_[screen_width_ * current_line_ * 3 + pixels_drawn * 3 + i] = rgb[i];
						}
					}
					catch (std::out_of_range &)
					{
						throw std::runtime_error("Trying to access invalid palette");
					}
					pixels_drawn += 1;
				}

				x_offset_in_tile += pixels_drawn;
				x_offset_in_tile &= 0x07;
			}
		}
	}
}

void GPU::RefreshScreen()
{
	renderer_.RefreshScreen(framebuffer_);
}
