#include "GPU_Sprite.h"
#include <sstream>
#include "GPU_Renderer.h"
#include "GPU_TileSet.h"
#include "GPU_Palette.h"

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
	is_above_background_ = (flags & 0x80) != 0;
	is_vertically_flipped_ = (flags & 0x40) != 0;
	is_horizontally_flipped_ = (flags & 0x20) != 0;
	obj_palette_ = ((flags & 0x10) != 0) ? ObjPalette::One : ObjPalette::Zero;
}

void Sprite::RenderSprite(Renderer &renderer, const TileSet &tileset, const std::map<ObjPalette, Palette> &obj_palettes,
	Sprite::Size sprite_size, size_t current_line) const
{
	try
	{
		// Check if current line is affected by sprite
		const auto sprite_height = (Sprite::Size::Pixels8x16 == sprite_size) ? 16 : 8;
		const uint8_t line_in_sprite{ static_cast<uint8_t>(current_line - y_position_) };
		if ((line_in_sprite >= 0) && (line_in_sprite < sprite_height))
		{
			const auto affected_tile_number = (Sprite::Size::Pixels8x8 == sprite_size) ? tile_number_ :
				(tile_number_ & (~0x01)) + (line_in_sprite / 8);
			
			const auto tile = tileset.GetTile(affected_tile_number);
			const auto line_in_tile = (Sprite::Size::Pixels8x16 == sprite_size) ? (line_in_sprite % 8) : line_in_sprite;
			for (auto x = 0; x < tile.GetWidth(); x++)
			{
				if (is_above_background_ || (Color::Transparent == renderer.GetPixel(x_position_ + x, y_position_ + line_in_tile)))
				{
					renderer.RenderPixel(x_position_ + x, y_position_ + line_in_tile, obj_palettes.at(obj_palette_).GetColor(tile.ReadPixel(x, line_in_tile)));
				}
			}
		}	
	}
	catch (std::out_of_range &)
	{
		std::stringstream msg;
		msg << "Trying to access invalid object palette " << static_cast<size_t>(obj_palette_) << std::endl;
		throw std::logic_error(msg.str());
	}
}
