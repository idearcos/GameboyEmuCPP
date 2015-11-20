#pragma once

#include <cstdint>
#include <vector>
#include "GPU_TileSet.h"

class Renderer;
class TileMap;
class Palette;

class Window
{
public:
	Window(size_t screen_width, size_t screen_height);
	~Window() = default;

	void Enable(bool window_on);
	void SetPositionX(uint8_t window_position_x);
	void SetPositionY(uint8_t window_position_y);

	void RenderLine(Renderer &renderer, const TileSet &tileset, TileSet::Number tileset_number,
		const TileMap &tilemap, const Palette &window_palette, uint8_t current_line) const;

private:
	bool window_on_{ false };
	uint8_t window_position_x_{ 0 };
	uint8_t window_position_y_{ 0 };
	mutable uint8_t line_to_be_rendered_{ 0 };

	const size_t screen_width_;
	const size_t screen_height_;

private:
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;
};
