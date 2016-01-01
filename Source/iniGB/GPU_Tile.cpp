#include "GPU_Tile.h"
#include <sstream>

Tile::Tile(size_t tile_width, size_t tile_height) :
	tile_width_(tile_width),
	tile_height_(tile_height),
	tile_data_(tile_width * tile_height, 0)
{

}

Tile::Tile(const Tile& rhs) :
	tile_width_(rhs.tile_width_),
	tile_height_(rhs.tile_height_),
	tile_data_(rhs.tile_data_)
{

}

Tile::Tile(Tile&& rhs) :
	tile_width_(rhs.tile_width_),
	tile_height_(rhs.tile_height_),
	tile_data_(std::move(rhs.tile_data_))
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
		std::stringstream msg;
		msg << "Trying to write a pixel out of tile boundaries: (" << x << "," << y << ")";
		throw std::logic_error(msg.str());
	}

	tile_data_[tile_width_ * y + x] = value;
}
