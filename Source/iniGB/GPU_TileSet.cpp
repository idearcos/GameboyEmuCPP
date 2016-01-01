#include "GPU_TileSet.h"

TileSet::TileSet(size_t num_tiles, size_t tile_width, size_t tile_height) :
	num_tiles_(num_tiles),
	tile_width_(tile_width),
	tile_height_(tile_height)
{
	for (auto i = 0; i < num_tiles; i++)
	{
		tileset_data_.emplace_back(tile_width, tile_height);
	}
}

Tile& TileSet::GetTile(size_t tile_number)
{
	try
	{
		return tileset_data_.at(tile_number);
	}
	catch (std::out_of_range &)
	{
		throw std::logic_error("Trying to get a tile out of the tileset's range");
	}
}

const Tile& TileSet::GetTile(Number tileset_number, uint8_t tile_number) const
{
	auto real_tile_number = (tileset_number == Number::One) ? tile_number : 256 + static_cast<int8_t>(tile_number);
	return const_cast<const Tile&>(const_cast<TileSet*>(this)->GetTile(real_tile_number));
}

void TileSet::WritePixel(size_t tile_number, size_t x, size_t y, uint8_t value)
{
	GetTile(tile_number).WritePixel(x, y, value);
}
