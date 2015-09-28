#pragma once

#include <map>
#include <memory>
#include <array>
#include "Clock.h"
#include "MMU.h"
#include "GPU_States.h"
#include "GPU_TileSet.h"
#include "GPU_TileMap.h"
#include "GPU_Renderer.h"

class GPU : public MMUObserver
{
public:
	GPU(MMU &mmu);
	~GPU() = default;

	void Lapse(const Clock &clock);
	void RenderScanLine();
	void RefreshScreen();
	void ResetCurrentLine() { current_line_ = 0; }
	size_t IncrementCurrentLine() { return ++current_line_; }

	void OnMemoryWrite(MMU::Region region, uint16_t address, uint8_t value) override;

private:
	static std::map<Mode, std::unique_ptr<State>> InitStateMap();
	inline bool IsAddressInTileSet(uint16_t address) const;
	inline bool IsAddressInTileMap(uint16_t address, TileMap::Number tilemap_number) const;
	size_t GetAbsoluteTileNumber(TileMap::TileNumber tile_number, TileSet::Number tileset_number) const;

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
	static const size_t tile_width_{ 8 };
	static const size_t tile_height_{ 8 };
	static const size_t tile_size_{ tile_width_ * tile_height_ };
	static const size_t num_tiles_in_set_{ 384 };

	TileSet tileset_;
	std::map<TileMap::Number, TileMap> tilemaps_;
	Renderer renderer_;
	std::vector<uint8_t> framebuffer_;
	std::map<size_t, std::array<uint8_t, 3>> palette_;
	MMU &mmu_;

	Mode current_mode_{ Mode::ReadingOAM };
	const std::map<Mode, std::unique_ptr<State>> states_;
	size_t ticks_current_period_{ 0 };
	size_t current_line_{ 0 };
	size_t bg_scroll_x_{ 0 };
	size_t bg_scroll_y_{ 0 };

	bool lcd_on_{ true };
	bool background_on_{ true };
	bool sprites_on_{ true };
	bool window_on_{ true };
	TileMap::Number current_bg_tilemap_{ TileMap::Number::Zero };
	TileSet::Number current_bg_tileset_{ TileSet::Number::Zero };
	TileMap::Number current_window_tilemap_{ TileMap::Number::Zero };

private:
	GPU(const GPU&) = delete;
	GPU(GPU&&) = delete;
	GPU& operator=(const GPU&) = delete;
	GPU& operator=(GPU&&) = delete;
};
