#include "GPU_Background.h"
#include "GPU_Renderer.h"
#include "GPU_TileSet.h"
#include "GPU_TileMap.h"
#include "GPU_Palette.h"

Background::Background(size_t screen_width, size_t screen_height) :
	screen_width_(screen_width),
	screen_height_(screen_height)
{

}

void Background::Enable(bool background_on)
{
	background_on_ = background_on;
}

void Background::SetScrollX(uint8_t bg_scroll_x)
{
	bg_scroll_x_ = bg_scroll_x;
}

void Background::SetScrollY(uint8_t bg_scroll_y)
{
	bg_scroll_y_ = bg_scroll_y;
}

void Background::RenderLine(Renderer &renderer, const TileSet &tileset, TileSet::Number tileset_number,
	const TileMap &tilemap, const Palette &bg_palette, uint8_t current_line) const
{
	if (background_on_)
	{
		// The pixel offset inside the first tile to be drawn, depending on the horizontal scroll of the background (last 3 bits, 0-7)
		size_t x_offset_in_tile = bg_scroll_x_ & 0x07;

		// The line (of the tiles) to be drawn, depending on the vertical scroll of the background and the current line being drawn (last 3 bits, 0-7)
		const uint8_t line_in_tile{ static_cast<uint8_t>((current_line + bg_scroll_y_) & 0x07) };

		size_t pixels_rendered = 0;
		while (pixels_rendered < screen_width_)
		{
			const auto tile_number = tilemap.GetTileNumber(current_line + bg_scroll_y_, pixels_rendered + bg_scroll_x_);

			const auto tile = tileset.GetTile(tileset_number, tile_number);

			for (auto x_in_tile = x_offset_in_tile; (x_in_tile < tile.GetWidth()) && (pixels_rendered < screen_width_); x_in_tile++)
			{
				try
				{
					const auto color = bg_palette.GetColor(tile.ReadPixel(x_in_tile, line_in_tile));
					renderer.RenderPixel(pixels_rendered, current_line, color);
				}
				catch (std::out_of_range &)
				{
					throw std::runtime_error("Trying to access invalid palette");
				}
				pixels_rendered += 1;
			}

			// For all tiles except the first one, the whole tile is to be rendered, so there is no offset whatsoever
			x_offset_in_tile = 0;
		}
	}
}
