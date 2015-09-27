#pragma once

#include <cstdint>
#include <vector>

class TileMap
{
public:
	using TileNumber = uint8_t;

	enum class Number
	{
		Zero,
		One
	};

	TileMap(size_t map_width, size_t map_height, size_t tile_width, size_t tile_height);
	~TileMap() = default;

	TileNumber GetTileNumber(size_t line, size_t scroll_y, size_t scroll_x) const;

private:
	const size_t map_width_;
	const size_t map_height_;
	const size_t tile_width_;
	const size_t tile_height_;

	std::vector<TileNumber> tilemap_data_;

private:
	TileMap(const TileMap&) = delete;
	TileMap(TileMap&&) = delete;
	TileMap& operator=(const TileMap&) = delete;
	TileMap& operator=(TileMap&&) = delete;
};
