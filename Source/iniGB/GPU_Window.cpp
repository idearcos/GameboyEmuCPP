#include "GPU_Window.h"
#include "GPU_Renderer.h"
#include "GPU_TileSet.h"
#include "GPU_TileMap.h"
#include "GPU_Palette.h"

Window::Window(size_t screen_width, size_t screen_height) :
	screen_width_(screen_width),
	screen_height_(screen_height)
{

}

void Window::Enable(bool window_on)
{
	window_on_ = window_on;
}

void Window::SetPositionX(uint8_t window_position_x)
{
	window_position_x_ = window_position_x;
}

void Window::SetPositionY(uint8_t window_position_y)
{
	window_position_y_ = window_position_y;
}

void Window::RenderLine(Renderer &renderer, const TileSet &tileset, TileSet::Number tileset_number,
	const TileMap &tilemap, const Palette &window_palette, uint8_t current_line) const
{
	// When a new frame begins, reset to beginning of Window
	if (0 == current_line)
	{
		line_to_be_rendered_ = 0;
	}

	if (window_on_)
	{
		// There is nothing to draw for lines before the vertical position of the window
		if (current_line < window_position_y_)
		{
			return;
		}

		// The line (of the tiles) to be drawn, depending on the vertical scroll of the background and the current line being drawn (last 3 bits, 0-7)
		const uint8_t line_in_tile{ static_cast<uint8_t>(line_to_be_rendered_ & 0x07) };

		// There is nothing to draw for pixels before the horizontal position of the window
		size_t pixels_drawn = window_position_x_;
		while (pixels_drawn < screen_width_)
		{
			const auto tile_number = tilemap.GetTileNumber(line_to_be_rendered_, pixels_drawn);

			const auto tile = tileset.GetTile(tileset_number, tile_number);

			for (auto x_in_tile = 0; (x_in_tile < tile.GetWidth()) && (pixels_drawn < screen_width_); x_in_tile++)
			{
				try
				{
					const auto color = window_palette.GetColor(tile.ReadPixel(x_in_tile, line_in_tile));
					renderer.RenderPixel(pixels_drawn, current_line, color);
				}
				catch (std::out_of_range &)
				{
					throw std::runtime_error("Trying to access invalid palette");
				}
				pixels_drawn += 1;
			}
		}

		// Only increase the line to be rendered if the window is enabled
		line_to_be_rendered_ += 1;
	}
}
