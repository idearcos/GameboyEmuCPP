#include "Keys.h"
#include <type_traits>
#include <iostream>
#include <sstream>

KeyPad::KeyPad(IMMU &mmu) :
	mmu_(mmu)
{
	keys_pressed_[KeyColumn::Bit5][Keys::Up] = false;
	keys_pressed_[KeyColumn::Bit5][Keys::Down] = false;
	keys_pressed_[KeyColumn::Bit5][Keys::Left] = false;
	keys_pressed_[KeyColumn::Bit5][Keys::Right] = false;
	keys_pressed_[KeyColumn::Bit4][Keys::A] = false;
	keys_pressed_[KeyColumn::Bit4][Keys::B] = false;
	keys_pressed_[KeyColumn::Bit4][Keys::Start] = false;
	keys_pressed_[KeyColumn::Bit4][Keys::Select] = false;
}

void KeyPad::HandleKeys(int key, int action)
{
	const bool key_pressed{ ((GLFW_PRESS == action) || (GLFW_REPEAT == action)) };

	switch (key)
	{
	case GLFW_KEY_W:
		keys_pressed_[KeyColumn::Bit5][Keys::Up] = key_pressed;
		break;
	case GLFW_KEY_A:
		keys_pressed_[KeyColumn::Bit5][Keys::Left] = key_pressed;
		break;
	case GLFW_KEY_S:
		keys_pressed_[KeyColumn::Bit5][Keys::Down] = key_pressed;
		break;
	case GLFW_KEY_D:
		keys_pressed_[KeyColumn::Bit5][Keys::Right] = key_pressed;
		break;
	case GLFW_KEY_J:
		keys_pressed_[KeyColumn::Bit4][Keys::B] = key_pressed;
		break;
	case GLFW_KEY_K:
		keys_pressed_[KeyColumn::Bit4][Keys::A] = key_pressed;
		break;
	case GLFW_KEY_SPACE:
		keys_pressed_[KeyColumn::Bit4][Keys::Start] = key_pressed;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		keys_pressed_[KeyColumn::Bit4][Keys::Select] = key_pressed;
		break;
	default:
		return;
	}
}

uint8_t KeyPad::KeyStatusToByte(KeyColumn requested_column) const
{
	try
	{
		uint8_t value{ 0x0F };
		for (const auto& pair : keys_pressed_.at(requested_column))
		{
			if (pair.second)
			{
				value &= ~(static_cast<std::underlying_type_t<Keys>>(pair.first));
			}
		}
		return value;
	}
	catch (std::out_of_range &)
	{
		std::stringstream msg;
		msg << "Trying to access invalid keypad column: " << requested_column;
		throw std::logic_error(msg.str());
		//return 0x0F;
	}
}

void KeyPad::OnMemoryWrite(Region region, uint16_t address, uint8_t value)
{
	if (writing_to_mmu_)
	{
		return;
	}

	if (Region::IO == region)
	{
		if (keypad_control_register == address)
		{
			const auto new_value = KeyStatusToByte(static_cast<KeyColumn>(value & 0x30));
			
			WriteToMmu(Region::IO, keypad_control_register, new_value);
		}
	}
}

void KeyPad::WriteToMmu(Region region, uint16_t address, uint8_t value) const
{
	writing_to_mmu_ = true;
	mmu_.Write8bitToMemory(region, address, value);
	writing_to_mmu_ = false;
}

std::ostream& operator << (std::ostream& os, const KeyColumn& key_column)
{
	switch (key_column)
	{
	case KeyColumn::Bit4:
		os << "Bit4";
		break;
	case KeyColumn::Bit5:
		os << "Bit5";
		break;
	default:
		os << static_cast<size_t>(key_column);
		break;
	}

	return os;
}
