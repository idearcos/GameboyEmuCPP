#include "GPU.h"

GPU::GPU() :
	states_(InitStateMap())
{
	vram_.fill(0);
}

std::map<GPU::Mode, std::unique_ptr<GPU::State>> GPU::InitStateMap()
{
	std::map<Mode, std::unique_ptr<State>> states;
	states.emplace(Mode::ReadingOAM, std::make_unique<ReadingOAM>());
	states.emplace(Mode::ReadingVRAM, std::make_unique<ReadingVRAM>());
	states.emplace(Mode::HBlank, std::make_unique<HBlank>());
	states.emplace(Mode::VBlank, std::make_unique<VBlank>());
	return states;
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

		// There are 384 tiles in the tile set. Get the index of the tile being modified (upper 12 bits)
		const uint16_t tile_index = addr >> 4;
		// The lower 4 bits point to one of the 16 bytes that a tile is made of.
		const uint8_t byte_index = addr & 0xF;
		// Each row of the tile consists of 2 bytes (2 bits per pixel * 8 pixels = 16 bits)
		// Get the y (row index)
		const uint8_t y = byte_index >> 1;
		// Get the address of the first byte in this row
		const uint16_t row_beginning = (addr & 1) != 0 ? addr - 1 : addr;

		for (auto x = 0; x < 8; x++)
		{
			uint8_t value = (vram_[row_beginning] & (1 << (7 - x))) | ((vram_[row_beginning + 1] & (1 << (7 - x))) << 1);
			tileset_.WriteTileData(tile_index, x, y, value);
		}
	}
}

void GPU::RenderScanLine()
{
	if (lcd_on_)
	{
		if (background_on_)
		{
			uint16_t tile_map_base{ 0 };
			switch (background_tile_map_)
			{
			case TileMapIndex::Zero:
				tile_map_base += 0x1800;
				break;
			case TileMapIndex::One:
				tile_map_base += 0x1C00;
				break;
			default:
				throw std::logic_error("Trying to access invalid tile map");
			}

			// Current line and scroll y are counted in pixels.
			// Divide by 8 (rounding down) to get tile row index to be used.
			const uint8_t tile_row_index{ ((current_line_ + bg_scroll_y) & 0xFF) >> 3 };

			// The first line index inside the tile to be used is given by the remainder of the previous division.
			const uint8_t line_in_tile{ (current_line_ + bg_scroll_y) & 0x07 };

			// Divide scroll x (in pixels) by 8 (rounding down) to get tile column index to be used.
			const uint8_t tile_column_index{ bg_scroll_x >> 3 };

			// 32 bytes per row => left shift 5
			const uint16_t tile_address = tile_map_base + (tile_row_index << 5) + tile_column_index;

			//auto tile_index{}
		}
	}
}

std::tuple<GPU::Mode, uint8_t> GPU::State::Lapse(uint8_t ticks_current_period, GPU &gpu)
{
	if (ticks_current_period >= ticks_per_period_)
	{
		return std::make_tuple(Transition(gpu), ticks_current_period - ticks_per_period_);
	}
	else
	{
		return std::make_tuple(Mode::VBlank, ticks_current_period);
	}
}

GPU::Mode GPU::ReadingVRAM::Transition(GPU &gpu)
{
	gpu.RenderScanLine();
	return Mode::HBlank;
}

GPU::Mode GPU::HBlank::Transition(GPU &gpu)
{
	if (gpu.IncrementCurrentLine() >= 144)
	{
		gpu.RefreshScreen();
		return Mode::VBlank;
	}
	else
	{
		return Mode::ReadingOAM;
	}	
}

GPU::Mode GPU::VBlank::Transition(GPU &gpu)
{
	if (gpu.IncrementCurrentLine() >= 154)
	{
		return Mode::ReadingOAM;
	}
	else
	{
		return Mode::VBlank;
	}

}

void GPU::TileSet::WriteTileData(uint32_t tile_index, uint8_t x, uint8_t y, uint8_t value)
{
	tileset_data_[tile_index * tile_width_ * tile_height_ + y * tile_width_ + x] = value;
}
