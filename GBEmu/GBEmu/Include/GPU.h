#pragma once

#include <map>
#include <memory>
#include <array>
#include "Clock.h"
#include "MMU.h"

class GPU : public MMUObserver
{
private:
	enum class Mode
	{
		ReadingOAM,
		ReadingVRAM,
		HBlank,
		VBlank
	};

	class State
	{
	public:
		State(uint8_t ticks_per_period) : ticks_per_period_(ticks_per_period) {}
		// Returns the new mode, and the remainder of ticks that have to be accounted for the next period
		std::tuple<Mode, uint8_t> Lapse(uint8_t ticks_current_period, GPU &gpu);

	private:
		virtual Mode Transition(GPU &gpu) = 0;
		const uint8_t ticks_per_period_;

	private:
		State(const State&) = delete;
		State(State&&) = delete;
		State& operator=(const State&) = delete;
		State& operator=(State&&) = delete;
	};

	class ReadingOAM final : public State
	{
	public:
		ReadingOAM() : State(80) {}
	private:
		Mode Transition(GPU&) override { return Mode::ReadingVRAM; }
	};

	class ReadingVRAM final : public State
	{
	public:
		ReadingVRAM() : State(172) {}
	private:
		Mode Transition(GPU&) override;
	};

	class HBlank final : public State
	{
	public:
		HBlank() : State(204) {}
	private:
		Mode Transition(GPU &gpu) override;
	};

	class VBlank final : public State
	{
	public:
		VBlank() : State(uint8_t(456)) {}
	private:
		Mode Transition(GPU &gpu) override;
	};

	class TileSet
	{
	public:
		static const uint32_t num_tiles_{ 128 * 3 };
		static const uint32_t tile_width_{ 8 };
		static const uint32_t tile_height_{ 8 };

		TileSet() = default;
		~TileSet() = default;

		void WriteTileData(uint32_t tile_index, uint8_t x, uint8_t y, uint8_t value);

	private:
		std::array<uint8_t, num_tiles_ * tile_width_ * tile_height_> tileset_data_;
	};
	
	enum class TileSetIndex
	{
		Zero,
		One
	};

	enum class TileMapIndex
	{
		Zero,
		One
	};

public:
	GPU();
	~GPU() = default;

	void Lapse(const Clock &clock);
	void RenderScanLine();
	void RefreshScreen() {}
	void ResetCurrentLine() { current_line_ = 0; }
	uint8_t IncrementCurrentLine() { return ++current_line_; }

	void OnMemoryWrite(MMU::Region region, uint16_t address, uint8_t value) override;

private:
	static std::map<Mode, std::unique_ptr<State>> InitStateMap();

private:
	std::array<uint8_t, 8192> vram_;
	TileSet tileset_;

	Mode current_mode_{ Mode::ReadingOAM };
	const std::map<Mode, std::unique_ptr<State>> states_;
	uint8_t ticks_current_period_{ 0 };
	uint8_t current_line_{ 0 };
	uint8_t bg_scroll_x{ 0 };
	uint8_t bg_scroll_y{ 0 };

	bool lcd_on_{ true };
	bool background_on_{ true };
	TileMapIndex background_tile_map_{ TileMapIndex::Zero };

private:
	GPU(const GPU&) = delete;
	GPU(GPU&&) = delete;
	GPU& operator=(const GPU&) = delete;
	GPU& operator=(GPU&&) = delete;
};
