#pragma once

#include <cstdint>
#include <map>

class Renderer;
class TileSet;
class Palette;

enum class ObjPalette : uint8_t
{
	Zero = 0,
	One = 1
};

class Sprite
{
public:
	enum class Size : uint8_t
	{
		Pixels8x8,
		Pixels8x16
	};

	Sprite() = default;
	~Sprite() = default;

	void SetPositionY(uint8_t y_position);
	void SetPositionX(uint8_t x_position);
	void SetTileNumber(uint8_t tile_number);
	void SetFlags(uint8_t flags);

	void RenderSprite(Renderer &renderer, const TileSet &tileset, const std::map<ObjPalette, Palette> &obj_palettes,
		Sprite::Size sprite_size, size_t current_line) const;

private:
	uint8_t y_position_{ 0 };
	uint8_t x_position_{ 0 };
	uint8_t tile_number_{ 0 };
	bool is_above_background_{ true };
	bool is_vertically_flipped_{ false };
	bool is_horizontally_flipped_{ false };
	ObjPalette obj_palette_{ ObjPalette::Zero };
};
