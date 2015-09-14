#pragma once

#include <cstdint>
#include <map>
#include <functional>

#include "Registers.h"
#include "MMU.h"
#include "Clock.h"

class Z80
{
public:
	Z80();
	~Z80() = default;

#pragma region 8-bit load group
	// LD r, r'
	Clock LoadRegister(Register8bit dest, Register8bit source);
	// LD r, n
	Clock LoadRegister(Register8bit dest, uint8_t value);
	// LD r, (dd)
	Clock LoadRegisterFromAddress(Register8bit dest, Register16bit source_addr);
	// LD (dd), r
	Clock LoadAddressFromRegister(Register16bit dest_addr, Register8bit source);
	// LD (dd), n
	Clock LoadAddress(Register16bit dest_addr, uint8_t value);
	// LD r, (nn)
	Clock LoadRegisterFromAddress(Register8bit dest, uint16_t addr);
	// LD (nn), r
	Clock LoadAddressFromRegister(uint16_t addr, Register8bit source);
#pragma endregion

#pragma region 16-bit load group
	// LD dd, nn
	Clock LoadRegister(Register16bit dest, uint16_t value);
	// LD dd, dd'
	Clock LoadRegister(Register16bit dest, Register16bit source);
	// PUSH qq
	Clock PushToStack(Register16bit source);
	// POP qq
	Clock PopFromStack(Register16bit dest);
#pragma endregion

#pragma region GB-specific load instruction group
	// LDI (dd), r
	Clock LoadAddressAndIncrement(Register16bit dest_addr, Register8bit source);
	// LDD (dd), r
	Clock LoadAddressAndDecrement(Register16bit dest_addr, Register8bit source);
	// LDI r, (dd)
	Clock LoadRegisterAndIncrement(Register8bit dest, Register16bit source_addr);
	// LDD r, (dd)
	Clock LoadRegisterAndDecrement(Register8bit dest, Register16bit source_addr);
#pragma endregion

#pragma region 8-bit arithmetic group
	// ADD A, n
	Clock Add(uint8_t value);
	// ADD A, r
	Clock Add(Register8bit source);
	// ADD A, (dd)
	Clock Add(Register16bit source_addr);
	// ADC A, n
	Clock AddPlusCarry(uint8_t value);
	// ADC A, r
	Clock AddPlusCarry(Register8bit source);
	// ADC A, (dd)
	Clock AddPlusCarry(Register16bit source_addr);
	// SUB A, n
	Clock Sub(uint8_t value);
	// SUB A, r
	Clock Sub(Register8bit source);
	// SUB A, (dd)
	Clock Sub(Register16bit source_addr);
	// SBC A, n
	Clock SubMinusCarry(uint8_t value);
	// SBC A, r
	Clock SubMinusCarry(Register8bit source);
	// SBC A, (dd)
	Clock SubMinusCarry(Register16bit source_addr);
	// AND A, n
	Clock And(uint8_t value);
	// AND A, r
	Clock And(Register8bit source);
	// AND A, (dd)
	Clock And(Register16bit source_addr);
	// OR A, n
	Clock Or(uint8_t value);
	// OR A, r
	Clock Or(Register8bit source);
	// OR A, (dd)
	Clock Or(Register16bit source_addr);
	// XOR A, n
	Clock Xor(uint8_t value);
	// XOR A, r
	Clock Xor(Register8bit source);
	// XOR A, (dd)
	Clock Xor(Register16bit source_addr);
	// CP A, n
	Clock Compare(uint8_t value);
	// CP A, r
	Clock Compare(Register8bit source);
	// CP A, (dd)
	Clock Compare(Register16bit source_addr);
	// INC r
	Clock Increment(Register8bit reg);
	// INC (dd)
	Clock Increment(Register16bit reg_addr);
	// DEC r
	Clock Decrement(Register8bit reg);
	// DEC (dd)
	Clock Decrement(Register16bit reg_addr);
#pragma endregion

#pragma region General purpose arithmetic and CPU control groups
	// DAA
	Clock DAA();
#pragma endregion

private:
	Registers registers;
	Clock clock;
	MMU mmu;
	std::map<uint8_t, std::function<Clock()>> operations_;
};
