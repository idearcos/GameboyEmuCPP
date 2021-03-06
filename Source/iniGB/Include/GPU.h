#pragma once

#include <map>
#include <memory>
#include <array>
#include "IMMU.h"
#include "IMMUObserver.h"
#include "IZ80Observer.h"
#include "GPU_States.h"
#include "GPU_TileSet.h"
#include "GPU_TileMap.h"
#include "GPU_Renderer.h"
#include "GPU_Palette.h"
#include "GPU_Background.h"
#include "GPU_Window.h"
#include "GPU_Sprite.h"

class GPU : public IMMUObserver, public IZ80Observer
{
public:
	GPU(GLFWwindow* &window, IMMU &mmu);
	~GPU() = default;

	void RenderScanLine();
	uint8_t IncrementCurrentLine();

	void OnClockLapse(const Clock &clock) override;
	void OnMemoryWrite(const Memory::Address &address, uint8_t value) override;

private:
	static std::map<Mode, std::unique_ptr<State>> InitStateMap();
	inline bool IsAddressInTileSet(uint16_t relative_address) const;
	inline bool IsAddressInTileMap(uint16_t relative_address, TileMap::Number tilemap_number) const;
	size_t GetAbsoluteTileNumber(TileMap::TileNumber tile_number, TileSet::Number tileset_number) const;

	void RefreshScreen();

	void SetCurrentMode(Mode new_mode);
	void SetCurrentLine(uint8_t line);
	void SetLineCompare(uint8_t line);
	void CompareLineAndUpdateRegister();

	void LcdOperation(bool enable);

	void WriteToMmu(const Memory::Address &address, uint8_t value) const;

private:
	// All addresses are relative to the beginning of VRAM
	static const uint16_t tileset1_start_{ 0x0000 };
	static const uint16_t tileset0_start_{ 0x0800 };
	static const size_t tileset_size_{ 0x1000 };
	static const size_t tileset_total_size_{ 0x1800 };
	static const uint16_t tilemap0_start_{ 0x1800 };
	static const uint16_t tilemap1_start_{ 0x1C00 };
	static const uint16_t tilemap_size_{ 0x0400 };

	static const size_t screen_width_{ 160 };
	static const size_t screen_height_{ 144 };
	static const size_t map_width_{ 32 };
	static const size_t map_height_{ 32 };
	static const size_t tile_width_{ 8 };
	static const size_t tile_height_{ 8 };
	static const size_t tile_size_{ tile_width_ * tile_height_ };
	static const size_t num_tiles_in_set_{ 384 };

	const Memory::Address interrupt_flags_register_{ Memory::Region::IO, 0x000F };
	const Memory::Address lcd_control_register_{ Memory::Region::IO, 0x0040 };
	const Memory::Address lcd_status_register_{ Memory::Region::IO, 0x0041 };
	const Memory::Address scroll_y_register_{ Memory::Region::IO, 0x0042 };
	const Memory::Address scroll_x_register_{ Memory::Region::IO, 0x0043 };
	const Memory::Address current_line_register_{ Memory::Region::IO, 0x0044 };
	const Memory::Address y_compare_register_{ Memory::Region::IO, 0x0045 };
	const Memory::Address dma_transfer_address_register_{ Memory::Region::IO, 0x0046 };
	const Memory::Address bg_palette_register_{ Memory::Region::IO, 0x0047 };
	const Memory::Address obj_palette_0_register_{ Memory::Region::IO, 0x0048 };
	const Memory::Address obj_palette_1_register_{ Memory::Region::IO, 0x0049 };
	const Memory::Address window_y_position_register_{ Memory::Region::IO, 0x004A };
	const Memory::Address window_x_position_plus_7_register_{ Memory::Region::IO, 0x004B };

	TileSet tileset_;
	std::map<TileMap::Number, TileMap> tilemaps_;
	Renderer renderer_;
	Background background_;
	Window window_;
	std::array<Sprite, 40> sprites_;
	Palette bg_and_window_palette_;
	std::map<ObjPalette, Palette> obj_palettes_;

	IMMU &mmu_;
	mutable bool writing_to_mmu_{ false };

	Mode current_mode_{ Mode::ReadingOAM };
	const std::map<Mode, std::unique_ptr<State>> states_;
	size_t ticks_current_period_{ 0 };

	uint8_t current_line_{ 0 };
	uint8_t line_compare_{ 0 };
	bool line_coincidence_{ false };

	// LCD Control
	bool lcd_on_{ false };
	bool sprites_on_{ false };
	TileMap::Number current_bg_tilemap_{ TileMap::Number::Zero };
	TileSet::Number current_bg_and_window_tileset_{ TileSet::Number::Zero };
	TileMap::Number current_window_tilemap_{ TileMap::Number::Zero };
	Sprite::Size sprites_size_{ Sprite::Size::Pixels8x8 };

	// LCD Status
	bool enable_hblank_interrupt_{ false };
	bool enable_vblank_interrupt_{ false };
	bool enable_oam_interrupt_{ false };
	bool enable_line_compare_interrupt_{ false };	

private:
	GPU(const GPU&) = delete;
	GPU(GPU&&) = delete;
	GPU& operator=(const GPU&) = delete;
	GPU& operator=(GPU&&) = delete;
};
