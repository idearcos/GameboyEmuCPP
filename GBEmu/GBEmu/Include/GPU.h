#pragma once

#include <map>
#include <memory>
#include <array>
#include "Clock.h"
#include "IMMU.h"
#include "MMUObserver.h"
#include "GPU_States.h"
#include "GPU_TileSet.h"
#include "GPU_TileMap.h"
#include "GPU_Renderer.h"
#include "GPU_Palette.h"
#include "GPU_Background.h"
#include "GPU_Sprite.h"

class GPU : public MMUObserver
{
public:
	GPU(GLFWwindow* &window, IMMU &mmu);
	~GPU() = default;

	void Lapse(const Clock &clock);
	void RenderScanLine();
	void RefreshScreen();

	void ResetCurrentLine();
	uint8_t IncrementCurrentLine();
	void SetCurrentMode(Mode new_mode);
	void SetLineCompare(uint8_t line);

	void OnMemoryWrite(Region region, uint16_t address, uint8_t value) override;

private:
	static std::map<Mode, std::unique_ptr<State>> InitStateMap();
	inline bool IsAddressInTileSet(uint16_t address) const;
	inline bool IsAddressInTileMap(uint16_t address, TileMap::Number tilemap_number) const;
	size_t GetAbsoluteTileNumber(TileMap::TileNumber tile_number, TileSet::Number tileset_number) const;

	void CompareLineAndUpdateRegister();
	void WriteToMmu(Region region, uint16_t address, uint8_t value) const;

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

	static const uint16_t lcd_control_register{ 0x0040 };
	static const uint16_t lcd_status_register{ 0x0041 };
	static const uint16_t scroll_y_register{ 0x0042 };
	static const uint16_t scroll_x_register{ 0x0043 };
	static const uint16_t current_line_register{ 0x0044 };
	static const uint16_t y_compare_register{ 0x0045 };
	static const uint16_t bg_palette_register{ 0x0047 };
	static const uint16_t obj_palette_0_register{ 0x0048 };
	static const uint16_t obj_palette_1_register{ 0x0049 };
	static const uint16_t window_y_position_register{ 0x004A };
	static const uint16_t window_x_position_plus_7_register{ 0x004B };

	TileSet tileset_;
	std::map<TileMap::Number, TileMap> tilemaps_;
	Renderer renderer_;
	Background background_;
	std::array<Sprite, 40> sprites_;
	Palette bg_palette_;
	std::map<ObjPalette, Palette> obj_palettes_;

	IMMU &mmu_;
	mutable bool writing_to_mmu_{ false };

	Mode current_mode_{ Mode::ReadingOAM };
	const std::map<Mode, std::unique_ptr<State>> states_;
	size_t ticks_current_period_{ 0 };

	uint8_t current_line_{ 0 };
	uint8_t line_compare_{ 0 };

	// LCD Control
	bool lcd_on_{ true };
	bool sprites_on_{ true };
	bool window_on_{ true };
	TileMap::Number current_bg_tilemap_{ TileMap::Number::Zero };
	TileSet::Number current_bg_tileset_{ TileSet::Number::Zero };
	TileMap::Number current_window_tilemap_{ TileMap::Number::Zero };
	Sprite::Size sprites_size_{ Sprite::Size::Pixels8x8 };

	// LCD Status
	bool enable_line_compare_interrupt_{ false };
	bool enable_oam_interrupt_{ false };
	bool enable_vblank_interrupt_{ false };
	bool enable_hblank_interrupt_{ false };
	bool line_coincidence_{ false };

private:
	GPU(const GPU&) = delete;
	GPU(GPU&&) = delete;
	GPU& operator=(const GPU&) = delete;
	GPU& operator=(GPU&&) = delete;
};
