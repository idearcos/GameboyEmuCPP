#pragma once

#include <cstdint>
#include <vector>

class Tile
{
public:
	Tile(size_t tile_width, size_t tile_height);
	Tile(const Tile& rhs);
	Tile(Tile&& rhs);
	~Tile() = default;

	size_t GetWidth() const { return tile_width_; }
	size_t GetHeight() const { return tile_height_; }
	uint8_t ReadPixel(size_t x, size_t y) const;
	void WritePixel(size_t x, size_t y, uint8_t value);

private:
	const size_t tile_width_;
	const size_t tile_height_;

	std::vector<uint8_t> tile_data_;

private:
	Tile& operator=(const Tile&) = delete;
	Tile& operator=(Tile&&) = delete;
};
