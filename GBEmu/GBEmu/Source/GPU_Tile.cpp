#include "GPU_Tile.h"
#include <sstream>

Tile::Tile(size_t tile_width, size_t tile_height) :
	tile_width_(tile_width),
	tile_height_(tile_height),
	tile_data_(tile_width * tile_height, 0)
{

}

uint8_t Tile::ReadPixel(size_t x, size_t y) const
{
	if ((x >= tile_width_) || (y >= tile_height_))
	{
		std::stringstream msg;
		msg << "Trying to read a pixel out of tile boundaries: (" << x << "," << y << ")";
		throw std::logic_error(msg.str());
	}

	return tile_data_[tile_width_ * y + x];
}

void Tile::WritePixel(size_t x, size_t y, uint8_t value)
{
	if ((x >= tile_width_) || (y >= tile_height_))
	{
		throw std::out_of_range("Trying to write a pixel out of tile boundaries");
	}

	tile_data_[tile_width_ * y + x] = value;
}
