#include "Keys.h"
#include <type_traits>
#include <iostream>
#include <sstream>
#include <list>

KeyPad::KeyPad(IMMU &mmu) :
	mmu_(mmu)
{
	direction_keys_pressed_[DirectionKeys::Up] = false;
	direction_keys_pressed_[DirectionKeys::Down] = false;
	direction_keys_pressed_[DirectionKeys::Left] = false;
	direction_keys_pressed_[DirectionKeys::Right] = false;
	button_keys_pressed_[ButtonKeys::A] = false;
	button_keys_pressed_[ButtonKeys::B] = false;
	button_keys_pressed_[ButtonKeys::Start] = false;
	button_keys_pressed_[ButtonKeys::Select] = false;
}

void KeyPad::HandleKeys(int key, int action)
{
	const bool key_pressed{ ((GLFW_PRESS == action) || (GLFW_REPEAT == action)) };

	switch (key)
	{
	case GLFW_KEY_W:
		direction_keys_pressed_[DirectionKeys::Up] = key_pressed;
		break;
	case GLFW_KEY_A:
		direction_keys_pressed_[DirectionKeys::Left] = key_pressed;
		break;
	case GLFW_KEY_S:
		direction_keys_pressed_[DirectionKeys::Down] = key_pressed;
		break;
	case GLFW_KEY_D:
		direction_keys_pressed_[DirectionKeys::Right] = key_pressed;
		break;
	case GLFW_KEY_J:
		button_keys_pressed_[ButtonKeys::B] = key_pressed;
		break;
	case GLFW_KEY_K:
		button_keys_pressed_[ButtonKeys::A] = key_pressed;
		break;
	case GLFW_KEY_SPACE:
		button_keys_pressed_[ButtonKeys::Start] = key_pressed;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		button_keys_pressed_[ButtonKeys::Select] = key_pressed;
		break;
	default:
		return;
	}

	// Only request interrupt during the initial press, not while repeating that same press
	if (GLFW_PRESS == action)
	{
		auto interrupt_flags = mmu_.Read8bitFromMemory(interrupt_flags_register_);
		interrupt_flags |= 0x10;
		// Bits 5-7 are unused, leave them set to 1
		interrupt_flags |= 0xE0;
		WriteToMmu(interrupt_flags_register_, interrupt_flags);
	}
}

uint8_t KeyPad::KeyStatusToByte(KeyGroups requested_key_group) const
{
	switch (requested_key_group)
	{
	case KeyGroups::Directions:
		{uint8_t value{ 0x3F };
		for (const auto& pair : direction_keys_pressed_)
		{
			if (pair.second)
			{
				value &= ~(static_cast<std::underlying_type_t<DirectionKeys>>(pair.first));
			}
		}
		return value; }
	case KeyGroups::Buttons:
		{uint8_t value{ 0x3F };
		for (const auto& pair : button_keys_pressed_)
		{
			if (pair.second)
			{
				value &= ~(static_cast<std::underlying_type_t<ButtonKeys>>(pair.first));
			}
		}
		return value; }
	default:
		//std::stringstream msg;
		std::cout << "Trying to access invalid key group: " << requested_key_group << std::endl;
		//throw std::logic_error(msg.str());
		return 0x3F;
	}
}

void KeyPad::OnMemoryWrite(const Memory::Address &address, uint8_t value)
{
	if (writing_to_mmu_)
	{
		return;
	}

	if (keypad_control_register_ == address)
	{
		std::list<KeyGroups> requested_key_groups;
		// All keypad-related bits are active-low
		if (((value & 0x30) & static_cast<std::underlying_type_t<KeyGroups>>(KeyGroups::Directions)) == 0)
		{
			requested_key_groups.push_back(KeyGroups::Directions);
		}
		if (((value & 0x30) & static_cast<std::underlying_type_t<KeyGroups>>(KeyGroups::Buttons)) == 0)
		{
			requested_key_groups.push_back(KeyGroups::Buttons);
		}
		
		if (requested_key_groups.size() == 1)
		{
			WriteToMmu(keypad_control_register_, KeyStatusToByte(requested_key_groups.front()));
		}
		else
		{
			// If none or both key groups are requested, just return no keys pressed
			WriteToMmu(keypad_control_register_, 0x3F);
		}
	}
}

void KeyPad::WriteToMmu(const Memory::Address &address, uint8_t value) const
{
	writing_to_mmu_ = true;
	mmu_.Write8bitToMemory(address, value);
	writing_to_mmu_ = false;
}

std::ostream& operator << (std::ostream& os, const KeyGroups& key_column)
{
	switch (key_column)
	{
	case KeyGroups::Directions:
		os << "Directions";
		break;
	case KeyGroups::Buttons:
		os << "Buttons";
		break;
	default:
		os << static_cast<size_t>(key_column);
		break;
	}

	return os;
}
