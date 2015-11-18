#include "GPU.h"

#include <list>
#include <bitset>
#include <iostream>

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
		bg_and_window_palette_.SetColor(i, Color::Transparent);
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

bool GPU::IsAddressInTileSet(uint16_t relative_address) const
{
	return (relative_address >= tileset1_start_) && (relative_address < (tileset1_start_ + tileset_total_size_));
}

bool GPU::IsAddressInTileMap(uint16_t relative_address, TileMap::Number tilemap_number) const
{
	switch (tilemap_number)
	{
	case TileMap::Number::Zero:
		return ((relative_address >= tilemap0_start_) && (relative_address < (tilemap0_start_ + tilemap_size_)));
	case TileMap::Number::One:
		return ((relative_address >= tilemap1_start_) && (relative_address < (tilemap1_start_ + tilemap_size_)));
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
	if (lcd_on_)
	{
		ticks_current_period_ += clock.GetTicks();

		Mode new_mode{ Mode::ReadingOAM };
		std::tie(new_mode, ticks_current_period_) = states_.at(current_mode_)->Lapse(ticks_current_period_, *this);
		// We need the IO memory write side-effect when updating the current mode
		SetCurrentMode(new_mode);
	}
}

void GPU::OnMemoryWrite(const Memory::Address &address, uint8_t value)
{
	if (writing_to_mmu_)
	{
		return;
	}

	Memory::Region region{ Memory::Region::VRAM };
	uint16_t relative_address{ 0 };
	std::tie(region, relative_address) = address.GetRelativeAddress();
	if (Memory::Region::VRAM == region)
	{
		if (IsAddressInTileSet(relative_address))
		{
			// There are 384 tiles in the tile set. Get the index of the tile being modified (upper 12 bits)
			const size_t tile_index = relative_address >> 4;

			// The lower 4 bits of the address point to one of the 16 bytes that a tile is made of.
			const uint8_t byte_in_tile{ static_cast<uint8_t>(relative_address & 0xF) };

			// Each row of the tile consists of 2 bytes (2 bits per pixel * 8 pixels = 16 bits)
			// Due to the memory structure (high-bit and low-bit in consecutive bytes), each byte affects the whole row of the tile.
			// Get the y (row index)
			const uint8_t y{ static_cast<uint8_t>(byte_in_tile >> 1) };

			// Get the address of the first byte in this row (either current address, or the previous byte)
			const uint16_t row_beginning = ((relative_address & 1) != 0) ? relative_address - 1 : relative_address;

			const std::bitset<8> low_bits{ mmu_.Read8bitFromMemory(Memory::Address{ Memory::Region::VRAM, row_beginning }) };
			const std::bitset<8> high_bits{ mmu_.Read8bitFromMemory(Memory::Address{ Memory::Region::VRAM, static_cast<uint16_t>(row_beginning + 1) }) };
			for (uint8_t x = 0; x < 8; x++)
			{
				const uint8_t pixel_value = (low_bits.test(7 - x) ? 1 : 0) + (high_bits.test(7 - x) ? 2 : 0);
				tileset_.WritePixel(tile_index, x, y, pixel_value);
			}
		}
		else if (IsAddressInTileMap(relative_address, TileMap::Number::Zero))
		{
			const auto index = relative_address - tilemap0_start_;
			tilemaps_.at(TileMap::Number::Zero).SetTileNumber(index, value);
		}
		else if (IsAddressInTileMap(relative_address, TileMap::Number::One))
		{
			const auto index = relative_address - tilemap1_start_;
			tilemaps_.at(TileMap::Number::One).SetTileNumber(index, value);
		}		
	}
	else if (Memory::Region::OAM == region)
	{
		// 40 Sprites of 4 bytes each
		const uint8_t sprite_index = static_cast<uint8_t>((relative_address >> 2) & 0xFF);

		switch (relative_address & 0x03)
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
	else if (Memory::Region::IO == region)
	{
		if (lcd_control_register_ == address)
		{
			background_.Enable((value & 0x01) != 0);
			sprites_on_ = (value & 0x02) != 0;
			sprites_size_ = ((value & 0x04) != 0) ? Sprite::Size::Pixels8x16 : Sprite::Size::Pixels8x8;
			current_bg_tilemap_ = ((value & 0x08) != 0) ? TileMap::Number::One : TileMap::Number::Zero;
			current_bg_and_window_tileset_ = ((value & 0x10) != 0) ? TileSet::Number::One : TileSet::Number::Zero;
			//window_.Enable((value & 0x20) != 0);
			current_window_tilemap_ = ((value & 0x40) != 0) ? TileMap::Number::One : TileMap::Number::Zero;
			LcdOperation((value & 0x80) != 0);
		}
		else if (lcd_status_register_ == address)
		{
			enable_hblank_interrupt_ = (value & 0x08) != 0;
			enable_vblank_interrupt_ = (value & 0x10) != 0;
			enable_oam_interrupt_ = (value & 0x20) != 0;
			enable_line_compare_interrupt_ = (value & 0x40) != 0;
		}
		else if (scroll_y_register_ == address)
		{
			background_.SetScrollY(value);
		}
		else if (scroll_x_register_ == address)
		{
			background_.SetScrollX(value);
		}
		else if (current_line_register_ == address)
		{
			std::cout << "Trying to write current scanline (read-only register)" << std::endl;
		}
		else if (y_compare_register_ == address)
		{
			SetLineCompare(value);
		}
		else if (dma_transfer_address_register_ == address)
		{
			// Copy all memory from the starting DMA address in either ROM or RAM, to OAM
			// (The sprites will be updated when listening to the changes in MMU)
			const Memory::Address dma_start_address{ static_cast<uint16_t>(value << 8) };
			for (uint16_t i = 0; i < 160; i++)
			{
				// Do not use WriteToMmu, as we need to be notified
				mmu_.Write8bitToMemory(Memory::Address(Memory::Region::OAM, i), mmu_.Read8bitFromMemory(Memory::Address{ dma_start_address + i }));
			}
		}
		else if (bg_palette_register_ == address)
		{
			bg_and_window_palette_.SetPaletteData(value);
		}
		else if (obj_palette_0_register_ == address)
		{
			obj_palettes_[ObjPalette::Zero].SetPaletteData(value);
		}
		else if (obj_palette_1_register_ == address)
		{
			obj_palettes_[ObjPalette::One].SetPaletteData(value);
		}
		else if (window_y_position_register_ == address)
		{
			//window_.SetPositionY(value);
		}
		else if (window_x_position_plus_7_register_ == address)
		{
			//window_.SetPositionX(value - 7);
		}
	}
}

void GPU::RenderScanLine()
{
	if (lcd_on_)
	{
		background_.RenderLine(renderer_, tileset_, current_bg_and_window_tileset_, tilemaps_.at(current_bg_tilemap_), bg_and_window_palette_, current_line_);

		//window_.RenderLine(renderer_, tileset_, current_bg_and_window_tileset_, tilemaps_.at(current_window_tilemap_), bg_and_window_palette_, current_line_);

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

uint8_t GPU::IncrementCurrentLine()
{
	SetCurrentLine(current_line_ + 1);

	return current_line_;
}

void GPU::SetCurrentMode(Mode new_mode)
{
	auto lcd_status = mmu_.Read8bitFromMemory(lcd_status_register_);
	lcd_status &= ~(0x03);
	lcd_status |= static_cast<std::underlying_type_t<Mode>>(new_mode);
	// Bit 7 is unused, leave it set to 1
	lcd_status |= 0x80;
	WriteToMmu(lcd_status_register_, lcd_status);

	// If the mode has changed, signal interrupts of VBLANK and/or LCDSTAT (if enabled)
	if (new_mode != current_mode_)
	{
		switch (new_mode)
		{
		case Mode::HBlank:
			if (enable_hblank_interrupt_)
			{
				{ auto interrupt_flags = mmu_.Read8bitFromMemory(interrupt_flags_register_);
				interrupt_flags |= 0x02;
				// Bits 5-7 are unused, leave them set to 1
				interrupt_flags |= 0xE0;
				WriteToMmu(interrupt_flags_register_, interrupt_flags); }
			}
			break;

		case Mode::VBlank:
			// Always request VBlank interrupt
			{auto interrupt_flags = mmu_.Read8bitFromMemory(interrupt_flags_register_);
			interrupt_flags |= 0x01;

			// Request LCD status interrupt only if enabled
			if (enable_vblank_interrupt_)
			{
				interrupt_flags |= 0x02;
			}

			// Bits 5-7 are unused, leave them set to 1
			interrupt_flags |= 0xE0;
			WriteToMmu(interrupt_flags_register_, interrupt_flags); }
			break;

		case Mode::ReadingOAM:
			if (enable_oam_interrupt_)
			{
				{ auto interrupt_flags = mmu_.Read8bitFromMemory(interrupt_flags_register_);
				interrupt_flags |= 0x02;
				// Bits 5-7 are unused, leave them set to 1
				interrupt_flags |= 0xE0;
				WriteToMmu(interrupt_flags_register_, interrupt_flags); }
			}
			break;
		}
	}

	current_mode_ = new_mode;
}

void GPU::SetLineCompare(uint8_t line)
{
	line_compare_ = line;
	CompareLineAndUpdateRegister();
}

void GPU::LcdOperation(bool enable)
{
	const bool did_change{ lcd_on_ != enable };
	lcd_on_ = enable;
	if (did_change)
	{
		SetCurrentLine(0);
		ticks_current_period_ = 0;
		if (lcd_on_)
		{
			// The LCD probably doesnt turn on immediately... adjustments might be necessary regarding its initial status
			SetCurrentMode(Mode::ReadingOAM);
		}
		else
		{
			SetCurrentMode(Mode::HBlank);
		}
	}
}

void GPU::SetCurrentLine(uint8_t line)
{
	current_line_ = line;
	if (current_line_ >= 154)
	{
		current_line_ = 0;
	}
	WriteToMmu(current_line_register_, current_line_);
	CompareLineAndUpdateRegister();
}

void GPU::CompareLineAndUpdateRegister()
{
	const auto line_coincidence = (current_line_ == line_compare_);
	{auto lcd_status = mmu_.Read8bitFromMemory(lcd_status_register_);
	if (line_coincidence)
	{
		lcd_status |= 0x04;
	}
	else
	{
		lcd_status &= ~(0x04);
	}
	// Bit 7 is unused, leave it set to 1
	lcd_status |= 0x80;
	WriteToMmu(lcd_status_register_, lcd_status); }

	if (enable_line_compare_interrupt_ && line_coincidence)
	{
		auto interrupt_flags = mmu_.Read8bitFromMemory(interrupt_flags_register_);
		interrupt_flags |= 0x02;
		// Bits 5-7 are unused, leave them set to 1
		interrupt_flags |= 0xE0;
		WriteToMmu(interrupt_flags_register_, interrupt_flags);
	}
}

void GPU::WriteToMmu(const Memory::Address &address, uint8_t value) const
{
	writing_to_mmu_ = true;
	mmu_.Write8bitToMemory(address, value);
	writing_to_mmu_ = false;
}
