#pragma once

#include <cstdint>
#include <vector>
#include "GPU_TileSet.h"

class Renderer;
class TileMap;
class Palette;

class Background
{
public:
	Background(size_t screen_width, size_t screen_height);
	~Background() = default;

	void Enable(bool background_on);
	void SetScrollX(uint8_t bg_scroll_x);
	void SetScrollY(uint8_t bg_scroll_y);

	void RenderLine(Renderer &renderer, const TileSet &tileset, TileSet::Number tileset_number,
		const TileMap &tilemap, const Palette &bg_palette, uint8_t current_line) const;

private:
	bool background_on_{ false };
	uint8_t bg_scroll_x_{ 0 };
	uint8_t bg_scroll_y_{ 0 };

	const size_t screen_width_;
	const size_t screen_height_;

private:
	Background(const Background&) = delete;
	Background(Background&&) = delete;
	Background& operator=(const Background&) = delete;
	Background& operator=(Background&&) = delete;
};
