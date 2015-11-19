#include "GPU_Sprite.h"
#include <iostream>
#include <sstream>
#include "GPU_Renderer.h"
#include "GPU_TileSet.h"
#include "GPU_Palette.h"

std::ostream& operator << (std::ostream& os, const ObjPalette& obj_palette)
{
	switch (obj_palette)
	{
	case ObjPalette::Zero:
		os << "ObjPalette::Zero";
		break;
	case ObjPalette::One:
		os << "ObjPalette::One";
		break;
	default:
		os << static_cast<size_t>(obj_palette);
		break;
	}

	return os;
}

void Sprite::SetPositionY(uint8_t y_position)
{
	y_position_ = y_position - 16;
}

void Sprite::SetPositionX(uint8_t x_position)
{
	x_position_ = x_position - 8;
}

void Sprite::SetTileNumber(uint8_t tile_number)
{
	tile_number_ = tile_number;
}

void Sprite::SetFlags(uint8_t flags)
{
	is_above_background_ = (flags & 0x80) == 0;
	is_vertically_flipped_ = (flags & 0x40) != 0;
	is_horizontally_flipped_ = (flags & 0x20) != 0;
	obj_palette_ = ((flags & 0x10) != 0) ? ObjPalette::One : ObjPalette::Zero;
}

void Sprite::RenderSprite(Renderer &renderer, const TileSet &tileset, const std::map<ObjPalette, Palette> &obj_palettes,
	Sprite::Size sprite_size, size_t current_line) const
{
	try
	{
		const uint8_t sprite_height = (Sprite::Size::Pixels8x16 == sprite_size) ? 16 : 8;
		uint8_t line_in_sprite{ static_cast<uint8_t>(current_line - y_position_) };

		// Check if sprite must be rendered in the current line
		if ((line_in_sprite >= 0) && (line_in_sprite < sprite_height))
		{
			// Correct the affected line in the sprite, if vertical flip is enabled
			if (is_vertically_flipped_)
			{
				line_in_sprite = (sprite_height - 1) - line_in_sprite;
			}

			// Correct the affected tile number, if the sprite size is 8x16
			const uint8_t affected_tile_number = (Sprite::Size::Pixels8x8 == sprite_size) ? tile_number_ :
				(tile_number_ & (~0x01)) + (line_in_sprite / 8);
			
			const auto tile = tileset.GetTile(TileSet::Number::One, affected_tile_number);

			// Get the line of the tile to be rendered
			const auto line_in_tile = line_in_sprite % 8;

			// Iterate horizontally through all pixels in the line to be rendered
			for (size_t x = 0; x < tile.GetWidth(); x++)
			{
				// Check if current sprite is inside the screen
				if (((x_position_ + x) >= 0) && ((x_position_ + x) < 160))
				{
					// Correct the pixel to be rendered, if horizontal flip is enabled
					const auto x_to_render = is_horizontally_flipped_ ? (tile.GetWidth() - 1) - x : x;

					const auto sprite_pixel_color = obj_palettes.at(obj_palette_).GetColor(tile.ReadPixel(x_to_render, line_in_tile));

					// Only render if the color of the sprite in this pixel is not transparent
					if (Color::Transparent != sprite_pixel_color)
					{
						// Only render if the sprite is above the background OR if the background color value is 0 for this pixel (regardless of the bg palette)
						if (is_above_background_ || (0 == renderer.GetPixel(x_position_ + x, current_line)))
						{
							renderer.RenderPixel(x_position_ + x, current_line, sprite_pixel_color);
						}
					}
				}
			}
		}
	}
	catch (std::out_of_range &)
	{
		std::stringstream msg;
		msg << "Trying to access invalid object palette: " << obj_palette_;
		throw std::logic_error(msg.str());
	}
}
