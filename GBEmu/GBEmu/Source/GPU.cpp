#include "GPU.h"

#include <list>
#include <bitset>

GPU::GPU(GLFWwindow* &window, IMMU &mmu) :
	states_(InitStateMap()),
	tileset_(num_tiles_in_set_, tile_width_, tile_height_),
	renderer_(window, screen_width_, screen_height_),
	background_(screen_width_, screen_width_),
	mmu_(mmu)
{
	tilemaps_.emplace(std::piecewise_construct, std::forward_as_tuple(TileMap::Number::Zero), std::forward_as_tuple(map_width_, map_height_, tile_width_, tile_height_));
	tilemaps_.emplace(std::piecewise_construct, std::forward_as_tuple(TileMap::Number::One), std::forward_as_tuple(map_width_, map_height_, tile_width_, tile_height_));

	for (auto i = 0; i < 4; i++)
	{
		bg_palette_.SetColor(i, Color::Transparent);
		obj_palettes_[ObjPalette::Zero].SetColor(i, Color::Transparent);
		obj_palettes_[ObjPalette::One].SetColor(i, Color::Transparent);
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

void GPU::OnClockLapse(const Clock &clock)
{
	ticks_current_period_ += clock.GetTicks();

	Mode new_mode{Mode::ReadingOAM};
	std::tie(new_mode, ticks_current_period_) = states_.at(current_mode_)->Lapse(ticks_current_period_, *this);
	// We need the IO memory write side-effect when updating the current mode
	SetCurrentMode(new_mode);
}

void GPU::OnMemoryWrite(Region region, uint16_t address, uint8_t value)
{
	if (writing_to_mmu_)
	{
		return;
	}

	if (Region::VRAM == region)
	{
		if (IsAddressInTileSet(address))
		{
			// There are 384 tiles in the tile set. Get the index of the tile being modified (upper 12 bits)
			const size_t tile_index = address >> 4;

			// The lower 4 bits of the address point to one of the 16 bytes that a tile is made of.
			const uint8_t byte_in_tile{ static_cast<uint8_t>(address & 0xF) };

			// Each row of the tile consists of 2 bytes (2 bits per pixel * 8 pixels = 16 bits)
			// Due to the memory structure (high-bit and low-bit in consecutive bytes), each byte affects the whole row of the tile.
			// Get the y (row index)
			const uint8_t y{ static_cast<uint8_t>(byte_in_tile >> 1) };

			// Get the address of the first byte in this row (either current address, or the previous byte)
			const uint16_t row_beginning = ((address & 1) != 0) ? address - 1 : address;

			const std::bitset<8> low_bits{ mmu_.Read8bitFromMemory(Region::VRAM, row_beginning) };
			const std::bitset<8> high_bits{ mmu_.Read8bitFromMemory(Region::VRAM, row_beginning + 1) };
			for (uint8_t x = 0; x < 8; x++)
			{
				const uint8_t pixel_value = (low_bits.test(7 - x) ? 1 : 0) + (high_bits.test(7 - x) ? 2 : 0);
				tileset_.WritePixel(tile_index, x, y, pixel_value);
			}
		}
		else if (IsAddressInTileMap(address, TileMap::Number::Zero))
		{
			const auto index = address - tilemap0_start_;
			tilemaps_.at(TileMap::Number::Zero).SetTileNumber(index, value);
		}
		else if (IsAddressInTileMap(address, TileMap::Number::One))
		{
			const auto index = address - tilemap1_start_;
			tilemaps_.at(TileMap::Number::One).SetTileNumber(index, value);
		}		
	}
	else if (Region::OAM == region)
	{
		// 40 Sprites of 4 bytes each
		const uint8_t sprite_index = static_cast<uint8_t>((address >> 2) & 0xFF);

		switch (address & 0x03)
		{
		// Byte 0: Y position
		case 0:
			sprites_[sprite_index].SetPositionY(value);
			break;
		// Byte 1: X position
		case 1:
			sprites_[sprite_index].SetPositionX(value);
			break;
		// Byte 2: Tile number
		case 2:
			sprites_[sprite_index].SetTileNumber(value);
			break;
		// Byte 3: Flags
		case 3:
			sprites_[sprite_index].SetFlags(value);
			break;
		}
	}
	else if (Region::IO == region)
	{
		if (lcd_control_register == address)
		{
			background_.EnableBackground((value & 0x01) != 0);
			sprites_on_ = (value & 0x02) != 0;
			sprites_size_ = ((value & 0x04) != 0) ? Sprite::Size::Pixels8x16 : Sprite::Size::Pixels8x8;
			current_bg_tilemap_ = ((value & 0x08) != 0) ? TileMap::Number::One : TileMap::Number::Zero;
			current_bg_tileset_ = ((value & 0x10) != 0) ? TileSet::Number::One : TileSet::Number::Zero;
			window_on_ = (value & 0x20) != 0;
			current_window_tilemap_ = ((value & 0x40) != 0) ? TileMap::Number::One : TileMap::Number::Zero;
			lcd_on_ = (value & 0x80) != 0;
		}
		else if (lcd_status_register == address)
		{

		}
		else if (scroll_y_register == address)
		{
			background_.SetScrollY(value);
		}
		else if (scroll_x_register == address)
		{
			background_.SetScrollX(value);
		}
		else if (current_line_register == address)
		{
			throw std::runtime_error("Trying to write current scanline (read-only register)");
		}
		else if (y_compare_register == address)
		{
			SetLineCompare(value);
		}
		else if (bg_palette_register == address)
		{
			bg_palette_.SetPaletteData(value);
		}
		else if (obj_palette_0_register == address)
		{
			obj_palettes_[ObjPalette::Zero].SetPaletteData(value);
		}
		else if (obj_palette_1_register == address)
		{
			obj_palettes_[ObjPalette::One].SetPaletteData(value);
		}
		else if (window_y_position_register == address)
		{

		}
		else if (window_x_position_plus_7_register == address)
		{

		}
	}
}

void GPU::RenderScanLine()
{
	if (lcd_on_)
	{
		background_.RenderBackground(renderer_, tileset_, tilemaps_.at(current_bg_tilemap_), bg_palette_, current_line_);

		if (sprites_on_)
		{
			if (current_line_ < 144)
			{
				for (const auto &sprite : sprites_)
				{
					sprite.RenderSprite(renderer_, tileset_, obj_palettes_, sprites_size_, current_line_);
				}
			}
		}
	}
}

void GPU::RefreshScreen()
{
	renderer_.RefreshScreen();
}

void GPU::ResetCurrentLine()
{
	current_line_ = 0;
}

uint8_t GPU::IncrementCurrentLine()
{
	WriteToMmu(Region::IO, current_line_register, ++current_line_);
	CompareLineAndUpdateRegister();
	return current_line_;
}

void GPU::SetCurrentMode(Mode new_mode)
{
	if (new_mode != current_mode_)
	{
		auto lcd_status = mmu_.Read8bitFromMemory(Region::IO, lcd_status_register);
		lcd_status &= ~(0x03);
		lcd_status |= static_cast<std::underlying_type_t<Mode>>(new_mode);
		WriteToMmu(Region::IO, lcd_status_register, lcd_status);
	}
	current_mode_ = new_mode;
}

void GPU::SetLineCompare(uint8_t line)
{
	line_compare_ = line;
	CompareLineAndUpdateRegister();
}

void GPU::CompareLineAndUpdateRegister()
{
	auto lcd_status = mmu_.Read8bitFromMemory(Region::IO, lcd_status_register);
	lcd_status &= ~(0x04);
	line_coincidence_ = (current_line_ == line_compare_);
	if (line_coincidence_)
	{
		lcd_status |= 1 << 2;
	}
	else
	{
		lcd_status &= ~(1 << 2);
	}
	WriteToMmu(Region::IO, lcd_status_register, lcd_status);

	if (enable_line_compare_interrupt_)
	{
		//TODO implement interrupt
	}
}

void GPU::WriteToMmu(Region region, uint16_t address, uint8_t value) const
{
	writing_to_mmu_ = true;
	mmu_.Write8bitToMemory(region, address, value);
	writing_to_mmu_ = false;
}
