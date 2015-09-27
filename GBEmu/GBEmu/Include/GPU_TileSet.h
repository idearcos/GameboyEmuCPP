#pragma once

#include <cstdint>
#include <vector>
#include "GPU_Tile.h"
#include "GPU_TileMap.h"

class TileSet
{
public:
	enum class Number
	{
		Zero,
		One
	};

	TileSet(size_t num_tiles, size_t tile_width, size_t tile_height);
	~TileSet() = default;

	Tile& GetTile(size_t tile_number);
	uint8_t ReadPixel(size_t tile_number, size_t x, size_t y);
	void WritePixel(size_t tile_number, size_t x, size_t y, uint8_t value);

private:
	const size_t num_tiles_;
	const size_t tile_width_;
	const size_t tile_height_;

	std::vector<Tile> tileset_data_;

private:
	TileSet(const TileSet&) = delete;
	TileSet(TileSet&&) = delete;
	TileSet& operator=(const TileSet&) = delete;
	TileSet& operator=(TileSet&&) = delete;
};
