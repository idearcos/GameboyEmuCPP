#include "GPU_TileMap.h"

TileMap::TileMap(size_t map_width, size_t map_height, size_t tile_width, size_t tile_height) :
	map_width_(map_width),
	map_height_(map_height),
	tile_width_(tile_width),
	tile_height_(tile_height),
	tilemap_data_(map_width * map_height, 0)
{

}

TileMap::TileNumber TileMap::GetTileNumber(size_t line, size_t scroll_y, size_t scroll_x) const
{
	// Current line and scroll Y are counted in pixels.
	// Divide by 8 (rounding down) to get target tile's row.
	const auto target_tile_row = ((line + scroll_y) & 0xFF) / tile_height_;

	// Divide scroll x (in pixels) by 8 (rounding down) to get target tile's column.
	const auto target_tile_column = scroll_x / tile_width_;

	try
	{
		return tilemap_data_.at((target_tile_row * map_width_) + target_tile_column);
	}
	catch (std::out_of_range &)
	{
		throw std::runtime_error("Tried to access tile map out of range");
	}
}
