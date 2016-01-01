#pragma once

#include <map>
#include "GPU_Color.h"

class Palette
{
public:
	Palette() = default;
	~Palette() = default;

	void SetPaletteData(uint8_t value)
	{
		for (auto i = 0; i < 4; i++)
		{
			switch ((value >> (i * 2)) & 3)
			{
			case 0:
				SetColor(i, Color::Transparent);
				break;
			case 1:
				SetColor(i, Color::LightGrey);
				break;
			case 2:
				SetColor(i, Color::DarkGrey);
				break;
			case 3:
				SetColor(i, Color::Black);
				break;
			}
		}
	}

	Color GetColor(size_t index) const
	{
		try
		{
			return palette_data_.at(index);
		}
		catch (std::out_of_range &)
		{
			throw std::logic_error("Trying to access palette data out of range");
		}
	}

	void SetColor(size_t index, Color color)
	{
		palette_data_[index] = color;
	}

private:
	std::map<size_t, Color> palette_data_;
};
