#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include "GLFW/glfw3.h"
#include "IMMU.h"
#include "MMUObserver.h"

enum class DirectionKeys : uint8_t
{
	Right = 0x01,
	Left = 0x02,
	Up = 0x04,
	Down = 0x08,
};

enum class ButtonKeys : uint8_t
{
	A = 0x01,
	B = 0x02,
	Select = 0x04,
	Start = 0x08
};

enum class KeyGroups : uint8_t
{
	Directions = 0x10,
	Buttons = 0x20
};

std::ostream& operator << (std::ostream& os, const KeyGroups& key_column);

class KeyPad : public MMUObserver
{
public:
	KeyPad(IMMU &mmu);
	~KeyPad() = default;

	void HandleKeys(int key, int action);

	uint8_t KeyStatusToByte(KeyGroups requested_key_group) const;

	void OnMemoryWrite(const Memory::Address &address, uint8_t value) override;

private:
	void WriteToMmu(const Memory::Address &address, uint8_t value) const;

private:
	const Memory::Address keypad_control_register_{ 0xFF00 };

	std::map<DirectionKeys, bool> direction_keys_pressed_;
	std::map<ButtonKeys, bool> button_keys_pressed_;
	IMMU &mmu_;

	mutable bool writing_to_mmu_{ false };

private:
	KeyPad(const KeyPad&) = delete;
	KeyPad(KeyPad&&) = delete;
	KeyPad& operator=(const KeyPad&) = delete;
	KeyPad& operator=(KeyPad&&) = delete;
};
