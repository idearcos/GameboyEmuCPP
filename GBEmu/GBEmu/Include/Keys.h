#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include "GLFW/glfw3.h"
#include "IMMU.h"
#include "MMUObserver.h"

enum class Keys : uint8_t
{
	Up = 0x04,
	Down = 0x08,
	Left = 0x02,
	Right = 0x01,
	A = 0x01,
	B = 0x02,
	Start = 0x08,
	Select = 0x04
};

enum class KeyColumn : uint8_t
{
	Bit4 = 0x10,
	Bit5 = 0x20
};

std::ostream& operator << (std::ostream& os, const KeyColumn& key_column);

class KeyPad : public MMUObserver
{
public:
	KeyPad(IMMU &mmu);
	~KeyPad() = default;

	void HandleKeys(int key, int action);

	uint8_t KeyStatusToByte(KeyColumn requested_column) const;

	void OnMemoryWrite(const Memory::Address &address, uint8_t value) override;

private:
	void WriteToMmu(const Memory::Address &address, uint8_t value) const;

private:
	const Memory::Address keypad_control_register_{ 0xFF00 };

	using KeyStatus = std::map<Keys, bool>;
	std::map<KeyColumn, KeyStatus> keys_pressed_;
	IMMU &mmu_;

	mutable bool writing_to_mmu_{ false };

private:
	KeyPad(const KeyPad&) = delete;
	KeyPad(KeyPad&&) = delete;
	KeyPad& operator=(const KeyPad&) = delete;
	KeyPad& operator=(KeyPad&&) = delete;
};
