#pragma once

#include <map>
#include <memory>
#include <array>
#include "Clock.h"
#include "MMU.h"
#include "GPU_States.h"
#include "GPU_TileSet.h"
#include "GPU_TileMap.h"

class GPU : public MMUObserver
{
public:
	GPU();
	~GPU() = default;

	void Lapse(const Clock &clock);
	void RenderScanLine();
	void RefreshScreen() {}
	void ResetCurrentLine() { current_line_ = 0; }
	size_t IncrementCurrentLine() { return ++current_line_; }

	void OnMemoryWrite(MMU::Region region, uint16_t address, uint8_t value) override;

private:
	static std::map<Mode, std::unique_ptr<State>> InitStateMap();
	inline bool IsAddressInTileSet(uint16_t address) const;
	inline bool IsAddressInTileMap(uint16_t address, TileMap::Number tilemap_number) const;
	size_t GetAbsoluteTileNumber(uint8_t tile_number, TileSet::Number tileset_number) const;

private:
	// All addresses are relative to the beginning of VRAM
	static const uint16_t tileset1_start{ 0x0000 };
	static const uint16_t tileset0_start{ 0x0800 };
	static const size_t tileset_size{ 0x1000 };
	static const size_t tileset_total_size{ 0x1800 };
	static const uint16_t tilemap0_start{ 0x1800 };
	static const uint16_t tilemap1_start{ 0x1C00 };
	static const uint16_t tilemap_size{ 0x0400 };
	static const size_t tile_width{ 8 };
	static const size_t tile_height{ 8 };
	static const size_t tile_size{ tile_width * tile_height };
	static const size_t num_tiles_in_set{ 384 };

	std::array<uint8_t, 8192> vram_;
	TileSet tileset_;
	std::map<TileMap::Number, TileMap> tilemaps_;

	Mode current_mode_{ Mode::ReadingOAM };
	const std::map<Mode, std::unique_ptr<State>> states_;
	size_t ticks_current_period_{ 0 };
	size_t current_line_{ 0 };
	size_t bg_scroll_x_{ 0 };
	size_t bg_scroll_y_{ 0 };

	bool lcd_on_{ true };
	bool background_on_{ true };
	TileMap::Number current_bg_tilemap_{ TileMap::Number::Zero };

private:
	GPU(const GPU&) = delete;
	GPU(GPU&&) = delete;
	GPU& operator=(const GPU&) = delete;
	GPU& operator=(GPU&&) = delete;
};
